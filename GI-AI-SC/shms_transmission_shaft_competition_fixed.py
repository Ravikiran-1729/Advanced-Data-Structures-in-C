from __future__ import annotations

import math
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Tuple

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


# =========================================================
# PROBLEM CONSTANTS
# =========================================================
@dataclass
class ShaftConstants:
    # Geometry and layout (inches)
    L1: float = 6.0
    L2: float = 10.0
    L3: float = 10.0
    x_pulley: float = 0.0
    x_support_A: float = 6.0
    x_gear: float = 16.0
    x_support_B: float = 26.0

    # Material
    density_lb_per_in3: float = 0.2834
    E_psi: float = 29_000_000.0
    Sut_psi: float = 85_000.0
    Se_psi: float = 30_000.0
    safety_factor: float = 2.0

    # Transmission assumptions
    Tmax_lb_in: float = 3500.0
    Tmin_lb_in: float = 875.0
    pulley_diameter_in: float = 20.0
    gear_pitch_diameter_in: float = 10.0
    gear_pressure_angle_deg: float = 20.0
    pulley_mass_lbf: float = 60.0
    gear_mass_lbf: float = 25.0

    # V-belt assumptions
    groove_angle_2beta_deg: float = 90.0
    wrap_angle_deg: float = 210.0
    friction_mu: float = 0.31

    # Stress concentration factors
    Kf_bending: Tuple[float, float, float] = (2.05, 3.00, 2.40)
    Kfm_bending: Tuple[float, float, float] = (2.05, 3.00, 2.40)
    Kfs_torsion: float = 2.05
    Kfsm_torsion: float = 2.05

    # Design limits
    min_step_diff_in: float = 0.0787
    max_gear_deflection_in: float = 0.005

    # Manufacturability
    standard_step_in: float = 0.125
    manufacturability_weight_lb: float = 0.20

    # Numerical settings
    opt_npts: int = 180
    final_npts: int = 2000


# =========================================================
# STEPPED SHAFT PROBLEM
# =========================================================
class SteppedTransmissionShaftProblem:
    """
    Design variables:
        x = [d1, d2, d3] in inches

    Objective:
        minimize shaft weight + manufacturability penalty

    Constraints:
        g1..g3: actual diameters >= required fatigue diameters
        g4: d2 - d1 >= minimum step difference
        g5: d2 - d3 >= minimum step difference
        g6: gear-point deflection <= allowable
    """

    def __init__(self, const: ShaftConstants):
        self.c = const

        # Precompute all load-dependent data ONCE.
        self.precomputed_req = self.required_diameters_modified_goodman()
        self.opt_defl_cache = self.precompute_deflection_load_curve(self.c.opt_npts)
        self.final_defl_cache = self.precompute_deflection_load_curve(self.c.final_npts)

    @staticmethod
    def cumtrapz_np(y: np.ndarray, x: np.ndarray) -> np.ndarray:
        out = np.zeros_like(y, dtype=float)
        dx = np.diff(x)
        out[1:] = np.cumsum(0.5 * (y[:-1] + y[1:]) * dx)
        return out

    def nearest_standard(self, d: float) -> float:
        step = self.c.standard_step_in
        return round(d / step) * step

    # ----------------------------
    # Loading model
    # ----------------------------
    def belt_tension_ratio(self) -> float:
        beta = math.radians(self.c.groove_angle_2beta_deg / 2.0)
        alpha = math.radians(self.c.wrap_angle_deg)
        return math.exp(self.c.friction_mu * alpha / math.sin(beta))

    def transmitted_forces(self, T_lb_in: float) -> Dict[str, float]:
        r_pulley = self.c.pulley_diameter_in / 2.0
        D_gear = self.c.gear_pitch_diameter_in

        # Pulley-side forces
        Ft_p = T_lb_in / r_pulley
        ratio = self.belt_tension_ratio()
        if abs(ratio - 1.0) < 1e-12:
            T1 = Ft_p
            T2 = 0.0
        else:
            T2 = Ft_p / (ratio - 1.0)
            T1 = ratio * T2
        F_belt_horizontal = T1 + T2

        # Gear-side forces
        Ft_g = 2.0 * T_lb_in / D_gear
        phi = math.radians(self.c.gear_pressure_angle_deg)
        Fr_g = Ft_g * math.tan(phi)

        return {
            "F_belt_horizontal": F_belt_horizontal,
            "F_gear_tangential": Ft_g,
            "F_gear_radial": Fr_g,
            "W_pulley": self.c.pulley_mass_lbf,
            "W_gear": self.c.gear_mass_lbf,
        }

    def support_reactions(self, point_loads: List[Tuple[float, float]]) -> Tuple[float, float]:
        A = self.c.x_support_A
        B = self.c.x_support_B
        span = B - A
        total_load = sum(P for _, P in point_loads)
        moment_about_A = sum(P * (x - A) for x, P in point_loads)
        RB = moment_about_A / span
        RA = total_load - RB
        return RA, RB

    @staticmethod
    def heav(x: np.ndarray, a: float) -> np.ndarray:
        return (x >= a).astype(float)

    def bending_moment_distribution(self, point_loads: List[Tuple[float, float]], npts: int):
        x = np.linspace(0.0, self.c.x_support_B, npts)
        RA, RB = self.support_reactions(point_loads)
        A = self.c.x_support_A
        B = self.c.x_support_B

        H_A = self.heav(x, A)
        H_B = self.heav(x, B)

        V = RA * H_A + RB * H_B
        M = RA * (x - A) * H_A + RB * (x - B) * H_B

        for xp, P in point_loads:
            H = self.heav(x, xp)
            V -= P * H
            M -= P * (x - xp) * H

        return x, V, M, RA, RB

    def sectionwise_max_moment(self, x: np.ndarray, M: np.ndarray) -> Tuple[float, float, float]:
        s1 = (x >= 0.0) & (x <= self.c.x_support_A)
        s2 = (x >= self.c.x_support_A) & (x <= self.c.x_gear)
        s3 = (x >= self.c.x_gear) & (x <= self.c.x_support_B)

        return (
            float(np.max(np.abs(M[s1]))),
            float(np.max(np.abs(M[s2]))),
            float(np.max(np.abs(M[s3]))),
        )

    def fluctuating_section_moments(self) -> Dict[str, np.ndarray]:
        loads_max = self.transmitted_forces(self.c.Tmax_lb_in)
        horiz_max = [
            (self.c.x_pulley, loads_max["F_belt_horizontal"]),
            (self.c.x_gear, loads_max["F_gear_tangential"]),
        ]
        vert_max = [
            (self.c.x_pulley, loads_max["W_pulley"]),
            (self.c.x_gear, loads_max["W_gear"] + loads_max["F_gear_radial"]),
        ]

        xh, _, Mh_max, _, _ = self.bending_moment_distribution(horiz_max, npts=self.c.opt_npts)
        _, _, Mv_max, _, _ = self.bending_moment_distribution(vert_max, npts=self.c.opt_npts)
        Mres_max = np.sqrt(Mh_max**2 + Mv_max**2)

        loads_min = self.transmitted_forces(self.c.Tmin_lb_in)
        horiz_min = [
            (self.c.x_pulley, loads_min["F_belt_horizontal"]),
            (self.c.x_gear, loads_min["F_gear_tangential"]),
        ]
        vert_min = [
            (self.c.x_pulley, loads_min["W_pulley"]),
            (self.c.x_gear, loads_min["W_gear"] + loads_min["F_gear_radial"]),
        ]

        _, _, Mh_min, _, _ = self.bending_moment_distribution(horiz_min, npts=self.c.opt_npts)
        _, _, Mv_min, _, _ = self.bending_moment_distribution(vert_min, npts=self.c.opt_npts)
        Mres_min = np.sqrt(Mh_min**2 + Mv_min**2)

        max_sections = np.array(self.sectionwise_max_moment(xh, Mres_max))
        min_sections = np.array(self.sectionwise_max_moment(xh, Mres_min))

        M_mean = 0.5 * (max_sections + min_sections)
        M_alt = 0.5 * np.abs(max_sections - min_sections)

        return {
            "x": xh,
            "Mh_max": Mh_max,
            "Mv_max": Mv_max,
            "Mres_max": Mres_max,
            "Mh_min": Mh_min,
            "Mv_min": Mv_min,
            "Mres_min": Mres_min,
            "M_mean_sections": M_mean,
            "M_alt_sections": M_alt,
        }

    def required_diameters_modified_goodman(self) -> Dict[str, np.ndarray]:
        data = self.fluctuating_section_moments()

        Ma = data["M_alt_sections"]
        Mm = data["M_mean_sections"]

        Ta = 0.5 * (self.c.Tmax_lb_in - self.c.Tmin_lb_in)
        Tm = 0.5 * (self.c.Tmax_lb_in + self.c.Tmin_lb_in)

        Kf = np.array(self.c.Kf_bending, dtype=float)
        Kfm = np.array(self.c.Kfm_bending, dtype=float)
        Kfs = self.c.Kfs_torsion
        Kfsm = self.c.Kfsm_torsion

        term_alt = np.sqrt((Kf * Ma) ** 2 + 0.75 * (Kfs * Ta) ** 2) / self.c.Se_psi
        term_mean = np.sqrt((Kfm * Mm) ** 2 + 0.75 * (Kfsm * Tm) ** 2) / self.c.Sut_psi

        d_req = ((32.0 * self.c.safety_factor / math.pi) * (term_alt + term_mean)) ** (1.0 / 3.0)

        return {
            "required_diameters_in": d_req,
            "M_alt_sections_lb_in": Ma,
            "M_mean_sections_lb_in": Mm,
            "T_alt_lb_in": Ta,
            "T_mean_lb_in": Tm,
            **data,
        }

    def precompute_deflection_load_curve(self, npts: int) -> Dict[str, np.ndarray]:
        loads = self.transmitted_forces(self.c.Tmax_lb_in)

        horiz = [
            (self.c.x_pulley, loads["F_belt_horizontal"]),
            (self.c.x_gear, loads["F_gear_tangential"]),
        ]
        vert = [
            (self.c.x_pulley, loads["W_pulley"]),
            (self.c.x_gear, loads["W_gear"] + loads["F_gear_radial"]),
        ]

        xh, _, Mh, _, _ = self.bending_moment_distribution(horiz, npts=npts)
        _, _, Mv, _, _ = self.bending_moment_distribution(vert, npts=npts)
        M = np.sqrt(Mh**2 + Mv**2)

        return {"x": xh, "M": M}

    def deflection_at_gear_from_cache(self, d1: float, d2: float, d3: float, cache: Dict[str, np.ndarray]) -> Dict[str, float]:
        x = cache["x"]
        M = cache["M"]

        I = np.zeros_like(x, dtype=float)
        mask1 = (x >= 0.0) & (x <= self.c.x_support_A)
        mask2 = (x > self.c.x_support_A) & (x <= self.c.x_gear)
        mask3 = (x > self.c.x_gear) & (x <= self.c.x_support_B)

        I[mask1] = math.pi * d1**4 / 64.0
        I[mask2] = math.pi * d2**4 / 64.0
        I[mask3] = math.pi * d3**4 / 64.0

        curvature = M / (self.c.E_psi * I)
        int1 = self.cumtrapz_np(curvature, x)
        int2 = self.cumtrapz_np(int1, x)

        A = self.c.x_support_A
        B = self.c.x_support_B
        iA = int(np.argmin(np.abs(x - A)))
        iB = int(np.argmin(np.abs(x - B)))

        XA, XB = x[iA], x[iB]
        YA, YB = int2[iA], int2[iB]

        denom = XA - XB
        C1 = 0.0 if abs(denom) < 1e-12 else (YB - YA) / denom
        C2 = -YA - C1 * XA

        y = int2 + C1 * x + C2
        ig = int(np.argmin(np.abs(x - self.c.x_gear)))

        return {
            "x": x,
            "deflection_curve_in": y,
            "gear_deflection_in": float(abs(y[ig])),
            "max_abs_deflection_in": float(np.max(np.abs(y))),
        }

    def final_validate(self, x: np.ndarray) -> Dict[str, float]:
        d1, d2, d3 = [float(v) for v in x]
        return self.deflection_at_gear_from_cache(d1, d2, d3, self.final_defl_cache)

    def evaluate(self, x: np.ndarray) -> Tuple[float, float, float, Dict[str, float]]:
        d1, d2, d3 = [float(v) for v in x]

        # Objective
        weight = self.c.density_lb_per_in3 * (math.pi / 4.0) * (
            self.c.L1 * d1**2 + self.c.L2 * d2**2 + self.c.L3 * d3**2
        )

        d_std = np.array([self.nearest_standard(d1), self.nearest_standard(d2), self.nearest_standard(d3)])
        std_dev = np.sum(np.abs(np.array([d1, d2, d3]) - d_std))
        manufact_pen = self.c.manufacturability_weight_lb * std_dev / self.c.standard_step_in
        objective = weight + manufact_pen

        # Constraints using precomputed fatigue quantities
        req = self.precomputed_req
        d_required = req["required_diameters_in"]

        defl = self.deflection_at_gear_from_cache(d1, d2, d3, self.opt_defl_cache)

        g = np.array([
            d_required[0] - d1,
            d_required[1] - d2,
            d_required[2] - d3,
            self.c.min_step_diff_in - (d2 - d1),
            self.c.min_step_diff_in - (d2 - d3),
            defl["gear_deflection_in"] - self.c.max_gear_deflection_in,
        ], dtype=float)

        violation = float(np.sum(np.maximum(g, 0.0)))
        penalized = objective + 1e6 * violation

        details = {
            "d1_in": d1,
            "d2_in": d2,
            "d3_in": d3,
            "weight_lb": weight,
            "manufacturability_penalty_lb": manufact_pen,
            "objective_lb_equiv": objective,
            "gear_deflection_in": defl["gear_deflection_in"],
            "max_abs_deflection_in": defl["max_abs_deflection_in"],
            "required_d1_in": float(d_required[0]),
            "required_d2_in": float(d_required[1]),
            "required_d3_in": float(d_required[2]),
            "M_alt_s1_lb_in": float(req["M_alt_sections_lb_in"][0]),
            "M_alt_s2_lb_in": float(req["M_alt_sections_lb_in"][1]),
            "M_alt_s3_lb_in": float(req["M_alt_sections_lb_in"][2]),
            "M_mean_s1_lb_in": float(req["M_mean_sections_lb_in"][0]),
            "M_mean_s2_lb_in": float(req["M_mean_sections_lb_in"][1]),
            "M_mean_s3_lb_in": float(req["M_mean_sections_lb_in"][2]),
            "constraint_violation": violation,
            "nearest_std_d1_in": float(d_std[0]),
            "nearest_std_d2_in": float(d_std[1]),
            "nearest_std_d3_in": float(d_std[2]),
        }
        return objective, violation, penalized, details


# =========================================================
# SHMS OPTIMIZER
# =========================================================
class SHMSOptimizer:
    def __init__(
        self,
        problem: SteppedTransmissionShaftProblem,
        lb: np.ndarray,
        ub: np.ndarray,
        n_homes: int = 5,
        snails_per_home: int = 8,
        max_iter: int = 100,
        initial_c_frac: float = 0.22,
        min_c_frac: float = 0.01,
        seed: int | None = None,
    ):
        self.problem = problem
        self.lb = np.asarray(lb, dtype=float)
        self.ub = np.asarray(ub, dtype=float)
        self.dim = len(self.lb)
        self.n_homes = n_homes
        self.snails_per_home = snails_per_home
        self.max_iter = max_iter
        self.initial_c_frac = initial_c_frac
        self.min_c_frac = min_c_frac
        self.rng = np.random.default_rng(seed)

        self.home_pos = None
        self.snails = None
        self.fitness = None
        self.raw_objective = None
        self.violation = None
        self.details = None
        self.history = []

    def clip(self, X: np.ndarray) -> np.ndarray:
        return np.clip(X, self.lb, self.ub)

    def eval_pop(self, pop: np.ndarray):
        raw = np.zeros(len(pop))
        vio = np.zeros(len(pop))
        fit = np.zeros(len(pop))
        details = []
        for i, x in enumerate(pop):
            raw_i, vio_i, fit_i, det_i = self.problem.evaluate(x)
            raw[i] = raw_i
            vio[i] = vio_i
            fit[i] = fit_i
            details.append(det_i)
        return raw, vio, fit, details

    def initialize(self):
        span = self.ub - self.lb
        c0 = self.initial_c_frac * span

        self.home_pos = self.rng.uniform(self.lb, self.ub, size=(self.n_homes, self.dim))
        self.snails = np.zeros((self.n_homes, self.snails_per_home, self.dim), dtype=float)

        for h in range(self.n_homes):
            self.snails[h] = self.home_pos[h] + self.rng.uniform(-c0, c0, size=(self.snails_per_home, self.dim))
        self.snails = self.clip(self.snails)

        pop_flat = self.snails.reshape(-1, self.dim)
        raw, vio, fit, det = self.eval_pop(pop_flat)

        self.raw_objective = raw.reshape(self.n_homes, self.snails_per_home)
        self.violation = vio.reshape(self.n_homes, self.snails_per_home)
        self.fitness = fit.reshape(self.n_homes, self.snails_per_home)
        self.details = np.array(det, dtype=object).reshape(self.n_homes, self.snails_per_home)

        self.fit_hist = np.repeat(self.fitness[..., None], 3, axis=2)

    def roulette_select(self, scores: np.ndarray) -> int:
        scores = np.asarray(scores, dtype=float)
        scores = np.maximum(scores, 1e-12)
        probs = scores / np.sum(scores)
        return int(self.rng.choice(np.arange(len(scores)), p=probs))

    def optimize(self, verbose: bool = True):
        self.initialize()

        span = self.ub - self.lb
        best_details = None
        best_x = None
        best_fit = None
        best_raw = None
        best_vio = None

        for it in range(1, self.max_iter + 1):
            frac = (it - 1) / max(self.max_iter - 1, 1)
            c = (1 - frac) * self.initial_c_frac * span + frac * self.min_c_frac * span

            home_best_idx = np.argmin(self.fitness, axis=1)
            home_best = np.array([self.snails[h, home_best_idx[h]].copy() for h in range(self.n_homes)])

            flat_best = np.argmin(self.fitness)
            hgb, sgb = np.unravel_index(flat_best, self.fitness.shape)
            global_best = self.snails[hgb, sgb].copy()

            new_snails = self.snails.copy()
            new_raw = self.raw_objective.copy()
            new_vio = self.violation.copy()
            new_fit = self.fitness.copy()
            new_det = self.details.copy()

            # Safe fecundity update
            prev = self.fit_hist[:, :, 1]
            prev2 = self.fit_hist[:, :, 2]
            curr = self.fit_hist[:, :, 0]
            numer = np.abs(curr - prev)
            denom = np.abs(curr - prev2)

            fecundity = np.zeros_like(curr)
            valid = denom > 1e-12
            fecundity[valid] = numer[valid] / denom[valid]
            fecundity[~valid] = self.rng.uniform(0.0, 1.0, size=np.sum(~valid))
            fecundity = np.clip(fecundity, 0.0, 5.0)

            for h in range(self.n_homes):
                inv_fit = 1.0 / (self.fitness[h] + 1e-12)
                fecund_candidates = np.where(fecundity[h] >= np.median(fecundity[h]))[0]
                if len(fecund_candidates) == 0:
                    fecund_candidates = np.arange(self.snails_per_home)

                local_scores = inv_fit[fecund_candidates]
                chosen_local = fecund_candidates[self.roulette_select(local_scores)]
                fecund_snail = self.snails[h, chosen_local].copy()
                fecund_fit = self.fitness[h, chosen_local]

                for s in range(self.snails_per_home):
                    x = self.snails[h, s].copy()
                    f = self.fitness[h, s]
                    I_sh = fecundity[h, s]

                    LD = 1.0 / (abs(f - fecund_fit) + 1e-9)
                    LD *= max(I_sh, 0.05)
                    LD = min(LD, 10.0)

                    mate_cand = x + self.rng.random(self.dim) * (fecund_snail - x)
                    mate_cand += self.rng.normal(0.0, 0.03, size=self.dim) * LD * span
                    mate_cand = self.clip(mate_cand)

                    trail_cand = (
                        x
                        + self.rng.random(self.dim) * (home_best[h] - x)
                        + self.rng.random(self.dim) * (global_best - x)
                        + self.rng.normal(0.0, 1.0, size=self.dim) * (0.15 * c)
                    )
                    trail_cand = self.clip(trail_cand)

                    home_cand = self.home_pos[h] + self.rng.uniform(-c, c, size=self.dim)
                    home_cand = self.clip(home_cand)

                    candidates = np.vstack([x, mate_cand, trail_cand, home_cand])
                    raw_c, vio_c, fit_c, det_c = self.eval_pop(candidates)
                    best_idx = int(np.argmin(fit_c))

                    if fit_c[best_idx] < self.fitness[h, s]:
                        new_snails[h, s] = candidates[best_idx]
                        new_raw[h, s] = raw_c[best_idx]
                        new_vio[h, s] = vio_c[best_idx]
                        new_fit[h, s] = fit_c[best_idx]
                        new_det[h, s] = det_c[best_idx]

                best_s = int(np.argmin(new_fit[h]))
                self.home_pos[h] = 0.65 * self.home_pos[h] + 0.35 * new_snails[h, best_s]
                self.home_pos[h] = self.clip(self.home_pos[h])

            self.snails = new_snails
            self.raw_objective = new_raw
            self.violation = new_vio
            self.fitness = new_fit
            self.details = new_det

            self.fit_hist[:, :, 2] = self.fit_hist[:, :, 1]
            self.fit_hist[:, :, 1] = self.fit_hist[:, :, 0]
            self.fit_hist[:, :, 0] = self.fitness

            flat_best = np.argmin(self.fitness)
            hgb, sgb = np.unravel_index(flat_best, self.fitness.shape)
            best_x = self.snails[hgb, sgb].copy()
            best_fit = float(self.fitness[hgb, sgb])
            best_raw = float(self.raw_objective[hgb, sgb])
            best_vio = float(self.violation[hgb, sgb])
            best_details = self.details[hgb, sgb]

            self.history.append({
                "iteration": it,
                "best_penalized_fitness": best_fit,
                "best_objective_lb_equiv": best_raw,
                "best_violation": best_vio,
                "mean_penalized_fitness": float(np.mean(self.fitness)),
                "feasible_count": int(np.sum(self.violation <= 1e-12)),
                "c_mean": float(np.mean(c)),
                "best_d1_in": float(best_x[0]),
                "best_d2_in": float(best_x[1]),
                "best_d3_in": float(best_x[2]),
            })

            if verbose and (it == 1 or it % 10 == 0 or it == self.max_iter):
                print(
                    f"Iter {it:3d} | Obj = {best_raw:8.4f} | "
                    f"Violation = {best_vio:.6f} | "
                    f"d = [{best_x[0]:.4f}, {best_x[1]:.4f}, {best_x[2]:.4f}]"
                )

        hist_df = pd.DataFrame(self.history)
        return {
            "best_x": best_x,
            "best_fitness": best_fit,
            "best_objective": best_raw,
            "best_violation": best_vio,
            "best_details": best_details,
            "history": hist_df,
        }


# =========================================================
# PLOTS
# =========================================================
def plot_convergence(history: pd.DataFrame, outdir: Path):
    plt.figure(figsize=(8, 5))
    plt.plot(history["iteration"], history["best_objective_lb_equiv"], linewidth=2)
    plt.xlabel("Iteration")
    plt.ylabel("Best objective (lb-equivalent)")
    plt.title("SHMS convergence on stepped transmission shaft")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(outdir / "convergence.png", dpi=220)
    plt.close()

def plot_diameter_history(history: pd.DataFrame, outdir: Path):
    plt.figure(figsize=(8, 5))
    plt.plot(history["iteration"], history["best_d1_in"], label="d1")
    plt.plot(history["iteration"], history["best_d2_in"], label="d2")
    plt.plot(history["iteration"], history["best_d3_in"], label="d3")
    plt.xlabel("Iteration")
    plt.ylabel("Diameter (in)")
    plt.title("Best-so-far diameters during SHMS search")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(outdir / "diameter_history.png", dpi=220)
    plt.close()

def plot_best_design(best_details: Dict[str, float], outdir: Path):
    labels = ["d1", "d2", "d3"]
    vals = [best_details["d1_in"], best_details["d2_in"], best_details["d3_in"]]
    req = [best_details["required_d1_in"], best_details["required_d2_in"], best_details["required_d3_in"]]

    x = np.arange(len(labels))
    width = 0.35

    plt.figure(figsize=(7, 5))
    plt.bar(x - width / 2, vals, width, label="Optimized")
    plt.bar(x + width / 2, req, width, label="Required")
    plt.xticks(x, labels)
    plt.ylabel("Diameter (in)")
    plt.title("Optimized vs required section diameters")
    plt.legend()
    plt.grid(True, axis="y", alpha=0.3)
    plt.tight_layout()
    plt.savefig(outdir / "optimized_vs_required_diameters.png", dpi=220)
    plt.close()


# =========================================================
# RUN EXPERIMENT
# =========================================================
def run_experiment(
    n_runs: int = 4,
    max_iter: int = 80,
    seed0: int = 123,
    outdir: str = "shaft_shms_results1"
):
    out = Path(outdir)
    out.mkdir(parents=True, exist_ok=True)

    const = ShaftConstants()
    problem = SteppedTransmissionShaftProblem(const)

    lb = np.array([1.10, 1.20, 1.00], dtype=float)
    ub = np.array([2.50, 2.80, 2.40], dtype=float)

    all_runs = []
    best_result = None
    best_fit = float("inf")

    for run in range(n_runs):
        print(f"\n=== RUN {run+1}/{n_runs} ===")
        opt = SHMSOptimizer(
            problem=problem,
            lb=lb,
            ub=ub,
            n_homes=5,
            snails_per_home=8,
            max_iter=max_iter,
            seed=seed0 + run,
        )
        result = opt.optimize(verbose=False)
        bd = result["best_details"]

        # final high-resolution validation
        final_val = problem.final_validate(result["best_x"])

        all_runs.append({
            "run": run + 1,
            "best_weight_lb": bd["weight_lb"],
            "best_objective_lb_equiv": bd["objective_lb_equiv"],
            "best_violation": bd["constraint_violation"],
            "best_d1_in": bd["d1_in"],
            "best_d2_in": bd["d2_in"],
            "best_d3_in": bd["d3_in"],
            "gear_deflection_in_opt": bd["gear_deflection_in"],
            "gear_deflection_in_final": final_val["gear_deflection_in"],
            "required_d1_in": bd["required_d1_in"],
            "required_d2_in": bd["required_d2_in"],
            "required_d3_in": bd["required_d3_in"],
        })

        print(
            f"Best this run -> W={bd['weight_lb']:.4f} lb, "
            f"d=[{bd['d1_in']:.4f}, {bd['d2_in']:.4f}, {bd['d3_in']:.4f}], "
            f"defl(final)={final_val['gear_deflection_in']:.6f} in, "
            f"vio={bd['constraint_violation']:.6f}"
        )

        if result["best_fitness"] < best_fit:
            best_fit = result["best_fitness"]
            best_result = result

    # Final validation for overall best
    final_val_best = problem.final_validate(best_result["best_x"])
    best_result["best_details"]["gear_deflection_in_final"] = final_val_best["gear_deflection_in"]
    best_result["best_details"]["max_abs_deflection_in_final"] = final_val_best["max_abs_deflection_in"]

    stats_df = pd.DataFrame(all_runs)
    stats_df.to_csv(out / "multi_run_stats.csv", index=False)
    best_result["history"].to_csv(out / "best_run_history.csv", index=False)
    pd.DataFrame([best_result["best_details"]]).to_csv(out / "best_design_details.csv", index=False)

    plot_convergence(best_result["history"], out)
    plot_diameter_history(best_result["history"], out)
    plot_best_design(best_result["best_details"], out)

    print("\n" + "=" * 70)
    print("OVERALL BEST DESIGN")
    print("=" * 70)
    for k, v in best_result["best_details"].items():
        print(f"{k:30s}: {v}")

    print(f"\nResults saved in: {out.resolve()}")


if __name__ == "__main__":
    run_experiment(n_runs=3000, max_iter=100, seed0=123, outdir="shaft_shms_results1")

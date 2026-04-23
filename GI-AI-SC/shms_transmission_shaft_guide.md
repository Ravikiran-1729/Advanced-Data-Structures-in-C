
# Competition Guide: SHMS-Based Optimization of a Stepped Transmission Shaft

## 1) Best choice for the competition

I recommend choosing a **stepped transmission shaft** for a compact agro-processing drive such as a flour mill or juice extraction drive.

Why this is strategically strong:

- It is a **true mechanical sub-assembly** with real industrial importance.
- It naturally supports **functionality**, **manufacturability**, and a lightweight/maintainable design story.
- It produces a clean optimization problem with:
  - clear decision variables,
  - measurable constraints,
  - interpretable results,
  - strong plots,
  - easy extension to CAD or FEA.
- SHMS has already been reported in the literature on stepped-transmission-shaft optimization, so the pairing is academically credible.

## 2) What is the actual engineering problem?

A transmission shaft carries torque from one rotating element to another. If its diameter is too small, it may fail under:

- fluctuating bending,
- fluctuating torsion,
- fatigue,
- excess deflection,
- poor gear meshing due to shaft sag.

If its diameter is too large, it becomes:

- heavier,
- more expensive,
- less material-efficient,
- harder to manufacture economically.

So the design target is:

**Minimize shaft weight while satisfying fatigue strength, shoulder geometry, and deflection constraints.**

## 3) Why stepped shaft and not the other options?

This is the strongest overall choice because:

- **Hooks** are excellent, but stress modeling becomes more assumption-heavy.
- **Screw jack / springs / bearings / clutch / gears** are also strong, but they need either contact mechanics, standard design catalog values, or more complicated geometry assumptions.
- **Transmission shaft** gives the best balance of:
  - analytical tractability,
  - SHMS suitability,
  - visual clarity,
  - engineering depth,
  - competition presentation value.

## 4) Design variables

We optimize three diameters:

- `d1` = diameter of pulley-side section
- `d2` = diameter of central section
- `d3` = diameter of gear-side section

These are enough to make the problem meaningful because the shaft is stepped into three sections with different stress demands.

## 5) Why these variables matter

### d1
This governs the overhung pulley section. That region experiences bending from pulley-side loads and belt forces.

### d2
This is the central section and usually becomes the most critical region because it carries substantial bending and is between support and gear loading.

### d3
This governs the gear-side section and affects stiffness and local fatigue resistance near the gear.

## 6) Fixed assumptions used in the code

The competition explicitly allows reasonable assumptions. The code assumes:

- material: AISI 1040 cold-drawn steel
- three shaft sections of lengths 6 in, 10 in, 10 in
- support positions at 6 in and 26 in
- pulley at 0 in
- gear at 16 in
- maximum torque = 3500 lb-in
- minimum torque = 875 lb-in
- gear pitch diameter = 10 in
- pulley diameter = 20 in
- gear pressure angle = 20°
- belt friction coefficient = 0.31
- wrap angle = 210°
- allowable gear-point deflection = 0.005 in

These assumptions make the model realistic and competition-ready.

## 7) Objective function

The main objective is shaft weight:

\[
W = \gamma \frac{\pi}{4}\left(L_1 d_1^2 + L_2 d_2^2 + L_3 d_3^2\right)
\]

where:

- \(\gamma\) = material density
- \(L_1, L_2, L_3\) = section lengths
- \(d_1, d_2, d_3\) = section diameters

### Manufacturability add-on
A small soft penalty is added if the optimized diameters drift away from standard shop-friendly increments such as 0.125 in. That improves practical manufacturability.

## 8) Constraints used

### Fatigue constraints
Each diameter must be at least the minimum required diameter from the **Modified Goodman** criterion under combined bending and torsion.

### Shoulder constraints
Adjacent diameters must maintain a minimum difference:

- \(d_2 - d_1 \ge 0.0787\)
- \(d_2 - d_3 \ge 0.0787\)

This helps preserve the stepped geometry.

### Deflection constraint
The deflection at the gear location must satisfy:

\[
|y_{gear}| \le 0.005 \text{ in}
\]

This is crucial because excessive gear-point deflection harms meshing and power transmission.

## 9) Why Modified Goodman is used

Transmission shafts usually operate under **fluctuating stresses**. That means a static stress check is not enough. Modified Goodman is appropriate because it combines:

- alternating stress,
- mean stress,
- fatigue safety.

This makes the problem much more credible than a simple static-only design.

## 10) How SHMS is used

The code uses a practical SHMS workflow based on the published concepts:

1. Initialize several **homes**
2. Create multiple **snails** around each home
3. Evaluate the objective and constraints
4. Compute **fecundity index**
5. Use **roulette wheel** selection to choose promising snails
6. Apply **mating / love-dart** movement
7. Apply **trail-following / homing** movement
8. Keep better solutions
9. Gradually shrink the local search region
10. Stop after the iteration limit

## 11) Why SHMS fits this problem

SHMS is a good match because the shaft problem is:

- nonlinear,
- constrained,
- continuous,
- not easy to solve analytically in one shot.

SHMS balances:

- local search around good homes,
- guided movement toward strong candidates,
- mating-based exploration,
- progressive convergence.

## 12) What makes this competition submission stronger

To make it stronger than a plain optimization exercise, this solution adds:

- a real shaft-use scenario,
- fatigue-based sizing,
- beam-deflection calculation,
- manufacturability soft penalty,
- repeated runs,
- convergence plots,
- design-history plots,
- run-to-run robustness statistics.

## 13) Files produced by the code

When you run the script it creates a results folder containing:

- `best_run_history.csv`
- `multi_run_stats.csv`
- `best_design_details.csv`
- `summary.txt`
- `convergence.png`
- `diameter_history.png`
- `optimized_vs_required_diameters.png`
- `multi_run_weight_distribution.png`
- `d2_vs_weight_scatter.png`

## 14) How to present this in the competition PDF

Use this structure:

### A. Problem definition
State that the goal is to design a stepped transmission shaft for a compact agro-processing drive.

### B. Assumptions
List all load, geometry, and material assumptions clearly.

### C. Variables and bounds
Present:

- \(d_1 \in [1.10, 2.50]\)
- \(d_2 \in [1.20, 2.80]\)
- \(d_3 \in [1.00, 2.40]\)

### D. Objective function
Weight minimization with a small manufacturability penalty.

### E. Constraints
Fatigue diameter constraints, shoulder constraints, and gear-point deflection.

### F. Optimization algorithm
Explain SHMS, its biological inspiration, and your implementation steps.

### G. Algorithm modifications
Mention:
- static penalty for constraints,
- manufacturability penalty,
- numerical deflection calculation,
- repeated-run analysis.

### H. Results
Show:
- best design,
- best weight,
- best deflection,
- required vs optimized diameters,
- convergence plot.

### I. Importance of output
Explain that the final solution gives:
- lower material use,
- adequate fatigue safety,
- controlled deflection,
- improved practical manufacturability,
- easier maintenance through reduced mass.

## 15) Very important honesty note

This solution is built as a **competition-grade engineering model** using published SHMS concepts plus explicit, defendable assumptions. It is not presented as the exact official author code from any paper. That is actually a strength in a competition setting, because you can clearly defend what you assumed and why.

## 16) How to run

```bash
pip install numpy pandas matplotlib
python shms_transmission_shaft_competition.py
```

## 17) Best next upgrade if you want to push this toward winning level

Add one of these:

- CAD rendering of the final stepped shaft
- ANSYS validation of the final best design
- comparison with PSO or GA
- discrete standard-diameter optimization after continuous SHMS
- cost model in addition to weight

That would make the submission even stronger.

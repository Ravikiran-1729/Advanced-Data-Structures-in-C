# SHMS-Based Optimization of Stepped Transmission Shaft

---

## 1. Problem Definition

The objective of this work is to design a **stepped transmission shaft** used in an agro-processing system such that:

**Minimize shaft weight**

while satisfying:

- Fatigue strength constraints under fluctuating loading  
- Geometric constraints for manufacturability  
- Deflection constraints to ensure proper mechanical functioning  

The shaft transmits torque between rotating components such as pulleys and gears. Improper design may lead to:

- Fatigue failure  
- Excessive bending  
- Misalignment of gears  
- Increased material cost  

Thus, the problem is formulated as a **constrained nonlinear optimization problem**.

---

## 2. Assumptions

- Shaft material is homogeneous and isotropic (AISI 1040 steel)
- Euler-Bernoulli beam theory is valid
- Loads are applied at discrete points
- Torque varies between known maximum and minimum values
- Stress concentration factors are constant
- Temperature effects are neglected
- Shaft is simply supported at two bearings
- Dynamic effects are approximated using fluctuating loads

---

## 3. Variable Definitions and Bounds

| Variable | Description | Bounds |
|--------|------------|--------|
| d1 | Pulley-side diameter | 1.10 – 2.50 in |
| d2 | Center diameter | 1.20 – 2.80 in |
| d3 | Gear-side diameter | 1.00 – 2.40 in |

---

## 4. Objective Function

\[
W = \rho \frac{\pi}{4}(L_1 d_1^2 + L_2 d_2^2 + L_3 d_3^2)
\]

Where:
- ρ = 0.2834 lb/in³  
- L1 = 6 in, L2 = 10 in, L3 = 10 in  

Final objective:

\[
\text{Minimize } f = W + P_m
\]

Where \(P_m\) is manufacturability penalty.

---

## 5. Constraints

### 5.1 Fatigue Constraint (Modified Goodman)

\[
d_i \ge d_{req,i}
\]

\[
d_i = \left[\frac{32N}{\pi}
\left(
\frac{\sqrt{(K_fM_a)^2+\frac{3}{4}(K_{fs}T_a)^2}}{S_e}
+
\frac{\sqrt{(K_{fm}M_m)^2+\frac{3}{4}(K_{fsm}T_m)^2}}{S_{ut}}
\right)\right]^{1/3}
\]

---

### 5.2 Deflection Constraint

\[
\delta_{gear} \le 0.005 \text{ in}
\]

---

### 5.3 Geometric Constraints

\[
d_2 - d_1 \ge 0.0787
\]

\[
d_2 - d_3 \ge 0.0787
\]

---

## 6. Load Formulation

### Torque:
- Tmax = 3500 lb-in  
- Tmin = 875 lb-in  

\[
T_a = \frac{T_{max}-T_{min}}{2}
\]

\[
T_m = \frac{T_{max}+T_{min}}{2}
\]

---

### Gear Forces:
\[
F_t = \frac{2T}{D}
\]

\[
F_r = F_t \tan \phi
\]

---

### Pulley Forces:
\[
F = \frac{T}{r}
\]

---

## 7. SHMS Algorithm

Steps:

1. Initialize homes and snails  
2. Evaluate fitness  
3. Compute fecundity index  
4. Selection (roulette wheel)  
5. Mating mechanism  
6. Trail-following  
7. Update population  
8. Repeat until convergence  

---

## 8. Algorithm Modifications

- Constraint handling using penalty function  
- Precomputation of load parameters  
- Beam-based deflection calculation  
- Manufacturability penalty  
- Multi-run analysis  

---

## 9. Flowchart
Start
↓
Initialize homes & snails
↓
Evaluate fitness
↓
Compute fecundity
↓
Selection
↓
Mating + movement
↓
Update population
↓
Check convergence
↓
End


---

## 10. Results

### Optimized Design

| Parameter | Value |
|----------|------|
| d1 | 1.5329 in |
| d2 | 1.8252 in |
| d3 | 1.7463 in |
| Weight | 17.34 lb |
| Deflection | 0.004985 in |

---

### Observations

- d2 > d3 > d1  
- Deflection constraint is active  
- All constraints satisfied  

---

## 11. Convergence Analysis

- Objective decreases steadily  
- Converges to stable optimum  
- SHMS shows efficient search  

---

## 12. Additional Plots

- Convergence plot  
- Diameter evolution  
- Required vs optimized diameter  
- Weight distribution  

---

## 13. Programming Language

Python  
Libraries: NumPy, Pandas, Matplotlib  

---

## 14. Code Runtime

Approximately:

10–30 seconds depending on iterations and runs  

---

## 15. Importance of Output

The optimized shaft design:

- Reduces material usage  
- Ensures fatigue safety  
- Maintains stiffness  
- Improves manufacturability  
- Enhances reliability  

---

## 16. Conclusion

The SHMS algorithm successfully optimized the stepped shaft design by balancing weight and safety constraints. The final design is lightweight, safe, and manufacturable, demonstrating the effectiveness of SHMS in mechanical optimization problems.

---

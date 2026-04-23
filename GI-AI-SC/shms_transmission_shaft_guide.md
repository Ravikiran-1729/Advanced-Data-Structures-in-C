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

<img width="387" height="76" alt="image" src="https://github.com/user-attachments/assets/fd7c4483-ed07-4fad-9945-b10906854a5f" />

Where:
- ρ = 0.2834 lb/in³  
- L1 = 6 in, L2 = 10 in, L3 = 10 in  

Final objective:

<img width="288" height="69" alt="image" src="https://github.com/user-attachments/assets/bc3493cd-f944-48c5-a0d8-b64b49f6f3a1" />


Where \(P_m\) is manufacturability penalty.

---

## 5. Constraints

### 5.1 Fatigue Constraint (Modified Goodman)
<img width="890" height="188" alt="image" src="https://github.com/user-attachments/assets/40abf01f-3f6b-4799-9dee-a4efbac49cd0" />

---

### 5.2 Deflection Constraint

<img width="249" height="70" alt="image" src="https://github.com/user-attachments/assets/b18a631e-60fd-47b2-87e4-ce6829b760b7" />

---

### 5.3 Geometric Constraints

<img width="277" height="108" alt="image" src="https://github.com/user-attachments/assets/843f86e6-71d1-4151-b193-cd50c5a6e3d7" />


---

## 6. Load Formulation

### Torque:
- Tmax = 3500 lb-in  
- Tmin = 875 lb-in  

<img width="324" height="152" alt="image" src="https://github.com/user-attachments/assets/40437e48-1b51-4887-ade3-5848310f96de" />

---

### Gear Forces:

<img width="278" height="137" alt="image" src="https://github.com/user-attachments/assets/cd5810de-a1dd-46fe-9f30-6aaf7af3f383" />


---

### Pulley Forces:

<img width="175" height="94" alt="image" src="https://github.com/user-attachments/assets/a8345f9d-f225-44b6-8e5b-408962413650" />

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

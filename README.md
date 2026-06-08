# 📐 Numerical Analysis — End-of-Term Project

> **Yıldız Technical University**  
> Interactive terminal application implementing 10 fundamental numerical methods in ANSI C.

---

## 📋 Overview

This project is a single-file ANSI C program that provides an interactive menu-driven interface for solving a wide range of numerical analysis problems. It features a custom **recursive descent expression parser** that allows the user to enter arbitrary mathematical functions (e.g. `sin(x^2) + ln(x)`) at runtime — no hardcoded test functions.

---

## ✨ Features

### 🔢 Root-Finding Methods
| # | Method | Description |
|---|--------|-------------|
| 1 | **Bisection** | Brackets a root by halving the interval; guaranteed convergence |
| 2 | **Regula-Falsi** | Linear interpolation between bracket endpoints |
| 3 | **Newton-Raphson** | Quadratic convergence using numerical derivative |

### 🧮 Linear Systems & Matrix Operations
| # | Method | Description |
|---|--------|-------------|
| 4 | **Matrix Inverse (NxN)** | Gauss-Jordan elimination with full pivoting |
| 5 | **LU Decomposition** | Doolittle LU factorization for solving Ax = b |
| 6 | **Gauss-Seidel** | Iterative solver; works best with diagonally dominant matrices |

### 📈 Numerical Calculus
| # | Method | Description |
|---|--------|-------------|
| 7 | **Numerical Derivative** | Forward, backward, and central difference formulas |
| 8 | **Simpson's Rule** | 1/3 and 3/8 variants for numerical integration |
| 9 | **Trapezoidal Rule** | Composite trapezoidal integration |

### 📊 Interpolation
| # | Method | Description |
|---|--------|-------------|
| 10 | **Gregory-Newton** | Divided-difference interpolation with full difference table output |

---

## 🧠 Expression Parser

One of the core components is a hand-written **recursive descent parser** that converts a string like `x^3 - 2*x + sin(pi*x)` into an abstract syntax tree (AST) evaluated at any point `x`.

**Supported syntax:**
- Operators: `+`, `-`, `*`, `/`, `^` (power)
- Functions: `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `exp`, `ln`, `log`, `log_N`, `sqrt`, `abs`
- Constants: `pi`, `e`
- Variable: `x`

---

## 🚀 Build & Run

```bash
# Compile (GCC)
gcc -ansi -pedantic -Wall -o na_project na_project.c -lm

# Run
./na_project
```

> Requires a C89/C90 compatible compiler. Tested with GCC on Linux/Windows (MinGW).

---

## 📁 Repository Structure

```
YTU-na_finish_project/
├── na_project.c      # Full source code (single-file, ANSI C)
└── README.md
```

---

## ⚙️ Technical Details

- **Standard:** ANSI C (C89/C90) — `-ansi -pedantic` compliant
- **No global variables** — all state passed via parameters or stack
- **Dynamic memory allocation** — matrices and AST nodes managed with `malloc`/`free`
- **Error handling** — divergence detection, invalid input guards, NaN/Inf checks via `IS_BAD` macro

---

## 🎓 Course Information

| Field | Detail |
|-------|--------|
| Course | Numerical Analysis |
| University | Yıldız Technical University |
| Department | Computer Engineering |
| Term | 2025–2026 Spring |

---

## 📄 License

This project was developed for academic purposes. Feel free to reference or adapt it with proper attribution.

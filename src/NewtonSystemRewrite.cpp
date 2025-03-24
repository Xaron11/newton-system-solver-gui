#include "../include/NewtonSystem.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// Helper functions to break down complex operations
void initializeIteration(int n1, std::vector<int> &r) {
  for (int i = 1; i <= n1; i++) {
    r[i] = 0;
  }
}

void calculateDerivativesAndFunction(int k, int n, const Vector &x,
                                     Vector &dfatx, std::vector<double> &a,
                                     int n1, FunctionTypeC f,
                                     DerivativeTypeC df) {

  df(k, n, &x[0], &dfatx[0]);

  for (int i = 1; i <= n; i++) {
    a[i] = dfatx[i];
  }

  double s = -f(k, n, &x[0]);
  for (int i = 1; i <= n; i++) {
    s += dfatx[i] * x[i];
  }
  a[n1] = s;
}

void updateBVector(int n, const std::vector<double> &a, std::vector<double> &b,
                   const std::vector<int> &r) {
  for (int i = 1; i <= n; i++) {
    int rh = r[i];
    if (rh != 0) {
      b[rh] = a[i];
    }
  }
}

bool findPivot(int n1, std::vector<double> &a, const std::vector<int> &r,
               int kh, int p, std::vector<double> &b,
               const std::vector<double> &x1, int &l, double &max, int &jh,
               int &lh) {
  l = 0;
  max = 0;

  for (int j = 1; j <= n1; j++) {
    if (r[j] == 0) {
      double s = a[j];
      l++;
      int q = l;

      for (int i = 1; i <= kh; i++) {
        s = s - b[i] * x1[q];
        q = q + p;
      }

      a[l] = s;
      s = std::abs(s);

      if (j < n1 && s > max) {
        max = s;
        jh = j;
        lh = l;
      }
    }
  }

  return max != 0;
}

void performPivoting(int p, double &max, const int lh, std::vector<double> &a,
                     std::vector<int> &r, int jh, int k, int kh,
                     std::vector<double> &x1, int &jh_out) {

  max = 1 / a[lh];
  r[jh] = k;

  for (int i = 1; i <= p; i++) {
    a[i] = max * a[i];
  }

  jh_out = 0;
  int q = 0;

  for (int j = 1; j <= kh; j++) {
    double s = x1[q + lh];
    for (int i = 1; i <= p; i++) {
      if (i != lh) {
        jh_out++;
        x1[jh_out] = x1[q + i] - s * a[i];
      }
    }
    q = q + p;
  }

  for (int i = 1; i <= p; i++) {
    if (i != lh) {
      jh_out++;
      x1[jh_out] = a[i];
    }
  }
}

void reorderSolution(int n, std::vector<double> &x1, std::vector<int> &r) {
  for (int k = 1; k <= n; k++) {
    int rh = r[k];
    if (rh != k) {
      double s = x1[k];
      x1[k] = x1[rh];
      int i = r[rh];

      while (i != k) {
        x1[rh] = x1[i];
        r[rh] = rh;
        rh = i;
        i = r[rh];
      }

      x1[rh] = s;
      r[rh] = rh;
    }
  }
}

bool checkConvergence(int n, const Vector &x, const std::vector<double> &x1,
                      double eps) {
  for (int i = 1; i <= n; i++) {
    double max_val = std::abs(x[i]);
    double s = std::abs(x1[i]);

    if (max_val < s) {
      max_val = s;
    }

    if (max_val > 0 && std::abs(x[i] - x1[i]) / max_val >= eps) {
      return false;
    }
  }

  return true;
}

void updateSolution(int n, Vector &x, const std::vector<double> &x1) {
  for (int i = 1; i <= n; i++) {
    x[i] = x1[i];
  }
}

/**
 * Solves a system of n nonlinear equations of the form
 * f[i](x[1],x[2],...,x[n])=0 (i=1,2,...,n) using Newton's method.
 *
 * @param n Number of equations
 * @param x Initial approximations to solution components (changed on exit)
 * @param f Function that calculates the value of function f[i]
 * @param df Function that calculates the derivatives df[i]/dx[j] (j=1,2,...,n)
 * @param mit Maximum number of iterations in Newton's method
 * @param eps Relative accuracy of the solution
 * @param it Number of iterations performed (output)
 * @param st Status code (output):
 *           0 = success,
 *           1 = invalid input (n<1 or mit<1),
 *           2 = singular matrix,
 *           3 = iterations exceeded
 */
void NewtonSystem(int n, Vector &x, FunctionTypeC f, DerivativeTypeC df,
                  int mit, double eps, int &it, int &st) {
  // Check for valid inputs
  if (n < 1 || mit < 1) {
    st = 1;
    return;
  }

  // Initialize variables
  st = 0;
  it = 0;
  int n1 = n + 1;

  Vector dfatx(n + 1);
  std::vector<double> a(n1 + 1);
  std::vector<double> b(n1 + 1);
  std::vector<int> r(n1 + 1, 0);
  std::vector<double> x1(((n + 2) * (n + 2)) / 4 + 1);

  bool converged = false;

  // Main iteration loop
  while (!converged && st == 0) {
    it++;

    // Check if exceeded maximum iterations
    if (it > mit) {
      st = 3;
      it--;
      break;
    }

    int p = n1;
    initializeIteration(n1, r);

    int k = 0;
    bool singular = false;

    // Process each equation in the system
    while (k < n && st != 2) {
      k++;

      calculateDerivativesAndFunction(k, n, x, dfatx, a, n1, f, df);
      updateBVector(n, a, b, r);

      int kh = k - 1;
      int l = 0;
      double max = 0;
      int jh = 0, lh = 0;

      // Find pivot element
      if (!findPivot(n1, a, r, kh, p, b, x1, l, max, jh, lh)) {
        st = 2;
        singular = true;
        continue;
      }

      // Perform pivoting
      int jh_out = 0;
      performPivoting(p, max, lh, a, r, jh, k, kh, x1, jh_out);
      p--;
    }

    // If matrix is not singular, check for convergence
    if (st == 0) {
      reorderSolution(n, x1, r);
      converged = checkConvergence(n, x, x1, eps);
      updateSolution(n, x, x1);
    }
  }
}
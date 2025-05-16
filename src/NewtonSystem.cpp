#include "../include/NewtonSystem.h"

#include <cmath>
#include <vector>

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
namespace NStandard {
void NewtonSystem(int n, Vector &x, FunctionTypeC f, DerivativeTypeC df,
                  int mit, Val eps, int &it, int &st) {
  if (n < 1 || mit < 1) {
    st = 1;
    return;
  }

  st = 0;
  it = 0;
  int n1 = n + 1;

  Vector dfatx(n + 1);
  std::vector<Val> a(n1 + 1);
  std::vector<Val> b(n1 + 1);
  std::vector<int> r(n1 + 1, 0);
  std::vector<Val> x1(((n + 2) * (n + 2)) / 4 + 1);

  bool cond = false;
  do {
    it++;
    if (it > mit) {
      st = 3;
      it--;
      break;
    }

    int p = n1;
    for (int i = 1; i <= n1; i++) {
      r[i] = 0;
    }

    int k = 0;
    do {
      k++;
      df(k, n, &x[0], &dfatx[0]);

      for (int i = 1; i <= n; i++) {
        a[i] = dfatx[i];
      }

      Val s = -f(k, n, &x[0]);
      for (int i = 1; i <= n; i++) {
        s += dfatx[i] * x[i];
      }
      a[n1] = s;

      for (int i = 1; i <= n; i++) {
        int rh = r[i];
        if (rh != 0) {
          b[rh] = a[i];
        }
      }

      int kh = k - 1;
      int l = 0;
      Val max = 0;
      int jh = 0, lh = 0;

      for (int j = 1; j <= n1; j++) {
        if (r[j] == 0) {
          s = a[j];
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

      if (max == 0) {
        st = 2;
        break;
      }

      max = 1 / a[lh];
      r[jh] = k;
      for (int i = 1; i <= p; i++) {
        a[i] = max * a[i];
      }

      jh = 0;
      int q = 0;
      for (int j = 1; j <= kh; j++) {
        s = x1[q + lh];
        for (int i = 1; i <= p; i++) {
          if (i != lh) {
            jh++;
            x1[jh] = x1[q + i] - s * a[i];
          }
        }
        q = q + p;
      }

      for (int i = 1; i <= p; i++) {
        if (i != lh) {
          jh++;
          x1[jh] = a[i];
        }
      }
      p = p - 1;
    } while (k < n && st != 2);

    if (st == 0) {
      for (int k = 1; k <= n; k++) {
        int rh = r[k];
        if (rh != k) {
          Val s = x1[k];
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

      cond = true;
      for (int i = 1; i <= n; i++) {
        Val max = std::abs(x[i]);
        Val s = std::abs(x1[i]);
        if (max < s) {
          max = s;
        }
        if (max != 0 && std::abs(x[i] - x1[i]) / max >= eps) {
          cond = false;
          break;
        }
      }

      for (int i = 1; i <= n; i++) {
        x[i] = x1[i];
      }
    }
  } while (st == 0 && !cond);
}
}  // namespace NStandard
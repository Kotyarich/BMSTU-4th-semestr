#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

inline double f(double x) {
  return x / (1 + 2 * x);
//  return x * x;
}

inline double dirF(double x) {
  return  1 / (1 + 2 * x) / ( 1 + 2 * x);
}

std::vector<double> getOneSideDiffs(const std::vector<double> &x, const std::vector<double> &y) {
  std::vector<double> result(x.size() - 1);

  for (int i = 0; i < x.size() - 1; i++) {
    result[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
  }

  return result;
}

std::vector<double> getCentralDiffs(const std::vector<double> &x, const std::vector<double> &y) {
  std::vector<double> result(x.size() - 1);

  for (int i = 1; i < x.size() - 1; i++) {
    result[i] = (y[i + 1] - y[i - 1]) / (x[i + 1] - x[i - 1]);
  }

  return result;
}

double leftBound(const std::vector<double> &x, const std::vector<double> &y) {
  return (-3 * y[0] + 4 * y[1] - y[2]) / (x[2] - x[0]);
}

double rightBound(const std::vector<double> &x, const std::vector<double> &y) {
  const auto size = y.size();
  return (3 * y[size - 1] - 4 * y[size - 2] + y[size - 3]) / (x[size - 1] - x[size - 3]);
}

std::vector<double> getRungeDerivs(const std::vector<double> &x, const std::vector<double> &y) {
  std::vector<double> result(x.size() - 2);

  for (int i = 0; i < x.size() - 2; i++) {
    double sideDif = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
    double secondSideDif = (y[i + 2] - y[i]) / (x[i + 2] - x[i]);

    result[i] = sideDif + (sideDif - secondSideDif);
  }

  return result;
}

std::vector<double> getAligmentVars(const std::vector<double> &x, const std::vector<double> &y) {
  std::vector<double> eta(x.size());
  std::vector<double> ksi(x.size());

  for (int i = 0; i < x.size() - 1; i++) {
    eta[i] = 1 / y[i];
    ksi[i] = 1 / x[i];
  }

  std::vector<double> result(x.size() - 1);

  for (int i = 0; i < x.size() - 1; i++) {
    result[i] = (eta[i + 1] - eta[i]) / (ksi[i + 1] - ksi[i]);
  }

  for (int i = 0; i < result.size(); i++) {
    result[i] *= y[i] * y[i] / x[i] / x[i];
  }

  return result;
}

int main() {
  double left = 1;
  double right = 10;
  double h = 1;

  std::vector<double> x(0);
  std::vector<double> y(0);

  for (double i = left; i <= right; i += h) {
    x.push_back(i);
    y.push_back(f(i));
  }

  auto one_side_diffs = getOneSideDiffs(x, y);
  auto central_diffs = getCentralDiffs(x, y);
  auto runge_derivs = getRungeDerivs(x, y);
  auto aligment_derivs = getAligmentVars(x, y);

  std::cout.width(9);
  std::cout <<  "x|";
  std::cout.width(9);
  std::cout <<  "y|";
  std::cout.width(9);
  std::cout <<  "1|";
  std::cout.width(9);
  std::cout <<  "2|";
  std::cout.width(9);
  std::cout <<  "3|";
  std::cout.width(9);
  std::cout <<  "4|";
  std::cout.width(9);
  std::cout <<  "5|";
  std::cout.width(8);
  std::cout <<  "6" << std::endl;
  for (int i = 0; i < 72; i++) {
    std::cout << '-';
  }
  std::cout << std::endl;

  for (int i = 0; i < x.size(); i++) {
    for (int j = 0; j < 8; j++) {
      switch (j) {
        case 0:
          std::cout.width(8);
          std::cout << std::fixed << std::setprecision(3) << x[i];
          break;
        case 1:
          std::cout << '|';
          std::cout.width(8);
          std::cout << std::fixed << std::setprecision(3) << y[i];
          break;
        case 2:
          std::cout << '|';
          std::cout.width(8);
          if (i < x.size() - 1) {
            std::cout << std::fixed << std::setprecision(3) << one_side_diffs[i];
          } else {
            std::cout << '-';
          }
          std::cout << std::flush;
          break;
        case 3:
          std::cout << '|';
          std::cout.width(8);
          if (i > 0 && i < x.size() - 1) {
            std::cout << std::fixed << std::setprecision(3) << central_diffs[i];
          } else {
            std::cout << '-';
          }
          break;
        case 4:
          std::cout << '|';
          std::cout.width(8);
          if (i == 0) {
            std::cout << std::fixed << std::setprecision(3) << leftBound(x, y);
          } else if (i == x.size() - 1) {
            std::cout << std::fixed << std::setprecision(3) << rightBound(x, y);
          } else {
            std::cout << '-';
          }
          break;
        case 5:
          std::cout << '|';
          std::cout.width(8);
          if (i < x.size() - 2) {
            std::cout << std::fixed << std::setprecision(3) << runge_derivs[i];
          } else {
            std::cout << '-';
          }
          break;
        case 6:
          std::cout << '|';
          std::cout.width(8);
          if (i < x.size() - 1) {
            std::cout << std::fixed << std::setprecision(3) << aligment_derivs[i];
          } else {
            std::cout << '-';
          }
          std::cout << '|';
          std::cout.width(8);
          std::cout << std::fixed << std::setprecision(3) << dirF(x[i]);
          std::cout << std::endl;
        default:break;
      }
    }
  }

  return 0;
}
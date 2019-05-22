#include <vector>
#include <math.h>
#include <iostream>
#include <functional>
#include <iomanip>
#include <chrono>

std::vector<std::vector<double>> Q = {{2000, 4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000},
                                      {1, 1, 1, 1.001, 1.0025, 1.0198, 1.0895, 1.2827, 1.6973, 2.4616, 3.6552, 5.3749, 7.6838},
                                      {4, 4, 4.1598, 4.3006, 4.4392, 4.5661, 4.6817, 4.7932, 4.9099, 5.0511, 5.2354, 5.4841, 5.8181},
                                      {5.5, 5.5, 5.5116, 5.9790, 6.4749, 6.9590, 7.4145, 7.8370, 8.2289, 8.5970, 8.9509, 9.3018, 9.6621},
                                      {11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},
                                      {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15}};
const double Z[]               =      { 0, 1, 2, 3, 4 };
const double Z2[]              =      { 0, 1, 4, 9, 16 };
const double E[]               =      { 12.13, 20.98, 31.0, 45.0};
const double EPS               =      1e-7;
const double MIN_P             =      0;
const double MAX_P             =      20;
bool first = true;


int find_nearest_point(std::vector<std::vector<double>> &input_data, double x) {
  int data_len = static_cast<int>(input_data.size());

  int lower = 0;
  int upper = data_len;

  while (std::abs(upper - lower) > 1) {
    if (input_data[(lower + upper) / 2][0] > x)
      upper = (lower + upper) / 2;
    else if (input_data[(lower + upper) /2][0] <= x)
      lower = (lower + upper) / 2;
  }
  int result;
  if (std::abs(x - upper) > abs(x - lower))
    result = lower;
  else
    result = upper;

  return result;
}


std::vector<std::vector<double>> find_div_diff(std::vector<std::vector<double>> &input_data, int lower_edge, int upped_edge) {
  int n = upped_edge - lower_edge;
  std::vector<std::vector<double>> div_diff(static_cast<unsigned long>(n + 1));

  for (int i = lower_edge; i < upped_edge + 1; i++) {
    div_diff[0].push_back(input_data[i][1]);
  }

  for (int i = 1; i < n + 1; i++) {
    for (int j = 0; j < n + 1 - i; j++) {
      double tmp = input_data[i + j + lower_edge][0] - input_data[j + lower_edge][0];
      if (tmp == 0) {
        div_diff[i].push_back(0.0);
        continue;
      }
      div_diff[i].push_back((div_diff[i - 1][j + 1] - div_diff[i - 1][j]) / (tmp));
    }
  }

  return div_diff;
}


double find_value(double x, std::vector<std::vector<double>> &div_diff, std::vector<std::vector<double>> &input_data) {
  double result = 0;
  int n = static_cast<int>(div_diff.size());

  for (int i = 0; i < n; i++) {
    double tmp = 1;
    for (int j = 0; j < i; j++) {
      tmp *= x - input_data[j][0];
    }
    tmp *= div_diff[i][0];

    result += tmp;
  }

  return result;
}

double interpolate(std::vector<std::vector<double>> &data, int n, double x) {
  if (n + 1 > data.size())
    n = static_cast<int>(data.size() - 1);

  auto nearest_point = find_nearest_point(data, x);
  int data_len = static_cast<int>(data.size());
  int upped_edge = nearest_point, lower_edge = nearest_point;

  int amount_of_points = n;

  if (amount_of_points > data_len)
    amount_of_points = data_len;

  while (amount_of_points > 0) {
    if (upped_edge < data_len - 1) {
      upped_edge += 1;
      amount_of_points -= 1;
    }
    if (amount_of_points > 0 and lower_edge > 0) {
      lower_edge -= 1;
      amount_of_points -= 1;
    }
  }

  auto div_diff = find_div_diff(data, lower_edge, upped_edge);

  std::vector<std::vector<double>> data_slice;
  for (int i = lower_edge; i < upped_edge; i++) {
    data_slice.push_back(data[i]);
  }
  double value = find_value(x, div_diff, data_slice);

  return value;
}

double getGamma(double gamma, double t, std::vector<double> &x) {
  gamma /= 2;
  auto result = std::exp(x[0]) / (1 + gamma);
  for (int i = 1; i < 6; i++) {
    result += std::exp(x[i]) * Z2[i - 1] / (1 + Z2[i - 1] * gamma);
  }

  result *= 5.87 * std::pow(10, 10) / std::pow(t, 3);
  return 4 * gamma * gamma - result;
}

double bisGamma(double start, double end, double t, std::vector<double> &x) {
  while (std::fabs(start - end) > EPS) {
    auto middle = (start + end) / 2;

    if (getGamma(middle, t, x) <= 0) {
      start = middle;
    } else {
      end = middle;
    }
  }

  return (start + end) / 2;
}

inline double getAlpha(double gamma, double t) {
  return 0.285 * std::pow(10, -11) * std::pow(gamma * t, 3);
}

inline std::vector<double> getDeltaE(double gamma, double t) {
  std::vector<double> result(4);
  gamma /= 2;
  for (int i = 0; i < 4; i++) {
    result[i] = 8.61 * pow(10, -5) * t * log((1 + Z[i+1] * Z[i+1] * gamma) * (1 + gamma) / (1 + Z[i] * Z[i] * gamma));
  }

  return result;
}

double interpolateFromVectors(double x, int n, std::vector<double> v1, std::vector<double> v2) {
  std::vector<std::vector<double>> data(v1.size());

  for (int i = 0; i < v1.size(); i++) {
    data[i].push_back(v1[i]);
    data[i].push_back(v2[i]);
  }

  return interpolate(data, n, x);
}

inline std::vector<double> getK(double t, std::vector<double> &deltaE) {
  std::vector<double> k(4);

  for (int i = 0; i < 4; i++) {
    double qi1 = interpolateFromVectors(t, 4, Q[0], Q[i + 2]);
    double qi = interpolateFromVectors(t, 4, Q[0], Q[i + 1]);

    k[i] = 0.00483 * qi1 / qi * std::pow(t, 1.5) * std::exp(-(E[i] - deltaE[i]) * 11603 / t);
  }

  return k;
}

inline double getT(double z, double t0, double tw, double m) {
  return t0 + (tw - t0) * std::pow(z, m);
}

double integrate(double lb, double ub, const std::function<double(double)> &function) {
  double step = 0.05;
  double half_step = step / 2;
  double result = 0;

  while (lb <= ub) {
    double left = function(lb);
    lb += step;
    double right = function(lb);

    result += half_step * (left + right);
  }

  return result;
}

void solveSystem(std::vector<std::vector<double>> &left_side, std::vector<double> &right_side) {
  int n = static_cast<int>(left_side.size());
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      double sep = left_side[j][i] / left_side[i][i];
      for (int k = 0; k < n; k++) {
        left_side[j][k] -= left_side[i][k] * sep;
      }
      right_side[j] -= right_side[i] * sep;
    }
  }

  for (int i = n - 1; i >= 0; i--) {
    for (int k = i + 1; k < n; k++) {
      right_side[i] -= left_side[i][k] * right_side[k];
    }
    right_side[i] /= left_side[i][i];
  }
}

double maxDelta(std::vector<double> &x, std::vector<double> &deltaX) {
  double max_delta = std::abs(deltaX[0] / x[0]);
  for (int i = 1; i < x.size(); i++) {
    double delta = std::abs(deltaX[i] / x[i]);
    if (delta > max_delta) {
      max_delta = delta;
    }
  }

  return max_delta;
}

double Nt(double t, double p, std::vector<double> &x) {
  double gamma;
  while (true) {
    gamma = bisGamma(0, 3, t, x);
    double alpha = getAlpha(gamma, t);
    std::vector<double> deltaE = getDeltaE(gamma, t);
    std::vector<double> k = getK(t, deltaE);

    std::vector<std::vector<double>> left_side = {{ 1, -1, 1, 0, 0, 0 },
                                                  { 1, 0, -1, 1, 0, 0 },
                                                  { 1, 0, 0, -1, 1, 0 },
                                                  { 1, 0, 0, 0, -1, 1 },
                                                  { -std::exp(x[0]), -std::exp(x[1]), -std::exp(x[2]), -std::exp(x[3]), -std::exp(x[4]), -std::exp(x[5]) },
                                                  { std::exp(x[0]), 0, -Z[1] * std::exp(x[2]), -Z[2] * std::exp(x[3]), -Z[3] * std::exp(x[4]), -Z[4] * std::exp(x[5]) }};

    std::vector<double> right_side = {std::log(k[0]) + x[1] - x[2] - x[0],
                                      std::log(k[1]) + x[2] - x[3] - x[0],
                                      std::log(k[2]) + x[3] - x[4] - x[0],
                                      std::log(k[3]) + x[4] - x[5] - x[0],
                                      std::exp(x[0]) + std::exp(x[1]) + std::exp(x[2]) + std::exp(x[3]) + std::exp(x[4]) + std::exp(x[5]) - alpha - p * 7243 / t,
                                      Z[1] * std::exp(x[2]) + Z[2] * std::exp(x[3]) + Z[3] * std::exp(x[4]) + Z[4] * std::exp(x[5]) - std::exp(x[0]) };
    auto item = right_side;
    solveSystem(left_side, right_side);

    if (maxDelta(x, right_side) < EPS) {
      break;
    }

    for (int i = 0; i < x.size(); i++) {
      x[i] += right_side[i];
    }
  }

  double result = 0;
  if (first) {
    std::cout << "T: " << t << std::endl;
  }
  for (int i = 0; i < 6; i++) {
    if (first) {
      if (i == 0) {
        std::cout << "v: ";
      } else {
        std::cout << "x" << i << ": ";
      }
      std::cout << std::fixed << std::setprecision(7) << std::exp(x[i]) << ";  ";
    }
    result += std::exp(x[i]);
  }
  if (first) {
    std::cout << std::endl;
    std::cout << "Gamma: " << std::fixed << std::setprecision(4) << gamma << std::endl;
    first = false;
  }

  return result;
}

int main() {
  double p0, t0, tw, m;
  std::cout << "Enter P0: ";
  std::cin >> p0;
  std::cout << "Enter T0: ";
  std::cin >> t0;
  std::cout << "Enter Tw: ";
  std::cin >> tw;
  std::cout << "Enter m: ";
  std::cin >> m;

  double p_end = MAX_P;
  double p_start = MIN_P;
  std::vector<double> x = { -1, 3, -1, -20, -20, -20 };

//  auto t1 = std::chrono::high_resolution_clock::now();

  double left_const = 7243 * p0 / 300 / 2;
  while (std::abs(p_end - p_start) > EPS) {
    double middle_p = std::abs(p_end + p_start) / 2.;

    double integral = integrate(0, 1, [&](double z){ return Nt(getT(z, t0, tw, m), middle_p, x) * z; });

    if (left_const - integral > 0) {
      p_start = middle_p;
    } else {
      p_end = middle_p;
    }
  }

//  auto t2 = std::chrono::high_resolution_clock::now();
//  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

  std::cout << std::endl << "Result: " << std::fixed << std::setprecision(10) << p_end << std::endl;
//  std::cout << "Time: " << std::fixed << std::setprecision(3) << duration / 1000000. << "s";

  return 0;
}
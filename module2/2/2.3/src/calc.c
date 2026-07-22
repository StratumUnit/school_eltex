#include "calc.h"

double opSum(double a, double b) { return a + b; }

double opSub(double a, double b) { return a - b; }

double opMult(double a, double b) { return a * b; }

double opDiv(double a, double b) {

  if (!b) {
    return NAN;
  }
  return a / b;
}

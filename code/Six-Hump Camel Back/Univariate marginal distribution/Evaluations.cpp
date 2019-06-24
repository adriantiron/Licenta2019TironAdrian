#include <vector>
#include <cmath>

#include "Evaluations.h"
#include "Converter.h"

double sixh_eval(string bits, double l1, double u1, double l2, double u2, int m1, int m2)
{
	double x1 = to_double(bits.substr(0, m1), l1, u1, m1);
	double x2 = to_double(bits.substr(m1, m2), l2, u2, m2);
	return ((4 - 2.1*pow(x1, 2) + pow(x1, 4 / 3))*pow(x1, 2) + x1 * x2 + (-4 + 4 * pow(x2, 2))*pow(x2, 2));
}
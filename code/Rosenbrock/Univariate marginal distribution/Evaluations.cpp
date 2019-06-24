#include <vector>
#include <cmath>

#include "Evaluations.h"
#include "Converter.h"
using namespace std;

double rosenbrock_eval(string bits, double l, double u, int len, int m)
{
	double total = 0;

	for (int i = 0; i < len - 2 * m; i += m)
	{
		double d1 = to_double(bits.substr(i, m), l, u, m),
			d2 = to_double(bits.substr(i + m, m), l, u, m);
		total += 100 * pow((d2 - pow(d1, 2)), 2) + pow((1 - d1), 2);
	}

	return total;
}
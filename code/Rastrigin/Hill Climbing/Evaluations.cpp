#include <vector>
#include <cmath>

#include "Evaluations.h"
#include "Converter.h"

#define M_PI (3.141592653589793)

using namespace std;

double rastrigin_eval(string bits, double l, double u, int len, int m)
{
	double total = 0;

	for (int i = 0; i < len - m; i += m)
	{
		double d = to_double(bits.substr(i, m), l, u, m);
		total += (pow(d, 2) - 10 * cos(2 * M_PI * d)) + 10;
	}

	return total;
}
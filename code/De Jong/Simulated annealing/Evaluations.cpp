#include <vector>
#include <cmath>

#include "Evaluations.h"
#include "Converter.h"

using namespace std;

double dejong_eval(string bits, double l, double u, int len, int m)
{
	double total = 0;

	for (int i = 0; i < len - m; i += m) {

		double x = to_double(bits.substr(i, m), l, u, m);

		total += pow(x, 2);
	}

	return total;

}
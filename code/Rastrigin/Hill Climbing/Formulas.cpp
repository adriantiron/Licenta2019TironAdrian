#include <cmath>
#include <string>

using namespace std;

int compute_n(double l, double u, int d)
{
	return ((int)ceil(log2((u - l) * pow(10, d))));
}

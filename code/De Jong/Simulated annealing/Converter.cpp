#include <iostream>
#include <vector>
#include <cmath>

#include "Converter.h"

using namespace std;

int to_base_10(string bits)
{
	int val = 0, add = 1;

	for (int i = bits.length(); i > 0; i--)
	{
		if (bits.at(i - 1) == '1')
			val += add;

		add *= 2;
	}

	return val;
}

double to_double(string bits, double a, double b, int m)
{
	return (a + to_base_10(bits) * (b - a) / (pow(2, m) - 1));
}


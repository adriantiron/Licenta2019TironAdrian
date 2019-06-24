#include <iostream>
#include <vector>
#include <random>
#include "Random_Generator.h"

using namespace std;

random_device dbl;
mt19937 ss(dbl());
uniform_real_distribution<double> unif(0.0, 1.0);

double randomDBL()
{
	return unif(ss);
}

int rangeRandomAlg(int max) {
	int remainder = RAND_MAX % max;
	int x;
	do {
		x = rand();
	} while (x >= RAND_MAX - remainder);
	return x % max;
}

string random_bitstring(int len)
{
	string bits;

	for (int i = 0; i < len; ++i)
	{
		if (randomDBL() < 0.5)

			bits += "1";

		else

			bits += "0";
	}

	return bits;
}
#include <iostream>
#include <string>
#include <stdio.h>
#include <random>
#include <fstream>

#include "Random_Generator.h"

#pragma warning (disable:4996)

using namespace std;
ofstream write("plot.csv");

const int MAX_ITER = 100, LEN = 50;
const double T0 = 100, Tn = 0, cycles = 403;

struct chromos
{
	string bits;

	double fitness;

	chromos() : bits(""), fitness(DBL_MAX) {};
	chromos(string newBits, double newFitness) : bits(newBits), fitness(newFitness) {}
};

double cool_temp(int k) {
	return Tn + (T0 - Tn) * pow((cycles - k) / cycles, 2);
}

int activate(int x)
{
	if (x == 0)
		return -1;

	return 1;
}

int fitness(string bits)
{
	int res = 0;

	for (int i = 1; i < bits.size(); ++i)
	{
		int inner = 0;
		for (int j = 1; j <= bits.size() - i; ++j)
			inner += activate(bits[i - 1] - '0') * activate(bits[i - 1 + j - 1] - '0');

		res += pow(inner, 2);
	}

	return res;
}

string random_neighbor(string cand)
{
	int pos = rangeRandomAlg(cand.size());

	cand[pos] = (cand[pos] == '1') ? '0' : '1';

	return cand;
}

chromos sa_alg()
{
	double T = T0;
	chromos best, cr, rn;
	/////////----------//////////////////----------//////////////////----------/////////
	int count = 0;
	vector<double> forPlotting;
	write << "Generation,Fitness\n";

	cr.bits = random_bitstring(LEN);
	cr.fitness = fitness(cr.bits);
	while (T > Tn)
	{
		for (int i = 0; i < MAX_ITER; i++)
		{
			rn.bits = random_neighbor(cr.bits);
			rn.fitness = fitness(rn.bits);
			if (rn.fitness < cr.fitness)
				cr = rn;
			else if (randomDBL() < exp(-1 * abs(rn.fitness - cr.fitness) / T))
				cr = rn;
		}
		T = cool_temp(count);
		count++;

		if (cr.fitness < best.fitness)
			best = cr;

		forPlotting.push_back(best.fitness);
	}

	for (int myit = 0; myit < forPlotting.size(); ++myit)
	{
		write << myit + 1 << "," << forPlotting.at(myit) << "\n";
	}

	return best;
}

int main()
{
	chromos best;

	cout << "Wait...\n";
	best = sa_alg();

	cout << "\nBest chromosome with fitness: " << best.fitness << "\n";

	return 0;
}
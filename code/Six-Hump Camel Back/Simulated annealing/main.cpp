#include <iostream>
#include <string>
#include <stdio.h>
#include <random>
#include <fstream>

#include "Random_Generator.h"
#include "Converter.h"
#include "Evaluations.h"
#include "Formulas.h"

#pragma warning (disable:4996)

using namespace std;
ofstream write("plot.csv");

const int MAX_ITER = 100;
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

// Adunam minimul la rezultat pt a centra valorile in jurul la 0
double fitness(string bits, double l1, double u1, double l2, double u2, int m1, int m2)
{
	double res = 1.0316 + sixh_eval(bits, l1, u1, l2, u2, m1, m2);

	if (res < 0)
		return (-1 * res);

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
	double l1 = -3.0, u1 = 3.0, l2 = -2.0, u2 = 2.0, T = T0;
	int prec = 5, len, m1, m2;
	chromos best, cr, rn;

	m1 = compute_n(l1, u1, prec);
	m2 = compute_n(l2, u2, prec);
	len = m1 + m2;

	/////////----------//////////////////----------//////////////////----------/////////
	int count = 0;
	vector<double> forPlotting;
	write << "Generation,Fitness\n";

	cr.bits = random_bitstring(len);
	cr.fitness = fitness(cr.bits, l1, u1, l2, u2, m1, m2);
	while (T > Tn)
	{
		for (int i = 0; i < MAX_ITER; i++)
		{
			rn.bits = random_neighbor(cr.bits);
			rn.fitness = fitness(rn.bits, l1, u1, l2, u2, m1, m2);
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

	cout << "Best chromosome with fitness: " << best.fitness << "\n";

	return 0;
}
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

#define MAX_ITER 100
#define MAX_RUNS 4

struct chromos
{
	string bits;

	double fitness;

	chromos() : bits(""), fitness(DBL_MAX) {};
	chromos(string newBits, double newFitness) : bits(newBits), fitness(newFitness) {}
};

double fitness(string bits, double l, double u, int len, int m)
{
	double res = dejong_eval(bits, l, u, len, m);

	if (res <= 0)
		return (-1 * res);

	return res;
}

string random_neighbor(string cand)
{
	int pos = rangeRandomAlg(cand.size());

	cand[pos] = (cand[pos] == '1') ? '0' : '1';

	return cand;
}

chromos hc_alg()
{
	double l = -5.12, u = 5.12;
	int var_nb = 6, prec = 5, len, m;
	chromos best;

	m = compute_n(l, u, prec);
	len = m * var_nb;

	/////////----------//////////////////----------//////////////////----------/////////
	int count = 0;
	vector<double> forPlotting;
	write << "Generation,Fitness\n";

	while (true)
	{
		count++;

		chromos candidate;
		candidate.bits = random_bitstring(len);
		candidate.fitness = fitness(candidate.bits, l, u, len, m);

		int generationCount = 0;
		bool stop = false;

		// Acest while se opreste cand depaseste nr de generatii stabilit

		while (!stop)
		{
			chromos neigh;

			// Alegem un vecin intamplator la distanta de 1 bit si ii calculam fitness-ul
			neigh.bits = random_neighbor(candidate.bits);
			neigh.fitness = fitness(neigh.bits, l, u, len, m);

			// Daca e mai promitator, il salvam
			if (neigh.fitness < candidate.fitness)
				candidate = neigh;

			if (candidate.fitness < best.fitness)
				best = candidate;

			forPlotting.push_back(best.fitness);

			++generationCount;

			// Daca depasesc maximul de generatii permise, termin cautarea
			if (generationCount > MAX_ITER)
				stop = true;
		}

		if (count == MAX_RUNS) {
			cout << "MAX_RUNS reached...\n\n";

			for (int myit = 0; myit < forPlotting.size(); ++myit)
			{
				write << myit + 1 << "," << forPlotting.at(myit) << "\n";
			}

			return best;
		}
	}

	return best;
}

int main()
{
	chromos best;

	cout << "Wait...\n";
	best = hc_alg();

	cout << fixed << "Best chromosome with fitness: " << best.fitness << "\n";

	return 0;
}
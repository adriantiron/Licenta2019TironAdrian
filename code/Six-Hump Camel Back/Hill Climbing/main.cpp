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

chromos hc_alg()
{
	double l1 = -3.0, u1 = 3.0, l2 = -2.0, u2 = 2.0;
	int prec = 5, len, m1, m2;
	chromos best;

	m1 = compute_n(l1, u1, prec);
	m2 = compute_n(l2, u2, prec);
	len = m1 + m2;

	/////////----------//////////////////----------//////////////////----------/////////
	int count = 0;
	vector<double> forPlotting;
	write << "Generation,Fitness\n";

	while (true)
	{
		count++;

		chromos candidate;
		candidate.bits = random_bitstring(len);
		candidate.fitness = fitness(candidate.bits, l1, u1, l2, u2, m1, m2);

		int generationCount = 0;
		bool stop = false;

		// Acest while se opreste cand depaseste nr de generatii stabilit

		while (!stop)
		{
			chromos neigh;

			// Alegem un vecin intamplator la distanta de 1 bit si ii calculam fitness-ul
			neigh.bits = random_neighbor(candidate.bits);
			neigh.fitness = fitness(neigh.bits, l1, u1, l2, u2, m1, m2);

			// Daca e mai promitator, il salvam
			if (neigh.fitness < candidate.fitness)
				candidate = neigh;

			if (candidate.fitness < best.fitness) {
				best = candidate;
			}

			++generationCount;

			// Daca depasesc maximul de generatii permise, termin cautarea
			if (generationCount > MAX_ITER)
				stop = true;

			// std::cout << to_double(best.bits.substr(0, m1), l1, u1, m1) << " " << to_double(best.bits.substr(m1, m2), l2, u2, m2) << "\n";
			forPlotting.push_back(best.fitness);
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

	cout << "Best chromosome with fitness: " << best.fitness << "\n";

	return 0;
}
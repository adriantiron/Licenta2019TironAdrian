#include <iostream>
#include <string>
#include <stdio.h>
#include <random>
#include <fstream>

#include "Random_Generator.h"

#pragma warning (disable:4996)

using namespace std;
ofstream write("plot.csv");

#define MAX_ITER 100
#define MAX_RUNS 4
#define LEN 50

struct chromos
{
	string bits;

	double fitness;

	chromos() : bits(""), fitness(DBL_MAX) {};
	chromos(string newBits, double newFitness) : bits(newBits), fitness(newFitness) {}
};

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

chromos hc_alg()
{
	chromos best;
	/////////----------//////////////////----------//////////////////----------/////////
	int count = 0;
	vector<double> forPlotting;
	write << "Generation,Fitness\n";

	while (true)
	{
		count++;

		chromos candidate;
		candidate.bits = random_bitstring(LEN);
		candidate.fitness = fitness(candidate.bits);

		int generationCount = 0;
		bool stop = false;

		// Acest while se opreste cand gaseste o solutie acceptabila, sau depaseste 400 de generatii

		while (!stop)
		{
			chromos neigh;

			neigh.bits = random_neighbor(candidate.bits);
			neigh.fitness = fitness(neigh.bits);

			if (neigh.fitness < candidate.fitness)
				candidate = neigh;

			if (candidate.fitness < best.fitness)
				best = candidate;

			++generationCount;
			forPlotting.push_back(best.fitness);

			// Daca depasesc maximul de generatii permise, termin cautarea
			if (generationCount > MAX_ITER)
				stop = true;
		}

		//cout << best.bits << "\n";
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

	cout << "\nBest chromosome with fitness: " << best.fitness << "\n";

	return 0;
}
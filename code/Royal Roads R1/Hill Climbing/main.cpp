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

#define LENGTH 256
#define BLOCK_SIZE 4

struct chromos
{
	string bits;

	double fitness;

	chromos() : bits(""), fitness(DBL_MIN) {};
	chromos(string newBits, double newFitness) : bits(newBits), fitness(newFitness) {}
};

int fitness(string bits)
{
	int res = 0;
	string block(BLOCK_SIZE, '1');

	for (int i = 0; i < LENGTH; i += BLOCK_SIZE)
	{
		if (bits.substr(i, BLOCK_SIZE) == block) {
			res += BLOCK_SIZE;
		}
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
		candidate.bits = random_bitstring(LENGTH);
		candidate.fitness = fitness(candidate.bits);

		int generationCount = 0;
		bool stop = false;

		// Acest while se opreste cand depaseste nr de generatii stabilit

		while (!stop)
		{
			chromos neigh;

			// Alegem un vecin intamplator la distanta de 1 bit si ii calculam fitness-ul
			neigh.bits = random_neighbor(candidate.bits);
			neigh.fitness = fitness(neigh.bits);

			// Daca e mai promitator, il salvam
			if (neigh.fitness > candidate.fitness)
				candidate = neigh;

			if (candidate.fitness > best.fitness)
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
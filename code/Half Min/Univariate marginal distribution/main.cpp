#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>

#include "Random_Generator.h"

#pragma warning (disable:4996)

using namespace std;
ofstream write("plot.csv");

#define POP_SIZE 120
#define SELECT_SIZE 30
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

void compute_bit_probabilities(chromos selected[], vector<double> &prob)
{
	int len = selected[0].bits.length();

	for (int i = 0; i < SELECT_SIZE; ++i)
		for (int j = 0; j < len; ++j)
			prob.at(j) += (int)(selected[i].bits.at(j) - '0');

	for (int i = 0; i < len; ++i)
		prob.at(i) /= (double)(SELECT_SIZE);
}

bool chromos_compare(chromos ch1, chromos ch2) { return ch1.fitness < ch2.fitness; }

chromos umda_alg()
{
	chromos best;
	vector<double> probab(LEN);
	/////////----------//////////////////----------//////////////////----------/////////
	int count = 0;
	vector<double> forPlotting;
	write << "Generation,Fitness\n";

	while (true)
	{
		count++;

		chromos Population[POP_SIZE];  // populatia

		// Initializez populatia cu fitness zero si string de biti random

		for (int i = 0; i < POP_SIZE; ++i)
		{
			Population[i].bits = random_bitstring(LEN);
			Population[i].fitness = DBL_MAX;
		}

		int generationCount = 0;
		bool stop = false;

		// Acest while se opreste cand gaseste o solutie acceptabila, sau depaseste 400 de generatii

		while (!stop)
		{
			// Asignez fitness-ul corespunzator fiecarui cromozom
			for (int i = 0; i < POP_SIZE; ++i)
				Population[i].fitness = fitness(Population[i].bits);

			// Sortam populatia in functie de fitness
			sort(Population, Population + POP_SIZE, chromos_compare);

			// Salvam cel mai promitator individ
			if (Population[0].fitness < best.fitness)
				best = Population[0];

			chromos selected[SELECT_SIZE]; // Selectam primii SELECT_SIZE cei mai buni indivizi

			for (int it = 0; it < SELECT_SIZE; ++it)
				selected[it] = Population[it];

			fill(probab.begin(), probab.end(), 0.0); // resetam vectorul de probabilitati
			compute_bit_probabilities(selected, probab);

			// Generam o populatie noua
			for (int i = 0; i < POP_SIZE; ++i)
				Population[i].bits = random_bitstring_with_prob(LEN, probab);

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
	best = umda_alg();

	cout << "\nBest chromosome with fitness: " << best.fitness << "\n";
	return 0;
}
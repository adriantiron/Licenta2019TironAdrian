#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>

#include "Converter.h"
#include "Evaluations.h"
#include "Random_Generator.h"
#include "Formulas.h"

#pragma warning (disable:4996)

using namespace std;
ofstream write("plot.csv");

#define POP_SIZE 120
#define SELECT_SIZE 30
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

void compute_bit_probabilities(chromos selected[], vector<double> &prob)
{
	int len = selected[0].bits.length();

	for (int i = 0; i < SELECT_SIZE; ++i)
		for (int j = 0; j < len; ++j)
			prob.at(j) += (double)(selected[i].bits.at(j) - '0');

	for (int i = 0; i < len; ++i)
		prob.at(i) /= (double)(SELECT_SIZE);
}

bool chromos_compare(chromos ch1, chromos ch2) { return ch1.fitness < ch2.fitness; }

chromos umda_alg()
{
	double l1 = -3.0, u1 = 3.0, l2 = -2.0, u2 = 2.0;
	int prec = 5, len, m1, m2;
	chromos best;

	m1 = compute_n(l1, u1, prec);
	m2 = compute_n(l2, u2, prec);
	len = m1 + m2;

	vector<double> probab(len);

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
			Population[i].bits = random_bitstring(len);
			Population[i].fitness = DBL_MAX;
		}

		int generationCount = 0;
		bool stop = false;

		// Acest while se opreste cand gaseste o solutie acceptabila, sau depaseste 400 de generatii

		while (!stop)
		{
			// Asignez fitness-ul corespunzator fiecarui cromozom
			for (int i = 0; i < POP_SIZE; ++i)
				Population[i].fitness = fitness(Population[i].bits, l1, u1, l2, u2, m1, m2);

			// Sortam populatia in functie de fitness
			sort(Population, Population + POP_SIZE, chromos_compare);

			// Salvam cel mai promitator individ
			if (Population[0].fitness < best.fitness) {
				best = Population[0];
			}

			chromos selected[SELECT_SIZE]; // Selectam primii SELECT_SIZE cei mai buni indivizi

			for (int it = 0; it < SELECT_SIZE; ++it)
				selected[it] = Population[it];

			fill(probab.begin(), probab.end(), 0.0); // resetam vectorul de probabilitati
			compute_bit_probabilities(selected, probab);

			// Generam o populatie noua
			for (int i = 0; i < POP_SIZE; ++i)
				Population[i].bits = random_bitstring_with_prob(len, probab);

			++generationCount;

			// Daca depasesc maximul de generatii permise, termin cautarea
			if (generationCount > MAX_ITER)
				stop = true;

			// std::cout << to_double(best.bits.substr(0, m1), l1, u1, m1) << " " << to_double(best.bits.substr(m1, m2), l2, u2, m2) << "\n";
			forPlotting.push_back(best.fitness);
		}

		if (count == MAX_RUNS) {
			cout << "MAX_RUNS reached...";

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

	cout << "\n\nSolution found with fitness = " << best.fitness << "\n";

	return 0;
}
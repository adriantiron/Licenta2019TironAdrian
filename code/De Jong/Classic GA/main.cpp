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

const int MAX_ITER = 100, MAX_RUNS = 4, POP_SIZE = 120;
float INTERBREED_RATE, MUTATION_RATE;

struct chromos
{
	string bits;

	double fitness;

	chromos() : bits(""), fitness(DBL_MAX) {};
	chromos(string newBits, double newFitness) : bits(newBits), fitness(newFitness) {}
};

double fitness(string bits, double l, double u, int len, int m);
chromos select(double TotalFitness, chromos* Population);
void mutate(string &bits);
void interbreed(string &child1, string &child2, int veclen);

chromos ga_alg()
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

		chromos Population[POP_SIZE];  // populatia

		// Initializez populatia cu fitness zero si string de biti random

		for (int i = 0; i < POP_SIZE; ++i)
			Population[i].bits = random_bitstring(len);


		int generationCount = 0;
		bool stop = false;

		int chromoCount = 0;
		// Acest while se opreste cand gaseste o solutie acceptabila, sau depaseste 400 de generatii

		while (!stop)
		{
			chromoCount++;

			double TotalFitness = 0.0, popbest = Population[0].fitness;
			// Asignez fitness-ul corespunzator fiecarui cromozom
			for (int i = 0; i < POP_SIZE; ++i)
			{
				Population[i].fitness = fitness(Population[i].bits, l, u, len, m);
				TotalFitness += Population[i].fitness;

				if (Population[i].fitness < popbest)
					popbest = Population[i].fitness;

				if (Population[i].fitness < best.fitness) {
					best = Population[i];
				}
			}

			double avg = TotalFitness / POP_SIZE;
			// Noua populatie temporara
			chromos temp[POP_SIZE];
			int tempCount = 0;

			// Acest while se opreste cand lungimea pop temporare e egala cu cea original;
			while (tempCount < POP_SIZE)
			{
				// Select, prin roata norocului, 2 copii indeajuns de buni
				chromos child1 = select(TotalFitness, Population);
				chromos child2 = select(TotalFitness, Population);
				double fp = max(child1.fitness, child2.fitness);

				INTERBREED_RATE = 1.0;
				if (fp < avg)
					INTERBREED_RATE = (popbest - fp) / (popbest - avg);
				interbreed(child1.bits, child2.bits, len);

				MUTATION_RATE = 0.5;
				if (child1.fitness < avg)
					MUTATION_RATE = 0.5 * (popbest - child1.fitness) / (popbest - avg);
				mutate(child1.bits);

				MUTATION_RATE = 0.5;
				if (child1.fitness < avg)
					MUTATION_RATE = 0.5 * (popbest - child2.fitness) / (popbest - avg);
				mutate(child2.bits);

				// Ii adaug la pop temporara
				temp[tempCount++] = child1;
				temp[tempCount++] = child2;
			}

			// Populatia temporara devinea cea actuala
			for (int i = 0; i < POP_SIZE; ++i)
				Population[i] = temp[i];

			++generationCount;
			forPlotting.push_back(best.fitness);

			// Daca depasesc maximul de generatii permise, termin cautarea
			if (generationCount > MAX_ITER)
				stop = true;

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

double fitness(string bits, double l, double u, int len, int m)
{
	double res = dejong_eval(bits, l, u, len, m);

	if (res <= 0)
		return (-1 * res);

	return res;
}

chromos select(double TotalFitness, chromos* Population)
{
	double cut = (double)(randomDBL() * TotalFitness);

	double cumulativeFitness = 0.0;

	for (int i = 0; i < POP_SIZE; ++i)
	{
		cumulativeFitness += Population[i].fitness;

		if (cumulativeFitness >= cut)
			return Population[i];
	}

	return chromos();
}

void mutate(string &bits)
{
	for (int i = 0; i < bits.length(); ++i)
	{
		if (randomDBL() < MUTATION_RATE)
		{
			if (bits.at(i) == '1')
				bits.at(i) = '0';

			else
				bits.at(i) = '1';
		}
	}

	return;
}

void interbreed(string &child1, string &child2, int veclen)
{
	if (randomDBL() < INTERBREED_RATE)
	{
		int INTERBREED = (int)(randomDBL() * veclen);

		string t1 = child1.substr(0, INTERBREED) + child2.substr(INTERBREED, veclen);
		string t2 = child2.substr(0, INTERBREED) + child1.substr(INTERBREED, veclen);

		child1 = t1; child2 = t2;
	}
}

int main()
{
	chromos best;

	cout << "Wait...\n";
	best = ga_alg();

	cout << "\nBest chromosome with fitness: " << best.fitness << "\n";

	return 0;
}
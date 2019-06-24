#pragma once
#ifndef _RANDOMGENERATOR_H_
#define _RANDOMGENERATOR_H_

#include <iostream>
#include <random>
#include <vector>

using namespace std;

// RANDOM GENERATOR

string random_bitstring(int len);

string random_bitstring_with_prob(int len, vector<double> prob);

double randomDBL();

#endif 


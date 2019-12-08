#include "AtariGame.h"

class Environment 
{
public:
	AtariGame* mGame;
	Execution mExe;
	Genotype* mPopulation;
	int mPopulationSize;

	Environment();
	Environment(AtariGame* game, Genotype* pop, int pop_size);
	virtual ~Environment();

	virtual int getFitness(int indi);
};
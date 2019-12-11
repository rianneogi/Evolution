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

	virtual void init(AtariGame* game, Genotype* pop, int pop_size);
	virtual int getFitness(int indi);
};
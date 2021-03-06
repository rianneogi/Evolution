#include "Environment.h"

class SupervisedEnvironment : public Environment
{
public:
	Genotype* mSupervisor;
	int* mScores;

	SupervisedEnvironment();
	SupervisedEnvironment(AtariGame *game, Genotype *pop, int pop_size, Genotype *supervisor);
	~SupervisedEnvironment();

	int getFitness(int indi);
};
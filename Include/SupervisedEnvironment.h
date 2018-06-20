#include "Environment.h"

class SupervisedEnvironment : public Environment
{
public:
	Genotype* mSupervisor;

	SupervisedEnvironment();
	SupervisedEnvironment(Genotype* pop, int pop_size);
	~SupervisedEnvironment();

	int getFitness(int indi);
};
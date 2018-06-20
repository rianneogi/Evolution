#include "Environment.h"

class SupervisedEnvironment : public Environment
{
public:
	Genotype* mSupervisor;

	SupervisedEnvironment();
	~SupervisedEnvironment();

	int getFitness(const Genotype* indi);
};
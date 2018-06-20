#include "AtariGame.h"

class Environment 
{
public:
	AtariGame* mGame;
	Execution mExe;

	Environment();
	virtual ~Environment();

	virtual int getFitness(const Genotype* indi);
};
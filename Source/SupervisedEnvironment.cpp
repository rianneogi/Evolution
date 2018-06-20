#include "SupervisedEnvironment.h"

SupervisedEnvironment::SupervisedEnvironment()
{

}

SupervisedEnvironment::SupervisedEnvironment(AtariGame* game, Genotype* pop, int pop_size, Genotype* supervisor) 
	: Environment(game, pop, pop_size), mSupervisor(supervisor)
{
	mScores = new int[pop_size];
}

SupervisedEnvironment::~SupervisedEnvironment()
{
	delete [] mScores;
}

int SupervisedEnvironment::getFitness(int indi)
{
	mGame->restart();
	mExe.reset();
	int gameReward = 0;
	int supervisorReward = 0;
	while (!mGame->is_over())
	{
		gameReward += mGame->mALE->act(PLAYER_A_FIRE);
		mExe.reset();
		mExe.run_code(&mPopulation[indi], 0);

		ActionVect vect = mGame->mALE->getMinimalActionSet();
		int size = vect.size();
		gameReward += mGame->do_action(mExe.mRegisters[0] % size);

		mExe.reset();
		mExe.run_code(mSupervisor, 0);
		supervisorReward += mExe.mRegisters[0];
	}
	mScores[indi] = gameReward;
	return supervisorReward;
}
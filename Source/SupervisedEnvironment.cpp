#include "SupervisedEnvironment.h"

SupervisedEnvironment::SupervisedEnvironment()
{

}

SupervisedEnvironment::~SupervisedEnvironment()
{
	
}

int SupervisedEnvironment::getFitness(const Genotype *indi)
{
	mGame->restart();
	mExe.reset();
	int gameReward = 0;
	int supervisorReward = 0;
	while (!mGame->is_over())
	{
		gameReward += mGame->mALE->act(PLAYER_A_FIRE);
		mExe.reset();
		mExe.run_code(indi, 0);

		ActionVect vect = mGame->mALE->getMinimalActionSet();
		int size = vect.size();
		gameReward += mGame->do_action(mExe.mRegisters[0] % size);

		mExe.reset();
		mExe.run_code(mSupervisor, 0);
		supervisorReward += mExe.mRegisters[0];
	}
	return supervisorReward;
}
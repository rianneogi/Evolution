#include "Environment.h"

Environment::~Environment()
{

}

int Environment::getFitness(const Genotype *indi)
{
	mGame->restart();
	// game.resetState();
	mExe.reset();
	// exe.mInputs = game.mALE->getRAM().array();
	// exe.mInputSize = game.mALE->getRAM().size();
	int totalReward = 0;
	// indi->print();
	while (!mGame->is_over())
	{
		totalReward += mGame->mALE->act(PLAYER_A_FIRE);
		mExe.resetStep();

		mExe.run_code(indi, 0);

		ActionVect vect = mGame->mALE->getMinimalActionSet();
		int size = vect.size();
		totalReward += mGame->do_action(mExe.mRegisters[0] % size);
	}
	return totalReward;
}
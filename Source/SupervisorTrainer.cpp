#include "SupervisorTrainer.h"

void SupervisorTrainer::init()
{
	// mLoadPath = "breakout_best";
	// mSavePath = "breakout_best";
	mLoadPath = "";
	mSavePath = "";

	const int GEN_PRINT_DELAY = 1;

	mNumSupervisors = 10;
	mNumPopulation = 10;
	mNumSurvivors = 1;
	mNumSupervisorSurvivors = 1;
	mNumSubGenerations = 10;

	mGame = new AtariGame("ALE/roms/breakout.bin", 123, false);
	// game.mALE->act(PLAYER_A_FIRE);
	ALEState baseState = mGame->mALE->cloneSystemState();
	const int INPUT_SIZE = mGame->mALE->getRAM().size();
	const int REG_SIZE = 8;
	printf("INPUT SIZE: %d\n", INPUT_SIZE);
	
	mExe.mRegisters = new int[REG_SIZE];
	mExe.mRegisterSize = REG_SIZE;
	mExe.mInputSize = INPUT_SIZE;
	mExe.mInputs = mGame->mALE->getRAM().array();
	mExe.reset();
	
	mSupervisors = new Genotype[mNumSupervisors];
	mSupervisorScores = new int[mNumSupervisors];
	mSupervisorLastBest = new int[mNumSupervisors];
	// mPopulation = new Genotype[mNumPopulation * mNumSupervisors];
	// mScores = new int[mNumPopulation*mNumSupervisors];
	// mLastBest = new int[mNumPopulation*mNumSupervisors];
	mTrainers = new GenericTrainer[mNumSupervisors];
	mEnvs = new SupervisedEnvironment[mNumSupervisors];

	//Load
	if(mLoadPath!="")
	{
		mSupervisors[0].load(mLoadPath);
		for(int i = 1;i<mNumSupervisors;i++)
		{
			mSupervisors[i] = mSupervisors[0];
		}
	}
	
	//Init
	// for(int i = 1;i<mNumPopulation*mNumSupervisors;i++) //mutate everyone except first
	// {
	//     mPopulation[i].mutate();
	//     mScores[i] = 0;
	//     mLastBest[i] = 0;
	// }
	for(int i = 0;i<mNumSupervisors;i++)
	{
		mSupervisors[i].mutate();
		mTrainers[i].init(mNumPopulation,mNumSurvivors,"","",1);
		mEnvs[i] = SupervisedEnvironment(mGame, mTrainers[i].mPopulation, mTrainers[i].mNumPopulation, &mSupervisors[i]);
		mTrainers[i].mEnv = &mEnvs[i];
		// mEnvs[i].mSupervisor = &mSupervisors[i];
		// mEnvs[i].mGame = mGame;

		// mEnvs[i].mPopulationSize = mTrainers[i].mNumPopulation;
		// mEnvs[i].mPopulation = mTrainers[i].mPopulation;
	}

	// mBestID.resize(mNumSurvivors);
	mSupervisorBestID.resize(mNumSupervisorSurvivors);
}

int SupervisorTrainer::testSupervisor(int num_gen, int sup_id, int sup_children)
{
	mTrainers[sup_id].train(num_gen);
	// return mTrainers[sup_id].mScores[mTrainers[sup_id].mBestID[0]];
	return mEnvs[sup_id].mScores[mTrainers[sup_id].mBestID[0]];
}

void SupervisorTrainer::train()
{
	const int GEN_PRINT_DELAY = 1;

	bool running = true;
	for(int gen = 0; running; gen++)
	{
		//Test
		for(int i = 0;i<mNumSupervisors;i++)
		{
			// if(Scores[i]!=0) continue;
			
			// mScores[i] = run_atari(*mGame, mExe, &mPopulation[i]);
			mSupervisorScores[i] = testSupervisor(mNumSubGenerations, i, mNumPopulation);

			printf("Supervisor Gen %d, Agent %d: Score %d\n", gen, i, mSupervisorScores[i]);
		}
		
		//Cull
		memset(mSupervisorLastBest, 0, mNumSupervisors*sizeof(int));
		for(int i = 0;i<mNumSupervisorSurvivors;i++)
		{
			int max = 0;
			int max_id = 0;
			for(int j = 0;j<mNumSupervisors;j++)
			{
				if(mSupervisorLastBest[j] == 1)
				{
					continue;
				}
				if(mSupervisorScores[j]>=max)
				{
					max = mSupervisorScores[j];
					max_id = j;
				}
			}
			mSupervisorBestID[i] = max_id;
			mSupervisorLastBest[max_id] = 1;
		}
		
		int min = 10000;
		int min_id = 0;
		for(int j = 0;j<mNumSupervisors;j++)
		{
			if(mSupervisorLastBest[j] == 1)
			{
				continue;
			}
			if(mSupervisorScores[j]<min)
			{
				min = mSupervisorScores[j];
				min_id = j;
			}
		}
		
		//Print
		if(gen%GEN_PRINT_DELAY==0)
		{
			printf("Supervisor Generation: %d, Max Score: %d, inst %d %d, min %d, max id: %d\n", gen, mSupervisorScores[mSupervisorBestID[0]], 
				mSupervisors[mSupervisorBestID[0]].mGenes[0].mCode.size(), mSupervisors[mSupervisorBestID[0]].mGenes.size(), min, mSupervisorBestID[0]);
			for(int i = 0;i<mNumSupervisorSurvivors;i++)
			{
				printf("%d ", mSupervisorScores[mSupervisorBestID[i]]);
			}   
			printf("\n");

			if(mSavePath!="")
			{
				mSupervisors[mSupervisorBestID[0]].save(mSavePath);
			}
		}
		
		//Reproduce
		for(int i = 0;i<mNumSupervisors;i++)
		{
			if(mSupervisorLastBest[i]==0)
			{
				if (mSupervisorScores[i] < mSupervisorScores[mSupervisorBestID[mNumSupervisorSurvivors - 1]])
				{
					int r = rand()%mNumSupervisorSurvivors;
					mSupervisors[i] = mSupervisors[mSupervisorBestID[r]];

					for(int j = 0;j<mNumPopulation;j++) //copy the population also
					{
						mTrainers[i].mPopulation[j] = mTrainers[mSupervisorBestID[r]].mPopulation[j];
					}
				}
				
				mSupervisors[i].mutate();
			}
			// else if(Scores[i]==0)
			// {
			//     gPopulation[i].mutate();
			// }
			// Scores[i] = 0;
		}
		memset(mSupervisorScores, 0, mNumSupervisors * sizeof(int));
	}
}
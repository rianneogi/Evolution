#include "SupervisorTrainer.h"

void SupervisorTrainer::init()
{
	mLoadPath = "breakout_best";
    mSavePath = "breakout_best";

    const int GEN_PRINT_DELAY = 1;

    mNumSupervisors = 5;
    mNumPopulation = 5;
    mNumSurvivors = 1;
    mNumSupervisorSurvivors = 1;
    mNumSubGenerations = 5;

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
    mPopulation = new Genotype[mNumPopulation * mNumSupervisors];
    mScores = new int[mNumPopulation*mNumSupervisors];
    mLastBest = new int[mNumPopulation*mNumSupervisors];
    mTrainers = new GenericTrainer[mNumSupervisors];
    mEnvs = new SupervisedEnvironment[mNumSupervisors];

    //Load
    if(mLoadPath!="")
    {
        mPopulation[0].load(mLoadPath);
        for(int i = 1;i<mNumPopulation*mNumSupervisors;i++)
        {
            mPopulation[i] = mPopulation[0];
        }
    }
    
    //Init
    for(int i = 1;i<mNumPopulation*mNumSupervisors;i++) //mutate everyone except first
    {
        mPopulation[i].mutate();
        mScores[i] = 0;
        mLastBest[i] = 0;
    }
    for(int i = 0;i<mNumSupervisors;i++)
    {
        // mEnvs[i].mSupervisor = &mSupervisors[i];
        // mEnvs[i].mGame = mGame;
        mSupervisors[i].mutate();
        mTrainers[i].init(10,1,"","",1);
        mEnvs[i] = SupervisedEnvironment(mGame, mTrainers[i].mPopulation, mTrainers[i].mNumPopulation, &mSupervisors[i]);
        mTrainers[i].mEnv = &mEnvs[i];
        // mEnvs[i].mPopulationSize = mTrainers[i].mNumPopulation;
        // mEnvs[i].mPopulation = mTrainers[i].mPopulation;
    }

    mBestID.resize(mNumSurvivors);
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

            printf("Supervisor Gen %d, Agent %d: Score %d, Inst: %d, Genes: %d\n", gen, i, mScores[i], mPopulation[i].mGenes[0].mCode.size(), mPopulation[i].mGenes.size());
        }
        
        //Cull
        memset(mSupervisorLastBest, 0, mNumSurvivors*sizeof(int));
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
                // else if(mScores[j]>=max)
                // {
                //     if(mPopulation[j].mGenes[0].mCode.size() >= mPopulation[max_id].mGenes[0].mCode.size()) //save the one with more instructions
                //     {
                //         max = mSupervisorScores[j];
                //         max_id = j;
                //     }
                // }
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
            printf("Supervisor Generation: %d, Max Score: %d, inst %d %d, min %d\n", gen, mScores[mBestID[0]], 
                mPopulation[mBestID[0]].mGenes[0].mCode.size(), mPopulation[mBestID[0]].mGenes.size(), min);
            for(int i = 0;i<mNumSupervisorSurvivors;i++)
            {
                printf("%d ", mScores[mBestID[i]]);
            }   
            printf("\n");

            if(mSavePath!="")
            {
                mPopulation[mBestID[0]].save(mSavePath);
            }
        }
        
        //Reproduce
        for(int i = 0;i<mNumSupervisors;i++)
        {
            if(mSupervisorLastBest[i]==0)
            {
                if (mSupervisorScores[i] < mSupervisorScores[mBestID[mNumSupervisorSurvivors - 1]])
                {
                    int r = rand()%mNumSupervisorSurvivors;
                    mSupervisors[i] = mSupervisors[mBestID[r]];
                    // ELO[i] = ELO[theBest[r]];
                    // ELO[i] = 1500;
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
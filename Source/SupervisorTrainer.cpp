#include "SupervisorTrainer.h"

void SupervisorTrainer::init()
{
	mLoadPath = "breakout_best";
    mSavePath = "breakout_best";

    const int GEN_PRINT_DELAY = 1;

    mNumSupervisors = 10;
    mNumPopulation = 10;
    mNumSurvivors = 1;

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
    mPopulation = new Genotype[mNumPopulation*mNumSupervisors];
    mScores = new int[mNumPopulation*mNumSupervisors];
    mLastBest = new int[mNumPopulation*mNumSupervisors];

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
        mSupervisors[i].mutate();
    }

    mBestID.resize(mNumSurvivors*mNumSupervisors);
}

void SupervisorTrainer::testSupervisor(int num_gen, int sup_id, int sup_children)
{
    const int GEN_PRINT_DELAY = 1;

	// bool running = true;
    for(int gen = 0; gen < num_gen; gen++)
    {
        //Test
        for(int i = 0;i<sup_children;i++)
        {
            int agent_id = sup_children*sup_id + i;

            // if(Scores[i]!=0) continue;
            
            // for(int j = 0;j<20;j++)
            // {
            //     Scores[i] += play_tictactoe(&gPopulation[i], exe, game);
            // }

            mScores[agent_id] = run_atari(*mGame, mExe, &mPopulation[i]);

            printf("Gen %d, Agent %d: Score %d, Inst: %d, Genes: %d\n", gen, agent_id, mScores[i], mPopulation[i].mGenes[0].mCode.size(), mPopulation[i].mGenes.size());
            
            // for(int j = 0;j<NUM_SURVIVORS;j++)
            // {
            //     int r = theBest[j];
            //     int res1 = duel(&gPopulation[i], &gPopulation[r], exe, game);
            //     int res2 = duel(&gPopulation[r], &gPopulation[i], exe, game);
            // 
            //     double qa = pow(10.0,ELO[i]/400.0);
            //     double qb = pow(10.0,ELO[r]/400.0);
            //     double ea = qa/(qa+qb);
            //     double eb = qb/(qa+qb);
            //     double sa=0,sb = 0;
            //     if(res1==15)
            //     {
            //         sa+=0.25;
            //         sb+=0.25;
            //     }
            //     if(res1>15)
            //     {
            //         sa+=0.5;
            //     }
            //     if(res1<15)
            //     {
            //         sb+=0.5;
            //     }
            //     if(res2==15)
            //     {
            //         sa+=0.25;
            //         sb+=0.25;
            //     }
            //     if(res2<15)
            //     {
            //         sa+=0.5;
            //     }
            //     if(res2>15)
            //     {
            //         sb+=0.5;
            //     }
            // 
            //     int diff = abs(ELO[i]-ELO[r]);
            // 
            //     ELO[i] += (sa-ea)*(32+0.1*diff);
            //     ELO[r] += (sb-eb)*(32+0.1*diff);
            // 
            //     Scores[i] += res1;
            //     Scores[i] += 30-res2;
            // }
        }
        
        //Cull
        memset(mLastBest, 0, mNumPopulation*sizeof(int));
        for(int i = 0;i<mNumSurvivors;i++)
        {
            int max = 0;
            int max_id = 0;
            for(int j = 0;j<sup_children;j++)
            {
                int agent_id = sup_children*sup_id + j;
                if (mLastBest[agent_id] == 1)
                {
                    continue;
                }
                if (mScores[agent_id] > max)
                {
                    max = mScores[j];
                    max_id = j;
                }
                else if (mScores[agent_id] >= max)
                {
                    if (mPopulation[agent_id].mGenes[0].mCode.size() >= mPopulation[max_id].mGenes[0].mCode.size()) //save the one with more instructions
                    {
                        max = mScores[agent_id];
                        max_id = agent_id;
                    }
                }
            }
            mBestID[i] = max_id;
            mLastBest[max_id] = 1;
        }
        
        int min = 10000;
        int min_id = 0;
        for (int j = 0; j < sup_children; j++)
        {
            int agent_id = sup_children * sup_id + j;
            if (mLastBest[agent_id] == 1)
            {
                continue;
            }
            if (mScores[agent_id] < min)
            {
                min = mScores[agent_id];
                min_id = agent_id;
            }
        }
        
        if(gen%GEN_PRINT_DELAY==0)
        {
            printf("Generation: %d, Max Score: %d, inst %d %d, min %d\n", gen, mScores[mBestID[0]], 
                mPopulation[mBestID[0]].mGenes[0].mCode.size(), mPopulation[mBestID[0]].mGenes.size(), min);
            for(int i = 0;i<mNumSurvivors;i++)
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
        for(int i = 0;i<sup_children;i++)
        {
            int agent_id = sup_children * sup_id + i;

            if (mLastBest[agent_id] == 0)
            {
                if (mScores[agent_id] < mScores[mBestID[mNumSurvivors - 1]])
                {
                    int r = rand()%mNumSurvivors;
                    mPopulation[agent_id] = mPopulation[mBestID[r]];
                    // ELO[i] = ELO[theBest[r]];
                    // ELO[i] = 1500;
                }

                mPopulation[agent_id].mutate();
            }
            // else if(Scores[i]==0)
            // {
            //     gPopulation[i].mutate();
            // }
            // Scores[i] = 0;
        }
        memset(mScores, 0, mNumPopulation*sizeof(int));
    }
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
            
            mScores[i] = run_atari(*mGame, mExe, &mPopulation[i]);
            
            printf("Gen %d, Agent %d: Score %d, Inst: %d, Genes: %d\n", gen, i, mScores[i], mPopulation[i].mGenes[0].mCode.size(), mPopulation[i].mGenes.size());
        }
        
        //Cull
        memset(mLastBest, 0, mNumPopulation*sizeof(int));
        for(int i = 0;i<mNumSurvivors;i++)
        {
            int max = 0;
            int max_id = 0;
            for(int j = 0;j<mNumSupervisors;j++)
            {
                if(mLastBest[j] == 1)
                {
                    continue;
                }
                if(mScores[j]>max)
                {
                    max = mScores[j];
                    max_id = j;
                }
                else if(mScores[j]>=max)
                {
                    if(mPopulation[j].mGenes[0].mCode.size() >= mPopulation[max_id].mGenes[0].mCode.size()) //save the one with more instructions
                    {
                        max = mScores[j];
                        max_id = j;
                    }
                }
            }
            mBestID[i] = max_id;
            mLastBest[max_id] = 1;
        }
        
        int min = 10000;
        int min_id = 0;
        for(int j = 0;j<mNumPopulation;j++)
        {
            if(mLastBest[j] == 1)
            {
                continue;
            }
            if(mScores[j]<min)
            {
                min = mScores[j];
                min_id = j;
            }
        }
        
        if(gen%GEN_PRINT_DELAY==0)
        {
            printf("Generation: %d, Max Score: %d, inst %d %d, min %d\n", gen, mScores[mBestID[0]], 
                mPopulation[mBestID[0]].mGenes[0].mCode.size(), mPopulation[mBestID[0]].mGenes.size(), min);
            for(int i = 0;i<mNumSurvivors;i++)
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
        for(int i = 0;i<mNumPopulation;i++)
        {
            if(mLastBest[i]==0)
            {
                if(mScores[i]<mScores[mBestID[mNumSurvivors-1]])
                {
                    int r = rand()%mNumSurvivors;
                    mPopulation[i] = mPopulation[mBestID[r]];
                    // ELO[i] = ELO[theBest[r]];
                    // ELO[i] = 1500;
                }
                
                mPopulation[i].mutate();
            }
            // else if(Scores[i]==0)
            // {
            //     gPopulation[i].mutate();
            // }
            // Scores[i] = 0;
        }
        memset(mScores, 0, mNumPopulation*sizeof(int));
    }
}
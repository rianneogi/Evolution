#include "EvolutionTraining.h"

void learn_sup()
{
    const std::string LOAD_FILE = "breakout_best";
    const std::string SAVE_FILE = "breakout_best";

    const int GEN_PRINT_DELAY = 1;

    const int SUP_SIZE = 10;
    const int POP_SIZE = 10;
    const int NUM_SURVIVORS = 1;
    
    AtariGame game("ALE/roms/breakout.bin",123,false);
    // game.mALE->act(PLAYER_A_FIRE);
    ALEState baseState = game.mALE->cloneSystemState();
    const int INPUT_SIZE = game.mALE->getRAM().size();
    const int REG_SIZE = 8;
    printf("INPUT SIZE: %d\n", INPUT_SIZE);
    
    Execution exe;
    exe.mRegisters = new int[REG_SIZE];
    exe.mRegisterSize = REG_SIZE;
    exe.mInputSize = INPUT_SIZE;
    exe.mInputs = game.mALE->getRAM().array();
    exe.reset();
    
    Genotype* gSupervisors = new Genotype[SUP_SIZE];
    Genotype* gPopulation = new Genotype[POP_SIZE*SUP_SIZE];
    int* Scores = new int[POP_SIZE*SUP_SIZE];
    int* LastBest = new int[POP_SIZE*SUP_SIZE];

    //Load
    if(LOAD_FILE!="")
    {
        gPopulation[0].load(LOAD_FILE);
        for(int i = 1;i<POP_SIZE*SUP_SIZE;i++)
        {
            gPopulation[i] = gPopulation[0];
        }
    }
    
    //Init
    for(int i = 1;i<POP_SIZE*SUP_SIZE;i++) //mutate everyone except first
    {
        gPopulation[i].mutate();
        Scores[i] = 0;
        LastBest[i] = 0;
    }
    for(int i = 0;i<SUP_SIZE;i++)
    {
        gSupervisors[i].mutate();
    }
    
    std::vector<int> theBest;
    theBest.resize(NUM_SURVIVORS*SUP_SIZE);
    
    bool running = true;
    for(int gen = 0; running; gen++)
    {
        for(int pop_gen = 0;pop_gen < 10;pop_gen++)
        {
            //Test
            for(int i = 0;i<POP_SIZE*SUP_SIZE;i++)
            {
                // if(Scores[i]!=0) continue;
                
                // for(int j = 0;j<20;j++)
                // {
                //     Scores[i] += play_tictactoe(&gPopulation[i], exe, game);
                // }
                
                Scores[i] = run_atari(game, exe, &gPopulation[i]);
                
                printf("Gen %d, Agent %d: Score %d, Inst: %d, Genes: %d\n", gen, i, Scores[i], gPopulation[i].mGenes[0].mCode.size(), gPopulation[i].mGenes.size());
            }

            memset(LastBest, 0, POP_SIZE*SUP_SIZE*sizeof(int));
            for(int i = 0;i<NUM_SURVIVORS*SUP_SIZE;i++)
            {
                int max = 0;
                int max_id = 0;
                for(int j = 0;j<POP_SIZE;j++)
                {
                    if(LastBest[j] == 1)
                    {
                        continue;
                    }
                    if(Scores[j]>max)
                    {
                        max = Scores[j];
                        max_id = j;
                    }
                    else if(Scores[j]>=max)
                    {
                        if(gPopulation[j].mGenes[0].mCode.size() >= gPopulation[max_id].mGenes[0].mCode.size()) //save the one with more instructions
                        {
                            max = Scores[j];
                            max_id = j;
                        }
                    }
                }
                theBest[i] = max_id;
                LastBest[max_id] = 1;
            }
        }

        memset(LastBest, 0, POP_SIZE*sizeof(int));
        for(int i = 0;i<NUM_SURVIVORS;i++)
        {
            int max = 0;
            int max_id = 0;
            for(int j = 0;j<POP_SIZE;j++)
            {
                if(LastBest[j] == 1)
                {
                    continue;
                }
                if(Scores[j]>max)
                {
                    max = Scores[j];
                    max_id = j;
                }
                else if(Scores[j]>=max)
                {
                    if(gPopulation[j].mGenes[0].mCode.size() >= gPopulation[max_id].mGenes[0].mCode.size()) //save the one with more instructions
                    {
                        max = Scores[j];
                        max_id = j;
                    }
                }
            }
            theBest[i] = max_id;
            LastBest[max_id] = 1;
        }
        
        int min = 10000;
        int min_id = 0;
        for(int j = 0;j<POP_SIZE;j++)
        {
            if(LastBest[j] == 1)
            {
                continue;
            }
            if(Scores[j]<min)
            {
                min = Scores[j];
                min_id = j;
            }
        }
        
        if(gen%GEN_PRINT_DELAY==0)
        {
            printf("Generation: %d, Max Score: %d, inst %d %d, min %d\n", gen, Scores[theBest[0]], 
                gPopulation[theBest[0]].mGenes[0].mCode.size(), gPopulation[theBest[0]].mGenes.size(), min);
            for(int i = 0;i<NUM_SURVIVORS;i++)
            {
                printf("%d ", Scores[theBest[i]]);
            }   
            printf("\n");

            if(SAVE_FILE!="")
            {
                gPopulation[theBest[0]].save(SAVE_FILE);
            }
        }
        
        //Reproduce
        for(int i = 0;i<POP_SIZE;i++)
        {
            if(LastBest[i]==0)
            {
                if(Scores[i]<Scores[theBest[NUM_SURVIVORS-1]])
                {
                    int r = rand()%NUM_SURVIVORS;
                    gPopulation[i] = gPopulation[theBest[r]];
                }
                
                gPopulation[i].mutate();
            }
            // else if(Scores[i]==0)
            // {
            //     gPopulation[i].mutate();
            // }
        }
        memset(Scores, 0, POP_SIZE*sizeof(int));

        // if(gen==1)   break;
    }
    
    delete[] gPopulation;
    delete[] gSupervisors;
}

void learn(int MAX_GEN)
{
    const std::string LOAD_FILE = "seaquest_best_0";
    const std::string SAVE_FILE = "";

    const int GEN_PRINT_DELAY = 1;

    const int POP_SIZE = 10;
    const int NUM_SURVIVORS = 1;
    
    AtariGame game("ALE/roms/seaquest.bin",123,true);
    // TicTacToe game;
    game.mALE->act(PLAYER_A_FIRE);
    ALEState baseState = game.mALE->cloneSystemState();
    // const int REG_SIZE = 1000; 
    // const int INPUT_SIZE = 10;
    const int INPUT_SIZE = game.mALE->getRAM().size();
    const int REG_SIZE = 8;
    printf("INPUT SIZE: %d\n", INPUT_SIZE);
    
    Execution exe;
    exe.mRegisters = new int[REG_SIZE];
    exe.mRegisterSize = REG_SIZE;
    exe.mInputSize = INPUT_SIZE;
    exe.mInputs = game.mALE->getRAM().array();
    exe.reset();
    
    Genotype* gPopulation = new Genotype[POP_SIZE];
    int* Scores = new int[POP_SIZE];
    int* ELO = new int[POP_SIZE];
    int* LastBest = new int[POP_SIZE];

    //Load
    if(LOAD_FILE!="")
    {
        gPopulation[0].load(LOAD_FILE);
        for(int i = 1;i<POP_SIZE;i++)
        {
            gPopulation[i] = gPopulation[0];
        }
    }
    
    //Init
    for(int i = 1;i<POP_SIZE;i++) //mutate everyone except first
    {
        gPopulation[i].mutate();
        Scores[i] = 0;
        LastBest[i] = 0;
        ELO[i] = 1500;
    }
    
    std::vector<int> theBest;
    theBest.resize(NUM_SURVIVORS);
    
    bool running = true;
    for(int gen = 0; gen != MAX_GEN; gen++)
    {
        //Test
        for(int i = 0;i<POP_SIZE;i++)
        {
            // if(Scores[i]!=0) continue;
            
            // for(int j = 0;j<20;j++)
            // {
            //     Scores[i] += play_tictactoe(&gPopulation[i], exe, game);
            // }
            
            Scores[i] = run_atari(game, exe, &gPopulation[i]);
            
            printf("Gen %d, Agent %d: Score %d, Inst: %d, Genes: %d\n", gen, i, Scores[i], gPopulation[i].mGenes[0].mCode.size(), gPopulation[i].mGenes.size());
            
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
        memset(LastBest, 0, POP_SIZE*sizeof(int));
        for(int i = 0;i<NUM_SURVIVORS;i++)
        {
            int max = 0;
            int max_id = 0;
            for(int j = 0;j<POP_SIZE;j++)
            {
                if(LastBest[j] == 1)
                {
                    continue;
                }
                if(Scores[j]>max)
                {
                    max = Scores[j];
                    max_id = j;
                }
                else if(Scores[j]>=max)
                {
                    if(gPopulation[j].mGenes[0].mCode.size() >= gPopulation[max_id].mGenes[0].mCode.size()) //save the one with more instructions
                    {
                        max = Scores[j];
                        max_id = j;
                    }
                }
            }
            theBest[i] = max_id;
            LastBest[max_id] = 1;
        }
        
        int min = 10000;
        int min_id = 0;
        for(int j = 0;j<POP_SIZE;j++)
        {
            if(LastBest[j] == 1)
            {
                continue;
            }
            if(Scores[j]<min)
            {
                min = Scores[j];
                min_id = j;
            }
        }
        
        if(gen%GEN_PRINT_DELAY==0)
        {
            printf("Generation: %d, Max Score: %d, inst %d %d, min %d\n", gen, Scores[theBest[0]], 
                gPopulation[theBest[0]].mGenes[0].mCode.size(), gPopulation[theBest[0]].mGenes.size(), min);
            for(int i = 0;i<NUM_SURVIVORS;i++)
            {
                printf("%d ", Scores[theBest[i]]);
            }   
            printf("\n");

            if(SAVE_FILE!="")
            {
                gPopulation[theBest[0]].save(SAVE_FILE);
            }
        }
        
        //Reproduce
        for(int i = 0;i<POP_SIZE;i++)
        {
            if(LastBest[i]==0)
            {
                if(Scores[i]<Scores[theBest[NUM_SURVIVORS-1]])
                {
                    int r = rand()%NUM_SURVIVORS;
                    gPopulation[i] = gPopulation[theBest[r]];
                    // ELO[i] = ELO[theBest[r]];
                    ELO[i] = 1500;
                }
                
                gPopulation[i].mutate();
            }
            // else if(Scores[i]==0)
            // {
            //     gPopulation[i].mutate();
            // }
            // Scores[i] = 0;
        }
        memset(Scores, 0, POP_SIZE*sizeof(int));
    }

    delete[] gPopulation;
}

void thread_instructions(int i, AtariGame* games, GenericTrainer* trainers, Environment* envs, int mixing_step)
{
    // std::string save_file = "seaquest_best_" + std::to_string(i);

    printf("Thread %d created on cpu %d\n", i, sched_getcpu());

    if(i==0)
        games[i].init("ALE/roms/seaquest.bin", 123, false);
    else
        games[i].init("ALE/roms/seaquest.bin", 123, false);

    envs[i].init(&games[i], trainers[i].mPopulation, 100);
    // trainers[i].mEnv = &(envs[i]);
    // trainers[i].init(10,5,"",save_file,1);
    // trainers[i].mThreadID = i;
    envs[i].mPopulationSize = trainers[i].mNumPopulation;
    envs[i].mPopulation = trainers[i].mPopulation;

    trainers[i].train(mixing_step);
}

void train_parallel(int num_threads, int mixing_step, int pop_per_thread, int survivors_per_thread)
{
    GenericTrainer *trainers = new GenericTrainer[num_threads];
    AtariGame *games = new AtariGame[num_threads];
    Environment *envs = new Environment[num_threads];

    std::thread *threads = new std::thread[num_threads];

    for (int i = 0; i < num_threads;i++)
    {
        std::string save_file = "seaquest_best_" + std::to_string(i);

        if(i==0)
            games[i].init("ALE/roms/seaquest.bin", 123, false);
        else
            games[i].init("ALE/roms/seaquest.bin", 123, false);

        envs[i].init(&games[i], trainers[i].mPopulation, 100);
        trainers[i].mEnv = &(envs[i]);
        trainers[i].init(pop_per_thread,survivors_per_thread,save_file,save_file,1);
        trainers[i].mThreadID = i;
        envs[i].mPopulationSize = trainers[i].mNumPopulation;
        envs[i].mPopulation = trainers[i].mPopulation;
    }
    

    int mixing_counter = 0;
    while(true)
    {
        for (int i = 0; i < num_threads; i++)
        {
            // threads[i] = std::thread(&thread_instructions, i, games, trainers, envs, mixing_step);
            threads[i] = std::thread(&GenericTrainer::train, &trainers[i], mixing_step);
            // threads[i].detach();

            // trainers[i].train(mixing_step);

            // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
            // only CPU i as set.
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            int rc = pthread_setaffinity_np(threads[i].native_handle(),
                                            sizeof(cpu_set_t), &cpuset);
            if (rc != 0) {
                std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
            }
        }
        // trainers[0].train(-1);
        // thread_instructions(0, games, trainers, envs, mixing_step);

        for (int i = 0; i < num_threads;i++)
        {
            threads[i].join();
        }
        // threads[0].join();
        //distribute max individual across all threads
        int *max_id = new int[num_threads];
        int *max = new int[num_threads];
        for (int i = 0; i < num_threads; i++)
        {
            max_id[i] = 0;
            max[i] = trainers[i].mScores[0];
            for (int j = 0; j < trainers[i].mNumPopulation; j++)
            {
                if(trainers[i].mScores[j]>max[i])
                {
                    max_id[i] = j;
                    max[i] = trainers[i].mScores[j];
                }
            }
        }

        for (int i = 0; i < num_threads;i++)
        {
            for (int j = 0; j < num_threads;j++)
            {
                trainers[i].mPopulation[j] = trainers[j].mPopulation[max_id[j]];
            }
        }

        printf("Mixing %d, best scores: ", mixing_counter);
        for (int i = 0; i < num_threads;i++)
        {
            printf("%d, ", max[i]);
        }
        printf("\n");

        mixing_counter++;
    }
}
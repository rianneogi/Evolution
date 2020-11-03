#include "NetTraining.h"

int main()
{
    srand(time(0));

    unsigned num_cpus = std::thread::hardware_concurrency();
    std::cout << "Hardware concurrency: " << num_cpus << " threads\n";

    train_parallel(10,5,100,10);
    // learn(-1);

    // GenericTrainer t;
    // AtariGame game("ALE/roms/seaquest.bin",123,false);
    // Environment *env = new Environment(&game, t.mPopulation, 100);
    // t.mEnv = env;
    // t.init(100, 5, "seaquest_best", "seaquest_best", 1);
    // env->mPopulationSize = t.mNumPopulation;
    // env->mPopulation = t.mPopulation;

    // GenericTrainer t2;
    // AtariGame game2("ALE/roms/seaquest.bin",123,false);
    // Environment *env2 = new Environment(&game2, t2.mPopulation, 100);
    // t2.mEnv = env2;
    // t2.init(100, 5, "seaquest_best", "seaquest_best_2", 1);
    // env2->mPopulationSize = t2.mNumPopulation;
    // env2->mPopulation = t2.mPopulation;

    // t.mPopulation[0].print();
    
    // // std::thread(&t2.train(-1));
    // // std::thread(&GenericTrainer::train, t2, -1).detach();
    // std::thread(&GenericTrainer::train, &t, -1).detach();
    // // t.train(-1);
    // t2.train(-1);

    // SupervisorTrainer t;
    // t.init();
    // t.train();
    
    // learn(-1);
    // test_lua();
    // test_execution();
    // test_save();
    
    // Execution exe;
    // exe.mRegisters = new int[1000];
    // exe.mRegisterSize = 1000;
    // exe.reset();
    // 
    // Genotype indi;
    // indi.load("tictactoe_best");
    // indi.print();
    // int x; std::cin >> x;
    // 
    // play_human(&indi, exe);

    // AtariGame game("ALE/roms/space_invaders.bin",123,false);
    // for(int i = 0;i<100;i++)
    // {
    //     game.restart();
    //     while(!game.is_over())
    //     {
    //         ActionVect vect = game.mALE->getMinimalActionSet();
    //         int size = vect.size();
    //         game.make_move(rand()%size);
    //     }
    //     printf("%d\n", i);
    // }
    
    
    return 0;
}

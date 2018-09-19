#include "Tests.h"

void test_execution()
{
    int* Register = new int[1000];
    Execution exe(Register, 1000, 10);
    TicTacToe game;
    
    Genotype indi;
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 25, 2));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 7, 8));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 8, 6));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 8));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 7));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 6));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction());
        
    indi.print();
    
    int winner = -1;
    
    while(game.is_over()==-1)
    {
        if(game.mTurn==0)
        {
            for(int i = 0;i<9;i++)
            {
                exe.mRegisters[i+1] = game.mState[i];
            }
            exe.run_code(&indi, 0);
            
            // printf("before\n");
            // t.print();
            
            if(exe.mRegisters[0]>=9) exe.mRegisters[0]=8;
            // printf("reg %d %d %d\n",exe.mRegisters[0], t.is_legal(exe.mRegisters[0]), t.mState[0]);
            
            if(game.is_legal(exe.mRegisters[0]))
            {
                game.make_move(exe.mRegisters[0]);
                // printf("after %d\n", t.is_over());
                // t.print();
            }
            else 
            {
                // indi->mutate();
                
                // exe.print(10);
                // indi->print();
                winner = 1;
                break;
                // return 1;
            }
            exe.reset();
        }
        else
        {
            // while(true)
            // {
            // int r = rand()%9;
            // if(t.is_legal(r))
            // {
            //     t.make_move(r);
            //     break;
            // }
            // else 
            // {
            //     return 0;
            // }
            // }
            
            for(int i = 0;i<9;i++)
            {
                if(game.is_legal(i))
                {
                    game.make_move(i);
                    break;
                }
            }
        }
    }
    
    if(winner==1)
    {
        printf("winner\n");
    }
    printf("res: %d\n", game.is_over());
    game.print();
}

void test_save()
{
    Genotype indi;
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 25, 2));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 7, 8));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 8, 6));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 8));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 7));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 6));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction());
    
    indi.mGenes.push_back(Gene());
    
    indi.save("test_indi");
    indi.print();
    
    Genotype indi2;
    
    indi2.load("test_indi");
    
    indi2.print();
}

void test_lua()
{
    Execution exe;
    exe.mRegisterSize = 8;
    exe.mInputSize = 128;
    exe.mInputs = new byte_t[128];
    for(int i = 0;i<128;i++)
    {
        exe.mInputs[i] = 0;
    }

    Genotype indi;
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 25, 2));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 7, 8));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 8, 6));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 8));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 7));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction(INST_SET, 0, 6));
    indi.mGenes[0].mCode.push_back(Instruction(INST_JUMP, 25, 10));
    indi.mGenes[0].mCode.push_back(Instruction());

    exe.compile_lua(&indi, "Lua/simple.lua", 0);
    exe.load_lua("Lua/simple.lua");
    int res = exe.run_lua("Lua/simple.lua", 0);
    printf("res %d\n", res);
    std::cin >> res;

    delete[] exe.mInputs;
}

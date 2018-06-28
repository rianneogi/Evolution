#pragma once

#include "Rating.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

enum MutationType
{
    MUT_CLONE, MUT_ADD, MUT_DEL, MUT_CHANGE, MUT_NUM
};

enum InstructionType
{
    INST_COPY, INST_SET, INST_JUMP, INST_FUNC, INST_COPY_INPUT, INST_JUMP_INPUT, INST_NUM
};

extern std::string gInstructionStrings[];

struct Instruction
{
    int type;
    int arg1;
    int arg2;
    
    Instruction() : type(INST_SET), arg1(0), arg2(rand()%128) {}
    Instruction(int t, int a1, int a2) : type(t), arg1(a1), arg2(a2) {}
    
    void mutate()
    {
        int x = rand()%5;
        if(x==0)
        {
            type = rand()%INST_NUM;
        }
        else if(x==1)
        {
            arg1++;
        }
        else if(x==2)
        {
            arg2++;
        }    
        else if(x==3)
        {
            arg1--;
            if(arg1<0)
            {
                arg1=0;
            }
        }
        else if(x==4)
        {
            arg2--;
            if(arg2<0)
            {
                arg2=0;
            }
        }
    }
    
    void print() const
    {
        printf("%s %d %d\n", gInstructionStrings[type].c_str(), arg1, arg2);
    }
};

class Gene
{
public:
    std::vector<Instruction> mCode;
    
    Gene();
    ~Gene();
    
    void mutate(int type);
    
    void print() const
    {
        for(int i = 0;i<mCode.size();i++)
        {
            printf("    Inst %d: ", i);
            mCode[i].print();
        }
    }
};

#include "Gene.h"

std::string gInstructionStrings[] = {"COPY", "SET", "JMP", "CALL", "COPY_IP", "JUMP_IP"};

Gene::Gene()
{
    mCode.push_back(Instruction());
    mCode[0].type = INST_SET;
    mCode[0].arg1 = 0;
    mCode[0].arg2 = rand()%128;
}

Gene::~Gene()
{
}

void Gene::mutate(int type)
{
    if(type==MUT_DEL)
    {
        if(mCode.size() > 1)
        {
            mCode.erase(mCode.begin()+(rand()%mCode.size()));
        }
    }
    else if(type==MUT_ADD)
    {
        mCode.insert(mCode.begin()+(rand()%mCode.size()), Instruction(rand()%INST_NUM,rand()%128,rand()%128));
    }
    else if(type==MUT_CHANGE)
    {
        mCode[rand()%mCode.size()].mutate();
    }
}

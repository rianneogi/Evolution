#include "Execution.h"

void Execution::run_code(const Genotype *indi, int code_id)
{
    // assert(code_id < indi->mGenes.size());
    if(code_id>=indi->mGenes.size())
    {
        return;
    }
    assert(mRegisters!=NULL);
    
    int type,arg1,arg2;
    for(int i = 0;i<indi->mGenes[code_id].mCode.size();i++)
    {
        // printf("executing instruction %d\n", i);
        
        mCurrentStep++;
        if(mCurrentStep>=mMaxSteps)
        {
            return;
        }
        
        type = indi->mGenes[code_id].mCode[i].type;
        arg1 = indi->mGenes[code_id].mCode[i].arg1;
        arg2 = indi->mGenes[code_id].mCode[i].arg2;
        
        assert(arg1 >= 0 && arg2 >= 0 && type >= 0 && type < INST_NUM);
        
        if(type==INST_SET)
        {
            arg1 += mInputSize;
            if(arg1 >= mRegisterSize)
            {
                arg1 = mRegisterSize-1;
            }
            mRegisters[arg1] = arg2;
        }
        else if(type==INST_COPY)
        {
            arg2 += mInputSize;
            if(arg1 >= mRegisterSize)
            {
                arg1 = mRegisterSize-1;
            }
            if(arg2 >= mRegisterSize)
            {
                arg2 = mRegisterSize-1;
            }
            mRegisters[arg1] = mRegisters[arg2];
        }
        else if(type==INST_JUMP)
        {
            if(arg1 >= mRegisterSize)
            {
                arg1 = mRegisterSize-1;
            }
            if(mRegisters[arg1]==2)
            {
                i = arg2-1;
            }
        }
        else if(type==INST_FUNC)
        {
            if(arg1 >= indi->mGenes[code_id].mCode.size())
            {
                arg1 = indi->mGenes[code_id].mCode.size()-1;
            }
            run_code(indi, arg1);
        }
        else if(type==INST_COPY_INPUT)
        {
            if(arg1 >= mInputSize)
            {
                arg1 = mInputSize-1;
            }
            if(arg2 >= mRegisterSize)
            {
                arg2 = mRegisterSize-1;
            }
            mRegisters[arg1] = mInputs[arg2];
        }
        else if(type==INST_JUMP_INPUT)
        {
            if(arg1 >= mInputSize)
            {
                arg1 = mInputSize-1;
            }
            if(mInputs[arg1]==2)
            {
                i = arg2-1;
            }
        }
    }
}

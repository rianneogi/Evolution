#pragma once

#include "Genotype.h"
#include <string.h>

extern "C"
{
	#include "lua5.3/lua.h"
	#include "lua5.3/lualib.h"
	#include "lua5.3/lauxlib.h"
}

class Execution
{
  public:
    int *mRegisters;
    byte_t *mInputs;
    int mRegisterSize;
    int mCurrentStep;
    int mInputSize;
    int mMaxSteps;

    lua_State* mLua;
    
    Execution() : mRegisters(NULL), mInputs(NULL), mRegisterSize(0), mCurrentStep(0), mMaxSteps(100) {}
    Execution(int *reg, int size, int input_size)
        : mRegisters(reg), mRegisterSize(size), mCurrentStep(0), mInputSize(input_size), mMaxSteps(100) {}
    Execution(int *reg, int size, int input_size, int max_steps)
        : mRegisters(reg), mRegisterSize(size), mCurrentStep(0), mInputSize(input_size), mMaxSteps(max_steps) {}
    ~Execution(){};

    void run_code(const Genotype *indi, int code_id);
    void compile_lua(const Genotype* indi, const std::string& path, int id);
    void run_lua(const std::string& path, int id);

    void reset()
    {
        memset(mRegisters, 0, mRegisterSize * sizeof(int));
        // for(int i = 0;i<mRegisterSize;i++)
        // {
        //     mRegisters[i] = 0;
        // }
        mCurrentStep = 0;
    }

    void resetStep()
    {
        mCurrentStep = 0;
    }

    void print(int num_registers) const
    {
        assert(num_registers <= mRegisterSize);
        for (int i = 0; i < num_registers; i++)
        {
            printf("%d ", mRegisters[i]);
        }
        printf("\n");
    }
};

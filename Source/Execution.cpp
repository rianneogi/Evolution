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
            // arg1 += mInputSize;
            arg1 = arg1%mRegisterSize;
            // if(arg1 >= mRegisterSize)
            // {
            //     arg1 = mRegisterSize-1;
            // }
            mRegisters[arg1] = arg2;
        }
        else if(type==INST_COPY)
        {
            // arg2 += mInputSize;
            arg1 = arg1%mRegisterSize;
            arg2 = arg2%mRegisterSize;

            // if(arg1 >= mRegisterSize)
            // {
            //     arg1 = mRegisterSize-1;
            // }
            // if(arg2 >= mRegisterSize)
            // {
            //     arg2 = mRegisterSize-1;
            // }
            mRegisters[arg1] = mRegisters[arg2];
        }
        else if(type==INST_JUMP)
        {
            arg1 = arg1%mRegisterSize;
            // if(arg1 >= mRegisterSize)
            // {
            //     arg1 = mRegisterSize-1;
            // }
            if(mRegisters[arg1]==2)
            {
                i = arg2-1;
            }
        }
        else if(type==INST_FUNC)
        {
            // arg1 = arg1%indi->mGenes[code_id].mCode.size();
            if(arg1 >= indi->mGenes[code_id].mCode.size())
            {
                arg1 = indi->mGenes[code_id].mCode.size()-1;
            }
            run_code(indi, arg1);
        }
        else if(type==INST_COPY_INPUT)
        {
            arg1 = arg1%mRegisterSize;
            arg2 = arg2%mInputSize;
            // if(arg2 >= mInputSize)
            // {
            //     arg2 = mInputSize-1;
            // }
            // if(arg1 >= mRegisterSize)
            // {
            //     arg1 = mRegisterSize-1;
            // }
            mRegisters[arg1] = mInputs[arg2];
        }
        else if(type==INST_JUMP_INPUT)
        {
            arg1 = arg1%mInputSize;
            // if(arg1 >= mInputSize)
            // {
            //     arg1 = mInputSize-1;
            // }
            if(mInputs[arg1]==2)
            {
                i = arg2-1;
            }
        }
    }
}

void Execution::compile_lua(const Genotype* indi, const std::string& path, int id)
{
    std::fstream file;
    file.open(path, std::ios::out | std::ios::trunc);
    
    if(!file.is_open())
    {
        printf("Unable to open lua file %s\n", path.c_str());
    }

    for(int i = 0;i<indi->mGenes.size();i++)
    {
        for(int j = 0;j<indi->mGenes[i].mCode.size();j++)
        {
            
        }
    }

    // printf("Loading Cards...\n");
	mLua = luaL_newstate(); //create new lua state
	luaL_openlibs(mLua);

	//lua_register(LuaCards, "loadcard", loadcard); //register loadcard
	// registerLua(mLua); //register functions

	if (luaL_loadfile(mLua, path.c_str()) || lua_pcall(mLua, 0, 0, 0))
	{
		printf("Error: failed to load %s\n", path.c_str());
		printf("%s\n", lua_tostring(mLua, -1));
		// _getch();
		return;
	}
}

void Execution::run_lua(const std::string& path, int id)
{

}

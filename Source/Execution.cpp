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
    int type,arg1,arg2;

    file << "Indi" + std::to_string(id) << " = {\n";
    for(int i = 0;i<indi->mGenes.size();i++)
    {
        file << "Code" + std::to_string(i) + " = function(Inputs)\n";
        file << "Registers = {}\n";
        for(int j = 0;j<indi->mGenes[i].mCode.size();j++)
        {
            type = indi->mGenes[id].mCode[j].type;
            arg1 = indi->mGenes[id].mCode[j].arg1;
            arg2 = indi->mGenes[id].mCode[j].arg2;
            
            assert(arg1 >= 0 && arg2 >= 0 && type >= 0 && type < INST_NUM);
            
            if(type==INST_SET)
            {
                arg1 = arg1%mRegisterSize;
                file << "Registers["+std::to_string(arg1)+"] = "+std::to_string(arg2)+"\n";
            }
            else if(type==INST_COPY)
            {
                arg1 = arg1%mRegisterSize;
                arg2 = arg2%mRegisterSize;

                file << "Registers["+std::to_string(arg1)+"] = Registers["+std::to_string(arg2)+"]\n";
            }
            else if(type==INST_JUMP)
            {
                // arg1 = arg1%mRegisterSize;
                // if(mRegisters[arg1]==2)
                // {
                //     i = arg2-1;
                // }
            }
            else if(type==INST_FUNC)
            {
                // arg1 = arg1%indi->mGenes[code_id].mCode.size();
                // if(arg1 >= indi->mGenes[code_id].mCode.size())
                // {
                //     arg1 = indi->mGenes[code_id].mCode.size()-1;
                // }
                // run_code(indi, arg1);
            }
            else if(type==INST_COPY_INPUT)
            {
                arg1 = arg1%mRegisterSize;
                arg2 = arg2%mInputSize;                
                file << "Registers["+std::to_string(arg1)+"] = Inputs["+std::to_string(arg2)+"]\n";
            }
            else if(type==INST_JUMP_INPUT)
            {
                // arg1 = arg1%mInputSize;
                // // if(arg1 >= mInputSize)
                // // {
                // //     arg1 = mInputSize-1;
                // // }
                // if(mInputs[arg1]==2)
                // {
                //     i = arg2-1;
                // }
            }
        }
        file << "return Registers[0]\n";
        if(i == indi->mGenes.size()-1)
        {
            file << "end\n";
        }
        else
        {
            file << "end,\n";
        }
    }
    file << "}\n";

    file.close();
}

void Execution::load_lua(const std::string& path)
{
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

void luaPushArray(lua_State* lua, byte_t* array, int size)
{
    lua_newtable(lua);
    for(int i = 0;i<size;i++)
    {
        lua_pushstring(lua, std::to_string(i).c_str());
        lua_rawseti(lua, -2, array[i]);
    }
}

int Execution::run_lua(const std::string& path, int id)
{
    lua_getglobal(mLua, ("Indi"+std::to_string(id)).c_str());
	lua_getfield(mLua, -1, ("Gene"+std::to_string(0)).c_str());
	// lua_getfield(LuaCards, -1, "OnCast");
	// lua_pushlightuserdata(mLua, mInputs);
    // lua_pushlightuserdata(mLua, mRegisters);
    luaPushArray(mLua, mInputs, mInputSize);
    // luaPushArray(mLua, mRegisters, mRegisterSize);
	lua_pcall(mLua, 1, 1, 0);
    int res = lua_tointeger(mLua, -1);
    lua_pop(mLua, 1);
	lua_pop(mLua, 1);
	lua_pop(mLua, 1);
    return res;
}

#include "Genotype.h"
#include <fstream>

Genotype::Genotype()
{
    mGenes.push_back(Gene());
}

Genotype::~Genotype()
{
    
}

void Genotype::mutate()
{
    if(rand()%100 < 5)
    {
        mGenes.push_back(Gene(mGenes[rand()%mGenes.size()]));
    }
    for(int i = 0;i<mGenes.size();i++)
    {
        int x = rand()%MUT_NUM;
        mGenes[i].mutate(x);    
    }
}

void Genotype::save(const std::string &path) const
{
    std::fstream file;
    file.open(path, std::ios::out | std::ios::trunc);
    
    if(file.is_open())
    {
        file << mGenes.size() << "\n";
        for(int i = 0;i<mGenes.size();i++)
        {
            file << mGenes[i].mCode.size() << "\n";
            for(int j = 0;j<mGenes[i].mCode.size();j++)
            {
                Instruction inst = mGenes[i].mCode[j];
                file << inst.type << "\n" << inst.arg1 << "\n" <<inst.arg2 << "\n";
            }
        }
    }
    else 
    {
        printf("Unable to open file %s for saving\n", path.c_str());
    }
    
    file.close();
}

void Genotype::load(const std::string &path)
{
    std::fstream file;
    file.open(path, std::ios::in);
    
    mGenes.clear();
    std::string s;
    if(file.is_open())
    {
        size_t num_genes = 0, num_inst = 0;
        // file >> num_genes;
        getline(file,s);
        num_genes = atoi(s.c_str());
        // printf("genes %d\n", num_genes);
        for(int i = 0;i<num_genes;i++)
        {
            mGenes.push_back(Gene());
            getline(file,s);
            num_inst = atoi(s.c_str());
            // file >> num_inst;
            // printf("inst %d\n", num_inst);
            mGenes[i].mCode.resize(num_inst);
            for(int j = 0;j<num_inst;j++)
            {
                Instruction inst;
                getline(file,s);
                inst.type = atoi(s.c_str());
                if(inst.type>=INST_NUM)
                {
                    printf("ERROR: Loading bad instruction...\n");
                }
                getline(file,s);
                inst.arg1 = atoi(s.c_str());
                getline(file,s);
                inst.arg2 = atoi(s.c_str());

                // file >> inst.type >> inst.arg1 >> inst.arg2;
                mGenes[i].mCode[j] = inst;
            }
        }
    }
    else 
    {
        printf("Unable to open file %s for loading\n", path.c_str());
    }
    
    file.close();
}

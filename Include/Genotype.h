#pragma once

#include "Gene.h"

class Genotype
{
public:
    std::vector<Gene> mGenes;
    
    Genotype();
    ~Genotype();
    
    void mutate();
    
    void save(const std::string& path) const;
    void load(const std::string& path);
    
    void print() const
    {
        for(int i = 0;i<mGenes.size();i++)
        {
            printf("Gene %d:\n", i);
            mGenes[i].print();
        }
    }
};

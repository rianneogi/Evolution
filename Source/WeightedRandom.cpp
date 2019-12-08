#include "WeightedRandom.h"

WeightedRandom::WeightedRandom(const std::vector<int>& weights) : mWeights(weights) 
{ 
    calculateThresholds();
}

int WeightedRandom::getInt() const
{
    int r = rand()%mThresholds[mThresholds.size()-1];
    
    for(int i = 0;i<mThresholds.size();i++)
    {
        if(r < mThresholds[i])
        {
            return i;
        }
    }
    
    return -1;
}

void WeightedRandom::calculateThresholds()
{
    int sum = 0;
    for(int i = 0;i<mWeights.size();i++)
    {
        sum += mWeights[i];
        mThresholds[i] = sum;
    }
}

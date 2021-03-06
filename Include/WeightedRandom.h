#pragma once

#include <NNInclude.h>

#include <ale_interface.hpp>
// #include <random>
#include <vector>
#include <string>

class WeightedRandom
{
public:
    std::vector<int> mWeights;
    std::vector<int> mThresholds;
    
    WeightedRandom() {}
    WeightedRandom(const std::vector<int>& weights);
    ~WeightedRandom() {};
    
    void calculateThresholds();
    
    int getInt() const;
};

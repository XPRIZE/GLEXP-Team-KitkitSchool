//
//  Random.h -- Random number generation
//  TodoSchool on Aug 8, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <unordered_set>
#include <random>
#include <cmath>


namespace todoschool {

class Random {
    std::random_device rd;
    std::mt19937 gen;

public:
    static Random& shared();
    Random();
    
    template<typename T>
    T sample(const T& population, size_t k) {
        // NB(xenosoz, 2016): See also https://svn.python.org/projects/python/tags/r32/Lib/random.py
        
        size_t n = population.size();
        size_t setsize = 21;

        if (k > 5)
            setsize += (size_t)(std::pow(4, std::ceil(std::log(k*3.f) / std::log(4.f))));

        if (n <= setsize)
            return sample_by_pool(population, k);

        return sample_by_index(population, k);
    }

    template <typename T>
    T sample_by_pool(const T& population, size_t k)
    {
        // NB(xenosoz, 2016): See also https://svn.python.org/projects/python/tags/r32/Lib/random.py

        T result;
        T pool = population;
        size_t n = population.size();
        
        for (size_t i = 0; i < k; ++i) {
            std::uniform_int_distribution<> dice(0, (int)(n-i-1));
            size_t j = dice(gen);
            result.push_back(pool[j]);
            std::swap(pool[j], pool[n-i-1]);
        }
        
        return result;
    }
    
    template <typename T>
    T sample_by_index(const T& population, size_t k)
    {
        // NB(xenosoz, 2016): See also https://svn.python.org/projects/python/tags/r32/Lib/random.py

        T result;
        std::unordered_set<size_t> selected;
        size_t n = population.size();

        for (size_t i = 0; i < k; ++i) {
            std::uniform_int_distribution<> dice(0, (int)(n-1));
            size_t j = dice(gen);
            while (selected.find(j) != selected.end()) {
                j = dice(gen);
            }
            selected.insert(j);
            result.push_back(population[j]);
        }
        
        return result;
    }

    template <typename RandomAccessIterator>
    void shuffle(RandomAccessIterator begin, RandomAccessIterator end)
    {
        // NB(xenosoz, 2016): See also https://svn.python.org/projects/python/tags/r32/Lib/random.py
        auto size = end - begin;

        for (size_t i = size; i --> 1; /* empty */) {
            std::uniform_int_distribution<> dice(0, (int)i);
            size_t j = dice(gen);
            std::swap(*(begin + i), *(begin + j));
        }
    }

    template <typename T>
    void shuffle(T& x)
    {
        shuffle(x.begin(), x.end());
    }
};
    

template<typename T>
T sample(const T& population, size_t k) {
    return Random::shared().sample(population, k);
}

template <typename T>
T sample_by_pool(const T& population, size_t k) {
    return Random::shared().sample_by_pool(population, k);
}

template <typename T>
T sample_by_index(const T& population, size_t k) {
    return Random::shared().sample_by_index(population, k);
}

template <typename RandomAccessIterator>
void shuffle(RandomAccessIterator begin, RandomAccessIterator end) {
    return Random::shared().shuffle(begin, end);
}

template <typename T>
void shuffle(T& x) {
    Random::shared().shuffle(x);
}

}  // namespace todoschool

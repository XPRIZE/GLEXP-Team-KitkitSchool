//
//  FunctionVector.h -- Callable vector
//  TodoSchool on Aug 17, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "TaggedFunction.h"
#include <vector>
#include <algorithm>
#include <functional>



namespace __impl {

template<typename>
class FunctionVectorBase;

template<typename R, typename... ArgTypes>
class FunctionVectorBase<R(ArgTypes...)>
{
public:
    typedef TaggedFunction<R(ArgTypes...)> FnType;

protected:
    std::vector<FnType> Storage;
    
public:
    void clear() { Storage.clear(); }

    explicit operator bool() const { return !empty(); }
    bool empty() const { return Storage.empty(); }
    size_t size() const { return Storage.size(); }
    
    void push_back(FnType Fn) { Storage.push_back(Fn); }
    
    template<typename... FnArgTypes>
    void emplace_back(FnArgTypes... Args) { Storage.emplace_back(Args...); }
    
    // NB(xenosoz, 2016): Remove first occurence of the value.
    void remove(const FnType& Fn) {
        auto It = std::find(Storage.begin(), Storage.end(), Fn);
        if (It == Storage.end()) {
            // XXX: Should we throw something here?
            return;
        }
        Storage.erase(It);
    }
};

}  // namespace __impl


template<typename>
class FunctionVector;

template<typename R, typename... ArgTypes>
class FunctionVector<R(ArgTypes...)>
    : public __impl::FunctionVectorBase<R(ArgTypes...)>
{
public:
    std::vector<R> operator()(ArgTypes&&... Args) const {
        std::vector<R> Rets;
        for (auto& Fn : this->Storage) {
            Rets.push_back(Fn(std::forward<ArgTypes>(Args)...));
        }
        return Rets;
    }
    std::vector<R> operator()(const ArgTypes&... Args) const {
        std::vector<R> Rets;
        for (auto& Fn : this->Storage) {
            Rets.push_back(Fn(Args...));
        }
        return Rets;
    }
};


template<typename... ArgTypes>
class FunctionVector<void(ArgTypes...)>
    : public __impl::FunctionVectorBase<void(ArgTypes...)>
{
public:
    void operator()(ArgTypes&&... Args) const {
        for (auto& Fn : this->Storage) {
            Fn(std::forward<ArgTypes>(Args)...);
        }
    }
    void operator()(const ArgTypes&... Args) const {
        for (auto& Fn : this->Storage) {
            Fn(Args...);
        }
    }
};


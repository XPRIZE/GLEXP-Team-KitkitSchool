//
//  TaggedFunction.h -- Compariable function
//  TodoSchool on Aug 17, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Basic/UniqueLocalValue.h"
#include <string>
#include <functional>


namespace todoschool {

template<typename> class TaggedFunction;

template<typename R, typename... ArgTypes>
class TaggedFunction<R(ArgTypes...)> {
public:
    typedef std::function<R(ArgTypes...)> FnType;

private:
    UniqueLocalValue Tag;
    FnType Function;

public:
    TaggedFunction(): Tag() {}
    TaggedFunction(FnType&& F): TaggedFunction() { Function = F; }
    TaggedFunction(const FnType& F): TaggedFunction() { Function = F; }
    
    TaggedFunction& operator=(const FnType& F) {
        Tag.reset();
        Function = F;
        return *this;
    }
    TaggedFunction& operator=(const TaggedFunction& TF) {
        Tag = TF.Tag;
        Function = TF.Function;
        return *this;
    }
    
    explicit operator bool() const { return bool(Function); }
    
    bool operator==(const TaggedFunction& TF) const {
        return Tag == TF.Tag;
    }
    bool operator!=(const TaggedFunction& TF) const {
        return Tag != TF.Tag;
    }
    
    R operator()(ArgTypes&&... Args) const {
        return Function(std::forward<ArgTypes>(Args)...);
    }
    R operator()(const ArgTypes&... Args) const {
        return Function(Args...);
    }

    UniqueLocalValue tag() const { return Tag; }
    FnType function() const { return Function; }
};

}  // namespace todoschool
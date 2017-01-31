//
//  CreateFunc.h -- New style CREATE_FUNC macro that accepts variable parameters.
//  TodoSchool on Oct 31, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


// NB(xenosoz, 2016): Modify CREATE_FUNC to accept __TYPE__::create(x, y, z ...).
//   It calls __TYPE__::init(x, y, z ...) internally.
#ifdef CREATE_FUNC
#undef CREATE_FUNC
#endif

#define CREATE_FUNC(__TYPE__) \
    template <typename... ArgTypes> \
    static __TYPE__* create(ArgTypes&&... Args) \
    { \
        __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
        if (pRet && pRet->init(std::forward<ArgTypes>(Args)...)) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = nullptr; \
            return nullptr; \
        } \
    }

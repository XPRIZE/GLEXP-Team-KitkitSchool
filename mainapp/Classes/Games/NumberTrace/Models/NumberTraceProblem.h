//
//  Problem.h -- A problem for NumberTrace
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/NumberTraceNS.h"


BEGIN_NS_NUMBERTRACE

class Problem {
public:
    enum class AssetType {
        Ant,
        Bee,
        Beetle,
        BlueButterfly,
        Cockroach,
        Ladybug,
        Moth,
        Spider,
        StagBeetle,
        YellowButterfly
    };
    AssetType TheAssetType;
    size_t AssetCount;
    string TraceText;

public:
    Problem();
    Problem(AssetType TheAssetType,
            size_t AssetCount,
            const string& TraceText);

public:
    static Problem fromInputStream(istream& IS);
    friend istream& operator>>(istream& IS, Problem& P);
};

END_NS_NUMBERTRACE

//
//  Problem.cpp -- A problem for NumberTrace
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "NumberTraceProblem.h"
#include "../Utils/NumberTraceMainDepot.h"


BEGIN_NS_NUMBERTRACE

Problem::Problem() {
}

Problem::Problem(AssetType TheAssetType,
                 size_t AssetCount,
                 const string& TraceText)
: TheAssetType(TheAssetType)
, AssetCount(AssetCount)
, TraceText(TraceText)
{
}

Problem Problem::fromInputStream(istream &IS) {
    string AssetTypeString;
    size_t AssetCount;
    if (IS >> AssetTypeString >> AssetCount) {
        auto TheAssetType = MainDepot().assetTypeForString(AssetTypeString);
        auto TraceText = itos((int)AssetCount);
        return Problem(TheAssetType, AssetCount, TraceText);
    }
    
    return Problem();
}

istream& operator>>(istream& IS, Problem& P) {
    P = Problem::fromInputStream(IS);
    return IS;
}

END_NS_NUMBERTRACE

//
//  WoodenPuzzleUtil.cpp on Jun 8, 2018
//  TodoSchool
//
//  Copyright (c) 2018 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzleUtil.h"
#include "WoodenPuzzleNS.h"
#include "Common/Basic/DeviceSpec.h"
#include <Managers/LanguageManager.hpp>
#include "Common/Sounds/CommonSound.hpp"
#include <string>


BEGIN_NS_WOODENPUZZLES;

Point WoodenPuzzleUtil::wallBiasedRandomPoint(const Rect& Region) const {
    // NB(xenosoz, 2016): Generate a point in the region.
    Point A(random(Region.minX(), Region.maxX()),
            random(Region.minY(), Region.maxY()));
    
    // NB(xenosoz, 2016): Get wall-side point.
    float W = (Region.maxX() - Region.minX()) / 2.f;
    float H = (Region.maxY() - Region.minY()) / 2.f;
    
    float T = atan2f(A.x - Region.midX(), A.y - Region.midY());
    float InvR = max(std::abs(cos(T)) / W, std::abs(sin(T)) / H);
    float R = 1.f / InvR;
    Point B(R * cos(T) + Region.midX(), R * sin(T) + Region.midY());
    
    // NB(xenosoz, 2016): Push point A toward point B.
    float Alpha = 0.90f;
    Point C = (1.f - Alpha) * A + Alpha * B;
    
    return C;
}

float WoodenPuzzleUtil::costOfNodePositions(const vector<Node*>& FlexibleNodes,
                                            Rect BoardBB) const
{
    float Cost = 0.f;
    
    for (size_t AI = 0, AE = FlexibleNodes.size(); AI < AE; ++AI) {
        auto A = FlexibleNodes[AI];
        
        Rect NodeBB = A->getBoundingBox();
        bool BadOnBoard = (BoardBB.intersection(NodeBB).area() >= NodeBB.area() * 0.60f);
        float Multiplier = (BadOnBoard ? 1e10 : 1.f);
        
        for (size_t BI = AI + 1, BE = FlexibleNodes.size(); BI < BE; ++BI) {
            auto B = FlexibleNodes[BI];
            float L = (A->getPosition() - B->getPosition()).length();
            Cost += pow((1.f/L), 10.f) * Multiplier;
        }
    }
    
    return Cost;
}

void WoodenPuzzleUtil::refineNodePositions(const vector<Node*>& FlexibleNodes,
                                           Rect BoardBB,
                                           Size RegionSize) const
{
    // NB(xenosoz, 2016): Quick and dirty implementation of refining node positions.
    
    for (auto N : FlexibleNodes) {
        // NB(xenosoz, 2016): Try to relax randomly.
        Size NodeSize = N->getContentSize();
        Rect Boundary(NodeSize / 2.f, RegionSize - NodeSize);
        
        Point BestPoint = N->getPosition();
        float BestCost = costOfNodePositions(FlexibleNodes, BoardBB);
        
        for (int J = 0; J < 20; ++J) {
            Point NewPoint = wallBiasedRandomPoint(Boundary);
            N->setPosition(NewPoint);
            float NewCost = costOfNodePositions(FlexibleNodes, BoardBB);
            
            if (BestCost >= NewCost) {
                BestPoint = NewPoint;
                BestCost = NewCost;
            }
        }
        
        N->setPosition(BestPoint);
    }
}

END_NS_WOODENPUZZLES;

//
//  SMCardSlotBuilder.cpp on Aug 8, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SMCardSlotBuilder.hpp"
#include "Utils/Random.h"
#include <limits>

using namespace cocos2d;
using namespace std;
using todoschool::sample;

namespace shapematching {

vector<Point> SMCardSlotBuilder::pointsInBoundary(size_t cardCount,
                                                Rect boardRect,
                                                Size cardSize)
{
    auto scoreFn = [](const vector<Point>& points) {
        float score = 0.f;
        for (auto A : points) {
            for (auto B : points) {
                if (A == B) { continue; }
                float dist = A.distance(B);
                
                score += dist * dist;
            }
        }
        return score;
    };
    
    
    auto candPoints = candidatePointsInBoundary(cardCount, boardRect, cardSize);
    
    vector<Point> maxPoints;
    float maxScore = -numeric_limits<float>::infinity();
    for (size_t i = 0; i < 20; ++i) {
        auto points = sample(candPoints, cardCount);
        auto score = scoreFn(points);
        
        if (score >= maxScore) {
            maxPoints = points;
            maxScore = score;
        }
    }
    
    return maxPoints;
}

vector<Point> SMCardSlotBuilder::candidatePointsInBoundary(size_t cardCount,
                                                         Rect boardRect,
                                                         Size cardSize)
{
    // NB(xenosoz, 2016): Create grid first and choose points to reduce collisions.
    
    float rhoX = 1.40f;
    float rhoY = 1.20f;
    float shakeRate = (1.f/4.f) / 2.f;
    
    vector<Point> points;
    for (float x = boardRect.getMinX() + cardSize.width * (shakeRate + .5f);
         x < boardRect.getMaxX() - cardSize.width * (shakeRate + .5f);
         x += cardSize.width / rhoX)
    {
        for (float y = boardRect.getMinY() + cardSize.height * (shakeRate + .5f);
             y < boardRect.getMaxY() - cardSize.height * (shakeRate + .5f);
             y += cardSize.height / rhoY)
        {
            float shakeX = random(-cardSize.width * shakeRate,
                                  +cardSize.width * shakeRate);
            float shakeY = random(-cardSize.height * shakeRate,
                                  +cardSize.height * shakeRate);
            Point p(x + shakeX, y + shakeY);
            points.push_back(p);
        }
    }
    
    while (points.size() < cardCount) {
        float x = random(boardRect.getMinX() + (cardSize.width / 2),
                         boardRect.getMaxX() - (cardSize.width / 2));
        float y = random(boardRect.getMinY() + (cardSize.height / 2),
                         boardRect.getMaxY() - (cardSize.height / 2));
        points.push_back(Point(x, y));
    }
    
    return points;
}

}

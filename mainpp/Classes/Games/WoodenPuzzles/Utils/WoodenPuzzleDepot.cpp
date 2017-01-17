//
//  WoodenPuzzleDepot.cpp on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzleDepot.h"
#include "WoodenPuzzleNS.h"
#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <Managers/LanguageManager.hpp>
#include <Common/Sounds/AdultVoice_enUS.h>
#include <Common/Sounds/AdultVoice_swTZ.h>
#include <Common/Sounds/Pam_enUS.h>
#include <Common/Sounds/Imma_swTZ.h>
#include <string>

BEGIN_NS_WOODENPUZZLES;


string WoodenPuzzleDepot::assetPrefix() const {
    return "WoodenPuzzles";
}

string WoodenPuzzleDepot::defaultFont() const {
    return "fonts/TodoSchoolV2.ttf";
}

Size WoodenPuzzleDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size WoodenPuzzleDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Size WoodenPuzzleDepot::cardSize() const {
    return Size(504.f, 672.f);
}

float WoodenPuzzleDepot::scaleToCover(Size FixedSize, Size SubjectSize) const {
    float Scale = max(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float WoodenPuzzleDepot::scaleToBeContained(Size FixedSize, Size SubjectSize) const {
    float Scale = min(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float WoodenPuzzleDepot::scaleToCoverWindow(Size SubjectSize) const {
    return scaleToCover(windowSize(), SubjectSize);
}

float WoodenPuzzleDepot::scaleToBeContainedInWindow(Size SubjectSize) const {
    return scaleToBeContained(windowSize(), SubjectSize);
}

Point WoodenPuzzleDepot::wallBiasedRandomPoint(const Rect& Region) const {
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

void WoodenPuzzleDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

float WoodenPuzzleDepot::costOfNodePositions(const vector<Node*>& FlexibleNodes,
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

SoundEffect WoodenPuzzleDepot::soundForCardinalNumber(int N) const {
    if (LanguageManager::getInstance()->isSwahili())
        return AdultVoice_swTZ().cardinalNumber(N).preloaded();
    
    return AdultVoice_enUS().cardinalNumber(N).preloaded();
}

SoundEffect WoodenPuzzleDepot::soundForLetter(const string& L) const {
    if (LanguageManager::getInstance()->isSwahili())
        return Imma_swTZ().soundForLetterName(L).preloaded();
    
    return Pam_enUS().soundForLetterName(L).preloaded();
}

void WoodenPuzzleDepot::refineNodePositions(const vector<Node*>& FlexibleNodes,
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

SoundEffect WoodenPuzzleDepot::soundForCardBirth() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_birth.m4a");
}



SoundEffect WoodenPuzzleDepot::soundForCardHit(int N /* = -1 */) const {
    if (N < 0)
        N = random(0, 5);
    
    return SoundEffect(assetPrefix() + StringUtils::format("/Sounds/card_hit.%d.m4a", N));
}

SoundEffect WoodenPuzzleDepot::soundForCardMiss() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_miss.m4a");
}


vector<SoundEffect> WoodenPuzzleDepot::soundEffectsToPreload() const {
    return {
        soundForCardBirth(),
        soundForCardHit(0),
        soundForCardHit(1),
        soundForCardHit(2),
        soundForCardHit(3),
        soundForCardHit(4),
        soundForCardHit(5),
        soundForCardMiss(),
    };
}

int WoodenPuzzleDepot::actionTagForPieceCreation() const {
    return 100;
}

int WoodenPuzzleDepot::actionTagForPullPiece() const {
    return 101;
}

END_NS_WOODENPUZZLES;

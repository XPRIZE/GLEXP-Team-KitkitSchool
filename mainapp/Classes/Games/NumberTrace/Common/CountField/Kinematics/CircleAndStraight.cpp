//
//  CircleAndStraight.cpp on Jun 28, 2016
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "CircleAndStraight.h"
#include "Common/Basic/DarkMagicMath.h"
#include <vector>

using namespace cocos2d;
using namespace std;

namespace {
    const bool CircleAndStraightDebugPrint = false;
}

namespace todoschool {
namespace countfield {

CircleAndStraight::CircleAndStraight(float Speed, float AngularSpeed)
    : Speed(Speed), AngularSpeed(AngularSpeed)
{
    clear();
}
    
void CircleAndStraight::clear() {
    EngineUptime = 0.f;
    EngineTimeLimit.reset();
    Scale.update(1.f);
}

void CircleAndStraight::update(float DeltaSeconds) {
    DarkMagicMath Math;

    EngineUptime += DeltaSeconds;
    
    Point DeltaPosition = TargetPosition() - CurrentPosition();
    bool PositionCheat = false ||
        DeltaPosition.length() <= Speed * Scale() * DeltaSeconds * 1.10f ||
        DeltaPosition.length() <= 10.f;

    if (PositionCheat) {
        // NB(xenosoz, 2016): We're here in the target position.
        updateToTargetPosition(DeltaSeconds);
        
        float DeltaRotation = Math.NormalizedAngle(TargetRotation() - CurrentRotation(), 0.f);
        bool AngleCheat = false ||
            fabs(DeltaRotation) <= AngularSpeed * DeltaSeconds * 1.10f ||
            fabs(DeltaRotation) <= 10.f;

        if (AngleCheat) {
            // NB(xenosoz, 2016): We're aligned with the target angle.
            updateToTargetAngle(DeltaSeconds);
            
            if (OnMovingDidEnd)
                OnMovingDidEnd(this);
            
            return;
        }
    }
    
    if (EngineTimeLimit && EngineUptime > EngineTimeLimit.value()) {
        // NB(xenosoz, 2016): Time's up. Jump to target and finish this.
        updateToTargetPosition(DeltaSeconds);
        updateToTargetAngle(DeltaSeconds);
        
        if (OnMovingDidEnd)
            OnMovingDidEnd(this);
        
        return;
    }

    Solution S = solve();
    if (!S.Valid) {
        // NB(xenosoz, 2016): While having no solution is practically impossible,
        //   I decided to buy insurance for future modification (or unknown bugs).
        updateToTargetPosition(DeltaSeconds);
        updateToTargetAngle(DeltaSeconds);
        
        if (OnMovingDidEnd)
            OnMovingDidEnd(this);
        
        return;
    }

    float TimeLeft = DeltaSeconds;
    
    int UsedCount = 0;
    for (auto& C : S.Commands) {
        if (TimeLeft <= 0.f) { break; }

        UsedCount += 1;
        
        if (C.Straight > 0.f) {
            float TimeUsed = min(C.Straight / (Speed * Scale()), TimeLeft);
            updateByMoveStraight(TimeUsed);
            
            TimeLeft -= TimeUsed;
            continue;
        }
        
        if (C.Left > 0.f) {
            float TimeUsed = min(C.Left / (Speed * Scale()), TimeLeft);
            updateByTurnLeft(TimeUsed);
            
            TimeLeft -= TimeUsed;
            continue;
        }
        
        if (C.Right > 0.f) {
            float TimeUsed = min(C.Right / (Speed * Scale()), TimeLeft);
            updateByTurnRight(TimeUsed);
            
            TimeLeft -= TimeUsed;
            continue;
        }
    }
    
    if (::CircleAndStraightDebugPrint) {
        // NB(xenosoz, 2016): Debug print.
        for (int Count = 0; Count < 3; ++Count) {
            auto& C = S.Commands[Count];
            char Check = (Count < UsedCount) ? 'v' : ' ';

            CCLOG("[%c%d] S:%f, L:%f, R:%f", Check, Count, C.Straight, C.Left, C.Right);
        }
        CCLOG("");
    }
}
 
void CircleAndStraight::updateToTargetPosition(float) {
    CurrentPosition.update(TargetPosition());
}

void CircleAndStraight::updateToTargetAngle(float) {
    CurrentRotation.update(TargetRotation());
}

void CircleAndStraight::updateByMoveStraight(float DeltaSeconds) {
    float Angle = CurrentRotation();
    Vec2 Velocity = Speed * Scale() * Vec2(cos(Angle), sin(Angle));
    
    Point P = CurrentPosition() + Velocity * DeltaSeconds;
    CurrentPosition.update(P);
}
    
void CircleAndStraight::updateByTurnLeft(float DeltaSeconds) {
    float Angle = AngularSpeed * DeltaSeconds;
    float Radius = radius();
    Point P = CurrentPosition();
    
    Point Pivot = ([&] {
        float PivotAngle = CurrentRotation() + M_PI_2;
        Vec2 PivotDirection(cos(PivotAngle), sin(PivotAngle));
        return P + Radius * PivotDirection;
    }());
    
    P.rotate(Pivot, Angle);
    CurrentPosition.update(P);
    
    // XXX: We need more elegant solution to cope with mathematical inconsistency.
    CurrentRotation.update(CurrentRotation() + Angle * 1.01f);
}
    
void CircleAndStraight::updateByTurnRight(float DeltaSeconds) {
    float Angle = -AngularSpeed * DeltaSeconds;
    float Radius = radius();
    Point P = CurrentPosition();
    
    Point Pivot = ([&] {
        float PivotAngle = CurrentRotation() - M_PI_2;
        Vec2 PivotDirection(cos(PivotAngle), sin(PivotAngle));
        return P + Radius * PivotDirection;
    }());
    
    P.rotate(Pivot, Angle);
    CurrentPosition.update(P);
    
    // XXX: We need more elegant solution to cope with mathematical inconsistency.
    CurrentRotation.update(CurrentRotation() + Angle * 1.01f);
}

CircleAndStraight::Command::Command()
    : Left(0.f), Straight(0.f), Right(0.f) {}
    
float CircleAndStraight::Command::sum() {
    return Left + Straight + Right;
}
    
CircleAndStraight::Solution::Solution()
    : Valid(false) {}
    
float CircleAndStraight::Solution::distance() {
    float D = 0.f;

    for (auto& C : Commands)
        D += C.sum();

    return D;
}
    
float CircleAndStraight::offsetRotation() {
    DarkMagicMath Math;
    return Math.Atan2OfPoint(TargetPosition() - CurrentPosition());
}
    
float CircleAndStraight::alpha() {
    DarkMagicMath Math;
    float OffsetRotation = offsetRotation();
    float Alpha = CurrentRotation() - OffsetRotation;
    
    return Math.NormalizedAngle(Alpha, M_PI);
}

float CircleAndStraight::beta() {
    DarkMagicMath Math;
    float OffsetRotation = offsetRotation();
    float Beta = TargetRotation() - OffsetRotation;

    return Math.NormalizedAngle(Beta, M_PI);
}

float CircleAndStraight::radius() {
    return Speed * Scale() / AngularSpeed;
}

float CircleAndStraight::distance() {
    float Distance = CurrentPosition().distance(TargetPosition());
    float Normalized = Distance / radius();

    return Normalized;
}
    
CircleAndStraight::Solution CircleAndStraight::solveLSL() {
    // NB(xenosoz, 2016): Case #1.
    Solution Ret;
    DarkMagicMath Math;

    float Alpha = alpha();
    float Beta = beta();
    float Distance = distance();
    float Radius = radius();
    {
        float Y = cos(Beta) - cos(Alpha);
        float X = Distance + sin(Alpha) - sin(Beta);
        float Angle = -Alpha + atan2(Y, X);
        Ret.Commands[0].Left = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    {
        float Squared = 2 + Distance * Distance - 2 * cos(Alpha - Beta) + 2 * Distance * (sin(Alpha) - sin(Beta));
        if (Squared < 0.f) { return Ret; }
        Ret.Commands[1].Straight = sqrt(Squared) * Radius;
    }
    {
        float Y = cos(Beta) - cos(Alpha);
        float X = Distance + sin(Alpha) - sin(Beta);
        float Angle = Beta - atan2(Y, X);
        Ret.Commands[2].Left = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    
    Ret.Valid = true;
    return Ret;
}

CircleAndStraight::Solution CircleAndStraight::solveRSR() {
    // NB(xenosoz, 2016): Case #2.
    Solution Ret;
    DarkMagicMath Math;
    
    float Alpha = alpha();
    float Beta = beta();
    float Distance = distance();
    float Radius = radius();
    {
        float Y = cos(Alpha) - cos(Beta);
        float X = Distance - sin(Alpha) + sin(Beta);
        float Angle = Alpha - atan2(Y, X);
        Ret.Commands[0].Right = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    {
        float Squared = 2 + Distance * Distance - 2 * cos(Alpha - Beta) + 2 * Distance * (sin(Beta) - sin(Alpha));
        if (Squared < 0.f) { return Ret; }
        Ret.Commands[1].Straight = sqrt(Squared) * Radius;
    }
    {
        float Y = cos(Alpha) - cos(Beta);
        float X = Distance - sin(Alpha) + sin(Beta);
        float Angle = -Beta + atan2(Y, X);
        Ret.Commands[2].Right = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    
    Ret.Valid = true;
    return Ret;
}

CircleAndStraight::Solution CircleAndStraight::solveLSR() {
    // NB(xenosoz, 2016): Case #3.

    Solution Ret;
    DarkMagicMath Math;
    
    float Alpha = alpha();
    float Beta = beta();
    float Distance = distance();
    float Radius = radius();
    
    float Squared = -2 + Distance * Distance + 2 * cos(Alpha - Beta) + 2 * Distance * (sin(Alpha) + sin(Beta));
    if (Squared < 0.f) { return Ret; }
    float P_lsr = sqrt(Squared);
    
    {
        float Y = -cos(Alpha) - cos(Beta);
        float X = Distance + sin(Alpha) + sin(Beta);
        float Angle = -Alpha + atan2(Y, X) - atan2(-2, P_lsr);
        Ret.Commands[0].Left = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    {
        Ret.Commands[1].Straight = P_lsr * Radius;
    }
    {
        float Y = -cos(Alpha) - cos(Beta);
        float X = Distance + sin(Alpha) + sin(Beta);
        float Angle = -Beta + atan2(Y, X) - atan2(-2, P_lsr);
        Ret.Commands[2].Right = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    
    Ret.Valid = true;
    return Ret;
}

CircleAndStraight::Solution CircleAndStraight::solveRSL() {
    // NB(xenosoz, 2016): Case #4.

    Solution Ret;
    DarkMagicMath Math;
    
    float Alpha = alpha();
    float Beta = beta();
    float Distance = distance();
    float Radius = radius();
    
    float Squared = Distance * Distance - 2 + 2 * cos(Alpha - Beta) - 2 * Distance * (sin(Alpha) + sin(Beta));
    if (Squared < 0.f) { return Ret; }
    float P_rsl = sqrt(Squared);
    
    {
        float Y = cos(Alpha) + cos(Beta);
        float X = Distance - sin(Alpha) - sin(Beta);
        float Angle = Alpha - atan2(Y, X) + atan2(2, P_rsl);
        Ret.Commands[0].Right = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    {
        Ret.Commands[1].Straight = P_rsl * Radius;
    }
    {
        float Y = cos(Alpha) + cos(Beta);
        float X = Distance - sin(Alpha) - sin(Beta);
        float Angle = Beta - atan2(Y, X) + atan2(2, P_rsl);
        Ret.Commands[2].Left = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    
    Ret.Valid = true;
    return Ret;
}

CircleAndStraight::Solution CircleAndStraight::solveRLR() {
    // NB(xenosoz, 2016): Case #5.

    Solution Ret;
    DarkMagicMath Math;
    
    float Alpha = alpha();
    float Beta = beta();
    float Distance = distance();
    float Radius = radius();
    
    float CosValue = (6.f - Distance * Distance + 2 * cos(Alpha - Beta) + 2 * Distance * (sin(Alpha) - sin(Beta))) / 8.f;
    if (abs(CosValue) > 1.f) { return Ret; }
    float P_rlr = Math.NormalizedAngle(-acos(CosValue), M_PI);  // XXX
    
    float T_rlr = 0.f;
    {
        float Y = cos(Alpha) - cos(Beta);
        float X = Distance - sin(Alpha) + sin(Beta);
        float Angle = Alpha - atan2(Y, X) + P_rlr / 2.f;
        T_rlr = Math.NormalizedAngle(Angle, M_PI);
        Ret.Commands[0].Right = T_rlr * Radius;
    }
    {
        Ret.Commands[1].Left = P_rlr * Radius;
    }
    {
        float Angle = Alpha - Beta - T_rlr + P_rlr;
        Ret.Commands[2].Right = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    
    Ret.Valid = true;
    return Ret;
}

CircleAndStraight::Solution CircleAndStraight::solveLRL() {
    // NB(xenosoz, 2016): Case #6.
    
    Solution Ret;
    DarkMagicMath Math;
    
    float Alpha = alpha();
    float Beta = beta();
    float Distance = distance();
    float Radius = radius();
    
    float CosValue = (6.f - Distance * Distance + 2 * cos(Alpha - Beta) + 2 * Distance * (-sin(Alpha) + sin(Beta))) / 8.f;  // XXX
    if (abs(CosValue) > 1.f) { return Ret; }
    float P_lrl = Math.NormalizedAngle(-acos(CosValue), M_PI);  // XXX
    
    float T_lrl = 0.f;
    {
        float Y = cos(Alpha) - cos(Beta);
        float X = Distance + sin(Alpha) - sin(Beta);
        float Angle = -Alpha - atan2(Y, X) + P_lrl / 2.f;
        T_lrl = Math.NormalizedAngle(Angle, M_PI);
        Ret.Commands[0].Left = T_lrl * Radius;
    }
    {
        Ret.Commands[1].Right = P_lrl * Radius;
    }
    {
        float Angle = Beta - Alpha - T_lrl + P_lrl;  // XXX
        Ret.Commands[2].Left = Math.NormalizedAngle(Angle, M_PI) * Radius;
    }
    
    Ret.Valid = true;
    return Ret;
}
    
CircleAndStraight::Solution CircleAndStraight::solve() {
    vector<Solution> Solutions;
    Solutions.push_back(solveLSL());
    Solutions.push_back(solveRSR());
    Solutions.push_back(solveLSR());
    Solutions.push_back(solveRSL());
    Solutions.push_back(solveRLR());
    Solutions.push_back(solveLRL());

    Solution Best;
    for (auto& S : Solutions) {
        if (!S.Valid) { continue; }
        if (!Best.Valid || S.distance() < Best.distance()) {
            Best = S;
        }
    }
    
    return Best;
}


}  // namespace countfield
}  // namespace todoschool

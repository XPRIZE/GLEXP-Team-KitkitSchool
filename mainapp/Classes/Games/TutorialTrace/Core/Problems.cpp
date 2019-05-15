//
//  Problems.cpp
//  TodoSchool on Dec 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "Problems.h"
#include "../Utils/TutorialTraceMainDepot.h"


BEGIN_NS_TUTORIALTRACE

void setupFieldForProblem(ScratchField* It, int Problem)
{
    switch (Problem) {
        case 1: return setupFieldForProblem_1(It);
        case 2: return setupFieldForProblem_2(It);
        case 3: return setupFieldForProblem_3(It);
        case 4: return setupFieldForProblem_4(It);
        case 5: return setupFieldForProblem_5(It);
        case 6: return setupFieldForProblem_6(It);
        case 7: return setupFieldForProblem_7(It);
        case 8: return setupFieldForProblem_8(It);
        case 9: return setupFieldForProblem_9(It);
        case 10: return setupFieldForProblem_10(It);
        case 11: return setupFieldForProblem_11(It);
        case 12: return setupFieldForProblem_12(It);
        case 13: return setupFieldForProblem_13(It);
        case 14: return setupFieldForProblem_14(It);
        case 15: return setupFieldForProblem_15(It);
        case 16: return setupFieldForProblem_16(It);
        case 17: return setupFieldForProblem_17(It);
        case 18: return setupFieldForProblem_18(It);
        case 19: return setupFieldForProblem_19(It);
    }

    CCLOGERROR("Unknown problem(%d) in %s", Problem, __PRETTY_FUNCTION__);
}

void setupFieldForProblem_1(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/1";
    auto TopA = AP + "/tutorialtracing1_line1top.png";
    auto BotA = AP + "/tutorialtracing1_line1bottom.png";
    auto TopB = AP + "/tutorialtracing1_line2top.png";
    auto BotB = AP + "/tutorialtracing1_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.1.json");
    
    It->appendStroke(Point(195.f, 633.f + 60.f + 413.f), TopA, BotA);
    It->appendStroke(Point(195.f, 633.f), TopB, BotB);
}

void setupFieldForProblem_2(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/2";
    auto Top = AP + "/tutorialtracing2_line1-4top.png";
    auto BotA = AP + "/tutorialtracing2_line1bottom.png";
    auto BotB = AP + "/tutorialtracing2_line2bottom.png";
    auto BotC = AP + "/tutorialtracing2_line3bottom.png";
    auto BotD = AP + "/tutorialtracing2_line4bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.2.json");

    float LeftX = 195.f;
    float RightX = 195.f + 961.f + 240.f;
    float UpY = 633.f + 60.f + 413.f;
    float DownY = 633.f;
    
    It->appendStroke(Point(LeftX, UpY), Top, BotA);
    It->appendStroke(Point(RightX, UpY), Top, BotB);

    It->appendStroke(Point(LeftX, DownY), Top, BotC);
    It->appendStroke(Point(RightX, DownY), Top, BotD);
}

void setupFieldForProblem_3(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/3";
    auto Top = AP + "/tutorialtracing3_line1-3top.png";
    auto BotA = AP + "/tutorialtracing3_line1bottom.png";
    auto BotB = AP + "/tutorialtracing3_line2bottom.png";
    auto BotC = AP + "/tutorialtracing3_line3bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.3.json");
    
    float LeftX = 560.f;
    float MidX = LeftX + 56.f + 635.f;
    float RightX = MidX + 56.f + 635.f;
    float Y = 387.f;
    
    It->appendStroke(Point(LeftX, Y), Top, BotA);
    It->appendStroke(Point(MidX, Y), Top, BotB);
    It->appendStroke(Point(RightX, Y), Top, BotC);
}

void setupFieldForProblem_4(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/4";
    auto Top = AP + "/tutorialtracing4_linetop.png";
    auto Bot = AP + "/tutorialtracing4_linebottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.4.json");

    float X = 424.f;
    float Y = 518.f;
    It->appendStroke(Point(X, Y), Top, Bot);
}

void setupFieldForProblem_5(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/5";
    auto TopA = AP + "/tutorialtracing5_line1top.png";
    auto BotA = AP + "/tutorialtracing5_line1bottom.png";
    auto TopB = AP + "/tutorialtracing5_line2top.png";
    auto BotB = AP + "/tutorialtracing5_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.5.json");
    
    It->appendStroke(Point(446.f, 1006.f), TopA, BotA);
    It->appendStroke(Point(446.f, 1006.f), TopB, BotB);
    
    It->appendStroke(Point(1514.f, 1006.f), TopA, BotA);
    It->appendStroke(Point(1514.f, 1006.f), TopB, BotB);
    
    It->appendStroke(Point(446.f, 192.f), TopA, BotA);
    It->appendStroke(Point(446.f, 192.f), TopB, BotB);
    
    It->appendStroke(Point(1514.f, 192.f), TopA, BotA);
    It->appendStroke(Point(1514.f, 192.f), TopB, BotB);
}

void setupFieldForProblem_6(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/6";
    auto Top = AP + "/tutorialtracing6_line1-3top.png";
    auto BotA = AP + "/tutorialtracing6_line1bottom.png";
    auto BotB = AP + "/tutorialtracing6_line2bottom.png";
    auto BotC = AP + "/tutorialtracing6_line3bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.6.json");
    
    float LeftX = 441.f;
    float MidX = LeftX + 374.f + 275.f;
    float RightX = MidX + 374.f + 275.f;
    float Y = 242.f;
    
    It->appendStroke(Point(LeftX, Y), Top, BotA);
    It->appendStroke(Point(MidX, Y), Top, BotB);
    It->appendStroke(Point(RightX, Y), Top, BotC);
}

void setupFieldForProblem_7(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/7";
    auto TopA = AP + "/tutorialtracing7_line1top.png";
    auto BotA = AP + "/tutorialtracing7_line1bottom.png";
    auto TopB = AP + "/tutorialtracing7_line2top.png";
    auto BotB = AP + "/tutorialtracing7_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.7.json");
    
    float X = 341.f;
    float UpY = 458.f + 307.f + 265.f;
    float DownY = 458.f;
    
    It->appendStroke(Point(X, UpY), TopA, BotA);
    It->appendStroke(Point(X, DownY), TopB, BotB);
}

void setupFieldForProblem_8(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/8";
    auto TopA = AP + "/tutorialtracing8_line1top.png";
    auto BotA = AP + "/tutorialtracing8_line1bottom.png";
    auto TopB = AP + "/tutorialtracing8_line2top.png";
    auto BotB = AP + "/tutorialtracing8_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.8.json");
    
    float X = 256.f;
    float UpY = 375.f + 390.f + 230.f;
    float DownY = 375.f;
    
    It->appendStroke(Point(X, UpY), TopA, BotA);
    It->appendStroke(Point(X, DownY), TopB, BotB);
}

void setupFieldForProblem_9(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/9";
    auto TopA = AP + "/tutorialtracing9_line1top.png";
    auto BotA = AP + "/tutorialtracing9_line1bottom.png";
    auto TopB = AP + "/tutorialtracing9_line2top.png";
    auto BotB = AP + "/tutorialtracing9_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.9.json");
    
    float X = 193.f;
    float UpY = 515.f + 267.f + 216.f;
    float DownY = 515.f;
    
    It->appendStroke(Point(X, UpY), TopA, BotA);
    It->appendStroke(Point(X, DownY), TopB, BotB);
}

void setupFieldForProblem_10(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/10";
    auto TopA = AP + "/tutorialtracing10_line1top.png";
    auto BotA = AP + "/tutorialtracing10_line1bottom.png";
    auto TopB = AP + "/tutorialtracing10_line2top.png";
    auto BotB = AP + "/tutorialtracing10_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.10.json");
    
    float X = 200.f;
    float UpY = 500.f + 238.f + 318.f;
    float DownY = 500.f;
    
    It->appendStroke(Point(X, UpY), TopA, BotA);
    It->appendStroke(Point(X, DownY), TopB, BotB);
}

void setupFieldForProblem_11(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/11";
    auto TopA = AP + "/tutorialtracing11_line1top.png";
    auto BotA = AP + "/tutorialtracing11_line1bottom.png";
    auto TopB = AP + "/tutorialtracing11_line2top.png";
    auto BotB = AP + "/tutorialtracing11_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.11.json");
    
    float X = 195.f;
    float UpY = 420.f + 324.f + 310.f;
    float DownY = 420.f;
    
    It->appendStroke(Point(X, UpY), TopA, BotA);
    It->appendStroke(Point(X, DownY), TopB, BotB);
}

void setupFieldForProblem_12(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/12";
    auto TopA = AP + "/tutorialtracing12_line1top.png";
    auto BotA = AP + "/tutorialtracing12_line1bottom.png";
    auto TopB = AP + "/tutorialtracing12_line2top.png";
    auto BotB = AP + "/tutorialtracing12_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.12.json");
    
    float X = 228.f;
    float UpY = 356.f + 440.f + 200.f;
    float DownY = 356.f;
    
    It->appendStroke(Point(X, UpY), TopA, BotA);
    It->appendStroke(Point(X, DownY), TopB, BotB);
}

void setupFieldForProblem_13(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/13";
    auto TopA = AP + "/tutorialtracing13_line1top.png";
    auto BotA = AP + "/tutorialtracing13_line1bottom.png";
    auto TopB = AP + "/tutorialtracing13_line2top.png";
    auto BotB = AP + "/tutorialtracing13_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.13.json");
    
    float X = 157.f;
    float UpY = 276.f + 502.f + 190.f;
    float DownY = 276.f;
    
    It->appendStroke(Point(X, UpY), TopA, BotA);
    It->appendStroke(Point(X, DownY), TopB, BotB);
}

void setupFieldForProblem_14(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/14";
    auto Top = AP + "/tutorialtracing14_linetop.png";
    auto Bot = AP + "/tutorialtracing14_linebottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.14.json");
    
    It->appendStroke(Point(200.f, 305.f), Top, Bot);
}

void setupFieldForProblem_15(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/15";
    auto TopA = AP + "/tutorialtracing15_linetop_rectangle.xenosoz.png";
    auto BotA = AP + "/tutorialtracing15_linebottom_rectangle.png";
    auto TopB = AP + "/tutorialtracing15_linetop_horizontal.png";
    auto BotB = AP + "/tutorialtracing15_linebottom_horizontal.png";
    auto TopC = AP + "/tutorialtracing15_linetop_vertical.png";
    auto BotC = AP + "/tutorialtracing15_linebottom_vertical.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.15.json");

    It->appendStroke(Point(216.f, 296.f), TopA, BotA);
    It->appendStroke(Point(216.f, 296.f), TopB, BotB);
    It->appendStroke(Point(216.f, 296.f), TopC, BotC);
}

void setupFieldForProblem_16(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/16";
    auto TopA = AP + "/tutorialtracing16_line1top.png";
    auto BotA = AP + "/tutorialtracing16_line1bottom.png";
    auto TopB = AP + "/tutorialtracing16_line2top.png";
    auto BotB = AP + "/tutorialtracing16_line2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.16.json");
    
    It->appendStroke(Point(345.f, 285.f), TopA, BotA);
    It->appendStroke(Point(1350.f, 800.f), TopB, BotB);
}

void setupFieldForProblem_17(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/17";
    auto TopA = AP + "/tutorialtracing17_circle1top.png";
    auto BotA = AP + "/tutorialtracing17_circle1bottom.png";
    auto TopB = AP + "/tutorialtracing17_circle2top.png";
    auto BotB = AP + "/tutorialtracing17_circle2bottom.png";
    auto TopC = AP + "/tutorialtracing17_circle3top.png";
    auto BotC = AP + "/tutorialtracing17_circle3bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.17.json");
    
    It->appendStroke(Point(321.f, 800.f), TopA, BotA);
    It->appendStroke(Point(590.f, 224.f), TopB, BotB);
    It->appendStroke(Point(2560.f - 262.f - 1164.f, 194.f), TopC, BotC);
}

void setupFieldForProblem_18(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/18";
    auto Top = AP + "/tutorialtracing18_startop.png";
    auto Bot = AP + "/tutorialtracing18_starbottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.18.json");
    
    It->appendStroke(Point(574.f, 230.f), Top, Bot);
}

void setupFieldForProblem_19(ScratchField* It) {
    auto AP = MainDepot().assetPrefix() + "/Images/19";
    auto TopA = AP + "/tutorialtracing19_star1top.png";
    auto BotA = AP + "/tutorialtracing19_star1bottom.png";
    auto TopB = AP + "/tutorialtracing19_star2top.png";
    auto BotB = AP + "/tutorialtracing19_star2bottom.png";
    It->StrokeName.update("TutorialTrace/Strokes/Custom.TutorialTracing.19.json");
    
    It->appendStroke(Point(277.f, 260.f), TopA, BotA);
    It->appendStroke(Point(1125.f, 465.f), TopB, BotB);
}

END_NS_TUTORIALTRACE

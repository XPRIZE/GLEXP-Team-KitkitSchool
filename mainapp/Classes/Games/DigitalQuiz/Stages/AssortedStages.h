//
//  AssortedStages.h -- Collection of all stage types
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "SoundToImageStage.h"
#include "SoundToTextStage.h"
#include "SoundToNumberStage.h"
#include "DotsToNumberStage.h"
#include "TextToNumberStage.h"
#include "FunStage.h"
#include "../Utils/DigitalQuizNS.h"


BEGIN_NS_DIGITALQUIZ

using ImageDummyStage = SoundToImageStage;
using LetterSoundStage = SoundToTextStage;
using SyllablesIDStage = SoundToTextStage;
using FamiliarWordStage = SoundToTextStage;
using InventedWordStage = SoundToTextStage;
using LiteracyEndStage = FunStage;

using NumberDummyStage = SoundToNumberStage;
using NumberIDStage = SoundToNumberStage;
using CountStage = DotsToNumberStage;
using AddStage = TextToNumberStage;
using SubStage = TextToNumberStage;
using MissingNumberStage = TextToNumberStage;
using MathEndStage = FunStage;

END_NS_DIGITALQUIZ

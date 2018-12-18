//
//  Fish.cpp
//  KitkitSchool
//
//  Created by YongSoo Hong on 2018. 9. 7..
//

#include "Fish.hpp"
#include "Utils/TodoUtil.h"

namespace
{
    const static string RESOURCE_PATH = "main/fish/";
    
    const static map<string, string> FISH_SKIN1 = {
        {make_pair("f01", "fish_n_angelfish_1.png")},
        {make_pair("f02", "fish_n_clownfish_1.png")},
        {make_pair("f03", "fish_n_mbunacichlid_1.png")},
        {make_pair("f04", "fish_n_neontetra_1.png")},
        {make_pair("f05", "fish_n_guppy_1.png")},
        {make_pair("f06", "fish_n_rainbowshark_1.png")},
        {make_pair("f07", "fish_n_jellyfish_1.png")},
        {make_pair("f08", "fish_n_seaslug_1.png")},
        {make_pair("f09", "fish_n_sesahorse_1.png")},
        {make_pair("f10", "fish_n_beeshrimp_1.png")},
        {make_pair("f11", "fish_r_dumbooctopus.png")},
        {make_pair("f12", "fish_r_firegoby.png")},
        {make_pair("f13", "fish_r_panthergroupter.png")},
        {make_pair("f14", "fish_r_clowntriggerfish.png")},
        {make_pair("f15", "fish_r_powderbluetang.png")},
        {make_pair("f16", "fish_r_regalangelfish.png")},
        {make_pair("f17", "fish_r_juvenile-emperor-angelfish.png")},
        {make_pair("f18", "fish_r_betta_blackmustradveiltails.png")},
        {make_pair("f19", "fish_r_seadragon.png")},
        {make_pair("f20", "fish_r_axolotl.png")},
        {make_pair("d01", "env_n_marimo_1.png")},
        {make_pair("d02", "env_n_starfish_1.png")},
        {make_pair("d03", "env_n_seashell_1.png")},
        {make_pair("d04", "env_n_plant_1.png")},
        {make_pair("d05", "env_n_cabbagecoral_1.png")},
        {make_pair("d06", "env_n_mushroomcoral_1.png")},
        {make_pair("d07", "env_n_coral_1.png")},
        {make_pair("d08", "env_n_tubecoral_1.png")},
        {make_pair("d09", "env_n_seaanemone_1.png")},
        {make_pair("d10", "env_n_lotus_1.png")},
        {make_pair("d11", "env_r_murex.png")},
        {make_pair("d12", "env_r_branchcoral.png")},
        {make_pair("d13", "env_r_barnacle.png")},
        {make_pair("d14", "env_r_seeweed.png")},
        {make_pair("d15", "env_r_pearlshell.png")},
        {make_pair("d16", "env_r_pot.png")},
        {make_pair("d17", "env_r_pancoral.png")},
        {make_pair("d18", "env_r_hermitcrab.png")},
        {make_pair("d19", "env_r_giantclam.png")},
        {make_pair("d20", "env_r_submarine.png")}
    };
    
    const static map<string, string> FISH_SKIN2 = {
        {make_pair("f01", "fish_n_angelfish_2.png")},
        {make_pair("f02", "fish_n_clownfish_2.png")},
        {make_pair("f03", "fish_n_mbunacichlid_2.png")},
        {make_pair("f04", "fish_n_neontetra_2.png")},
        {make_pair("f05", "fish_n_guppy_2.png")},
        {make_pair("f06", "fish_n_rainbowshark_2.png")},
        {make_pair("f07", "fish_n_jellyfish_2.png")},
        {make_pair("f08", "fish_n_seaslug_2.png")},
        {make_pair("f09", "fish_n_sesahorse_2.png")},
        {make_pair("f10", "fish_n_beeshrimp_2.png")},
        {make_pair("d01", "env_n_marimo_2.png")},
        {make_pair("d02", "env_n_starfish_2.png")},
        {make_pair("d03", "env_n_seashell_2.png")},
        {make_pair("d04", "env_n_plant_2.png")},
        {make_pair("d05", "env_n_cabbagecoral_2.png")},
        {make_pair("d06", "env_n_mushroomcoral_2.png")},
        {make_pair("d07", "env_n_coral_2.png")},
        {make_pair("d08", "env_n_tubecoral_2.png")},
        {make_pair("d09", "env_n_seaanemone_2.png")},
        {make_pair("d10", "env_n_lotus_2.png")},
    };
    
    const static map<string, string> FISH_POSITION = {
        {make_pair("d01", "B2,b2,D1,D5,D8,D11")},
        {make_pair("d02", "C1,C2,C3,C4")},
        {make_pair("d03", "C1,C2,C4,D3,D6")},
        {make_pair("d04", "B1,b1,B3,b3")},
        {make_pair("d05", "D2,D3,D6,D7,D9,D10")},
        {make_pair("d06", "D3,D6,D8,D11")},
        {make_pair("d07", "A1,A2,A3,D3,D6,D8,D10")},
        {make_pair("d08", "D2,D3,D6,D7,D9,D10")},
        {make_pair("d09", "D2,D5,D6,D7,D8,D9,D11")},
        {make_pair("d10", "A2,A3,D1,D2,D5,D6,D7,D8")},
        {make_pair("d11", "C1,C2,C3,C4")},
        {make_pair("d12", "D2,D3,D6,D7,D8,D10,D11")},
        {make_pair("d13", "C1,D2,D8,D11")},
        {make_pair("d14", "B1,b1,B3,b3")},
        {make_pair("d15", "C1,C2,C3,C4,D3,D6")},
        {make_pair("d16", "C1,D6,D11")},
        {make_pair("d17", "D2,D3,D6,D7,D10,D11")},
        {make_pair("d18", "C1,C2,C3,C4")},
        {make_pair("d19", "C1,C2,C3,C4,D3,D6")},
        {make_pair("d20", "C1,C2,C3,C4")}
    };
    
    const static map<int, string> LEVEL_NORMAL_FISH_L = {
        {make_pair(1, "f01")},
        {make_pair(2, "f04")},
        {make_pair(3, "f07")},
        {make_pair(4, "f02")},
        {make_pair(5, "f05")},
        {make_pair(6, "f10")},
        {make_pair(7, "f06")},
        {make_pair(8, "f03")},
        {make_pair(9, "f09")},
        {make_pair(10, "f08")}
    };
    
    const static map<int, string> LEVEL_NORMAL_FISH_M = {
        {make_pair(1, "d04")},
        {make_pair(2, "d02")},
        {make_pair(3, "d01")},
        {make_pair(4, "d07")},
        {make_pair(5, "d10")},
        {make_pair(6, "d03")},
        {make_pair(7, "d05")},
        {make_pair(8, "d08")},
        {make_pair(9, "d06")},
        {make_pair(10, "d09")}
    };
    
    const static map<int, string> LEVEL_RARE_FISH_L = {
        {make_pair(1, "f16")},
        {make_pair(2, "f12")},
        {make_pair(3, "f17")},
        {make_pair(4, "f14")},
        {make_pair(5, "f13")},
        {make_pair(6, "f20")},
        {make_pair(7, "f18")},
        {make_pair(8, "f15")},
        {make_pair(9, "f19")},
        {make_pair(10, "f11")}
    };
    
    const static map<int, string> LEVEL_RARE_FISH_M = {
        {make_pair(1, "d14")},
        {make_pair(2, "d12")},
        {make_pair(3, "d11")},
        {make_pair(4, "d17")},
        {make_pair(5, "d20")},
        {make_pair(6, "d13")},
        {make_pair(7, "d15")},
        {make_pair(8, "d18")},
        {make_pair(9, "d16")},
        {make_pair(10, "d19")}
    };
}

string Fish::getFishID(char category, int level, bool isRare) {
    if (category == 'L') {
        return isRare ? LEVEL_RARE_FISH_L.at(level) : LEVEL_NORMAL_FISH_L.at(level);
        
    } else if (category == 'M') {
        return isRare ? LEVEL_RARE_FISH_M.at(level) : LEVEL_NORMAL_FISH_M.at(level);
        
    }

    return "";
}

vector<string> Fish::getFishPosition(string fishID) {
    if (TodoUtil::startsWith(fishID, "f")) {
        return vector<string>();
    }
    
    return TodoUtil::split(FISH_POSITION.at(fishID), ',');
}

Fish* Fish::create(char category, int level, bool isRare, int skin, bool isGray) {
    Fish* pRet = new (std::nothrow) Fish();
    
    if(pRet && pRet->init(category, level, isRare, skin, isGray))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Fish::init(char category, int level, bool isRare, int skin, bool isGray) {
    if (!Node::init()) {
        return false;
    }
    
    if (category != 'L' && category != 'M') {
        return false;
    }
    
    if (skin != 1 && skin != 2) {
        return false;
    }
    
    _fishID = getFishID(category, level, isRare);
    _skin = skin;
    string imagePath = _skin == 1 ? FISH_SKIN1.at(_fishID) : FISH_SKIN2.at(_fishID);
    
    _spriteFish = Sprite::create(RESOURCE_PATH + imagePath);
    if (_spriteFish) {
        if (isGray) {
            convertSpriteToGrayScale(_spriteFish);
        }
        _spriteFish->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _spriteFish->setPosition(getContentSize() / 2);
        addChild(_spriteFish);
    }
    return true;
}

void Fish::setContentSize(const Size &size) {
    Node::setContentSize(size);
    fishScale(size);
    _spriteFish->setPosition(size / 2);
}

void Fish::fishScale(Size holdSize) {
    if (_spriteFish == nullptr) {
        return;
    }
    
    Size fishSize = _spriteFish->getContentSize();
    
    if (fishSize.width <= holdSize.width && fishSize.height <= holdSize.height) {
        _spriteFish->setScale(1.0f);
        
    } else {
        float holdAspectRatio = holdSize.width / holdSize.height;
        float fishAspectRatio = fishSize.width / fishSize.height;
        float scale;
        if (fishAspectRatio > holdAspectRatio) {
            scale = holdSize.width / fishSize.width;
            
        } else {
            scale = holdSize.height / fishSize.height;
        }
        
        _spriteFish->setScale(scale);
    }
}

void Fish::convertSpriteToGrayScale(Sprite* sprite) {
    if(sprite) {
        GLProgram * p = new GLProgram();
        p->initWithFilenames(RESOURCE_PATH + "gray.vsh", RESOURCE_PATH + "gray.fsh");
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p->link();
        p->updateUniforms();
        sprite->setGLProgram(p);
    }
}

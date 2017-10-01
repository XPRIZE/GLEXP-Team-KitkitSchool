//
//  Piece.hpp -- A card piece setting
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Piece.hpp"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;


namespace ShapeMatching {
    
    Piece::Piece() {
    }
    
    Piece::Piece(int numOfPices,
                 const std::string& matchSound,
                 const std::string& shape,
                 const std::string& typeA,
                 const std::string& colorA,
                 const std::string& sizeA,
                 const std::string& rotationA,
                 const std::string& typeB,
                 const std::string& colorB,
                 const std::string& sizeB,
                 const std::string& rotationB)
    : numOfPices(numOfPices)
    , matchSound(matchSound)
    , shape(shape)
    , typeA(typeA)
    , colorA(colorA)
    , sizeA(sizeA)
    , rotationA(rotationA)
    , typeB(typeB)
    , colorB(colorB)
    , sizeB(sizeB)
    , rotationB(rotationB)
    {
    }
    
    Piece Piece::fromInputStream(istream &stream) {
        int numOfPices;
        string matchSound;
        std::string shape;
        std::string typeA;
        std::string colorA;
        std::string sizeA;
        std::string rotationA;
        std::string typeB;
        std::string colorB;
        std::string sizeB;
        std::string rotationB;
        
        if (!(stream >> numOfPices >> matchSound >> shape >> typeA >> colorA >> sizeA >> rotationA >> typeB >> colorB >> sizeB >> rotationB)) {
            CCLOGERROR("Unexpected end of stream in %s", __PRETTY_FUNCTION__);
            return Piece();
        }
        
        // NB(xenosoz, 2016): Ignore the piece ID.
        return Piece(numOfPices, matchSound, shape, typeA, colorA, sizeA, rotationA, typeB, colorB, sizeB, rotationB);
    }
    
    istream& operator>>(istream& stream, Piece& Piece) {
        Piece = Piece::fromInputStream(stream);
        return stream;
    }
    
}  // namespace ShapeMatching

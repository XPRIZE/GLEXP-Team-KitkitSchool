//
//  Piece.hpp -- A card piece setting
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once
#include <istream>


namespace ShapeMatching {
    
    class Piece {
    public:
        int numOfPices;
        std::string matchSound;
        std::string shape;
        std::string typeA;
        std::string colorA;
        std::string sizeA;
        std::string rotationA;
        std::string typeB;
        std::string colorB;
        std::string sizeB;
        std::string rotationB;
        
    public:
        Piece();
        Piece(int numOfPices,
              const std::string& matchSound,
              const std::string& shape,
              const std::string& typeA,
              const std::string& colorA,
              const std::string& sizeA,
              const std::string& rotationA,
              const std::string& typeB,
              const std::string& colorB,
              const std::string& sizeB,
              const std::string& rotationB);
        
    public:
        static Piece fromInputStream(std::istream& stream);
        friend std::istream& operator>>(std::istream& stream, Piece& problem);
    };
    
}  // namespace ShapeMatching

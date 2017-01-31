//
//  TodoSchoolDrawNode.cpp on Jul 3, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "TodoSchoolDrawNode.h"
#include <Games/NumberTrace/Common/Repr/AllRepr.h>

#include <vector>

using namespace cocos2d;
using namespace std;

namespace {
    
Tex2F tex2ByVec2(const Vec2& V) {
    Tex2F T(V.x, V.y);
    return T;
}

}  // unnamed namespace


namespace todoschool {
    
void TodoSchoolDrawNode::drawSegments(const cocos2d::Vec2 *verts, int count,
                                      float thickness, const cocos2d::Color4B& color)
{
    CCASSERT(count >= 0, "invalid count value");

    // NB(xenosoz, 2016): Ensure normal vector != zero.
    // XXX: Possible optimization?
    vector<Point> points;
    points.reserve(count);
    points.push_back(verts[0]);
    for (int i = 0; i < count; ++i) {
        if (verts[i] == points.back()) { continue; }
        points.push_back(verts[i]);
    }
    
    verts = &points[0];
    count = (int)points.size();
    float radius = thickness / 2.f;

    auto triangle_count = 2 * (count + 1);
    auto vertex_count = 3 * triangle_count;
    ensureCapacity(vertex_count);
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle *cursor = triangles;
    
    vector<Vec2> normals;
    normals.reserve(count);

    for (int i = 0; i < count; ++i) {
        Point v_prev = (i - 1 >= 0) ? verts[i - 1] : verts[i];
        Point v_zero = verts[i];
        Point v_next = (i + 1 < count) ? verts[i + 1] : verts[i];

        Vec2 n_prev = (v_zero - v_prev).getPerp().getNormalized();
        Vec2 n_next = (v_next - v_zero).getPerp().getNormalized();
        Vec2 n = (n_prev + n_next) / (Vec2::dot(n_prev, n_next) + 1.f);

        normals.push_back(move(n));
    }

    // NB(xenosoz, 2016): Starting point.
    if (true) {
        Point v = verts[0];

        Vec2 n = normals[0];
        Vec2 t = n.getPerp();

        Vec2 nr = n * radius;
        Vec2 tr = t * radius;
        
        Point a = v + nr;
        Point b = v + nr + tr;
        Point c = v - nr + tr;
        Point d = v - nr;
        
        V2F_C4B_T2F_Triangle tri_a = {
            {c, color, tex2ByVec2(-n + t)},
            {d, color, tex2ByVec2(-n)},
            {a, color, tex2ByVec2(n)}
        };
        *cursor++ = tri_a;
        
        V2F_C4B_T2F_Triangle tri_b = {
            {c, color, tex2ByVec2(-n + t)},
            {b, color, tex2ByVec2(n + t)},
            {a, color, tex2ByVec2(n)}
        };
        *cursor++ = tri_b;
    }

    // NB(xenosoz, 2016): Internal line segments.
    for (int i = 0; i < count - 1; i++) {
        Point v0 = verts[i];
        Point v1 = verts[i + 1];
        
        Vec2 n0 = normals[i];
        Vec2 n1 = normals[i + 1];
        Vec2 nr0 = n0 * radius;
        Vec2 nr1 = n1 * radius;

        Vec2 v0_in = v0 - nr0;
        Vec2 v0_out = v0 + nr0;
        Vec2 v1_in = v1 - nr1;
        Vec2 v1_out = v1 + nr1;

        V2F_C4B_T2F_Triangle tri_a = {
            {v0_in, color, tex2ByVec2(-n0)},
            {v1_in, color, tex2ByVec2(-n1)},
            {v1_out, color, tex2ByVec2(n1)}
        };
        *cursor++ = tri_a;
        
        V2F_C4B_T2F_Triangle tri_b = {
            {v0_in, color, tex2ByVec2(-n0)},
            {v0_out, color, tex2ByVec2(n0)},
            {v1_out, color, tex2ByVec2(n1)}
        };
        *cursor++ = tri_b;
    }
    
    // NB(xenosoz, 2016): End point.
    if (true) {
        Point v = verts[count - 1];
        
        Vec2 n = normals[count - 1];
        Vec2 t = n.getRPerp();
        
        Vec2 nr = n * radius;
        Vec2 tr = t * radius;
        
        Point a = v + nr;
        Point b = v + nr + tr;
        Point c = v - nr + tr;
        Point d = v - nr;
        
        V2F_C4B_T2F_Triangle tri_a = {
            {c, color, tex2ByVec2(-n + t)},
            {d, color, tex2ByVec2(-n)},
            {a, color, tex2ByVec2(n)}
        };
        *cursor++ = tri_a;
        
        V2F_C4B_T2F_Triangle tri_b = {
            {c, color, tex2ByVec2(-n + t)},
            {b, color, tex2ByVec2(n + t)},
            {a, color, tex2ByVec2(n)}
        };
        *cursor++ = tri_b;
    }
    
    _bufferCount += vertex_count;
    
    _dirty = true;
}

}  // namespace todoschool

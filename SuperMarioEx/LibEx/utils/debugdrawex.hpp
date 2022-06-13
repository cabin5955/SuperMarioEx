//
//  debugdrawex.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/11.
//

#ifndef debugdrawex_hpp
#define debugdrawex_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glad.h"

#define ex_pi            3.14159265359f

struct exAABB;
struct GLRenderPointsEx;
struct GLRenderLinesEx;
struct GLRenderTrianglesEx;

/// Ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
struct  exRayCastInput
{
    glm::vec2 p1, p2;
    float maxFraction;
};

/// Ray-cast output data. The ray hits at p1 + fraction * (p2 - p1), where p1 and p2
/// come from b2RayCastInput.
struct  exRayCastOutput
{
    glm::vec2 normal;
    float fraction;
};

template <typename T>
inline T exMin(T a, T b)
{
    return a < b ? a : b;
}

inline glm::vec2 exMin(const glm::vec2& a, const glm::vec2& b)
{
    return glm::vec2(exMin(a.x, b.x), exMin(a.y, b.y));
}

template <typename T>
inline T exMax(T a, T b)
{
    return a > b ? a : b;
}

inline glm::vec2 exMax(const glm::vec2& a, const glm::vec2& b)
{
    return glm::vec2(exMax(a.x, b.x), exMax(a.y, b.y));
}

struct exAABB
{
    /// Verify that the bounds are sorted.
    bool IsValid() const;

    /// Get the center of the AABB.
    glm::vec2 GetCenter() const
    {
        return 0.5f * (lowerBound + upperBound);
    }

    /// Get the extents of the AABB (half-widths).
    glm::vec2 GetExtents() const
    {
        return 0.5f * (upperBound - lowerBound);
    }

    /// Get the perimeter length
    float GetPerimeter() const
    {
        float wx = upperBound.x - lowerBound.x;
        float wy = upperBound.y - lowerBound.y;
        return 2.0f * (wx + wy);
    }

    /// Combine an AABB into this one.
    void Combine(const exAABB& aabb)
    {
        lowerBound = exMin(lowerBound, aabb.lowerBound);
        upperBound = exMax(upperBound, aabb.upperBound);
    }

    /// Combine two AABBs into this one.
    void Combine(const exAABB& aabb1, const exAABB& aabb2)
    {
        lowerBound = exMin(aabb1.lowerBound, aabb2.lowerBound);
        upperBound = exMax(aabb1.upperBound, aabb2.upperBound);
    }

    /// Does this aabb contain the provided AABB.
    bool Contains(const exAABB& aabb) const
    {
        bool result = true;
        result = result && lowerBound.x <= aabb.lowerBound.x;
        result = result && lowerBound.y <= aabb.lowerBound.y;
        result = result && aabb.upperBound.x <= upperBound.x;
        result = result && aabb.upperBound.y <= upperBound.y;
        return result;
    }

    bool RayCast(exRayCastOutput* output, const exRayCastInput& input) const;

    glm::vec2 lowerBound;    ///< the lower vertex
    glm::vec2 upperBound;    ///< the upper vertex
};

struct  exColor
{
    exColor() {}
    exColor(float rIn, float gIn, float bIn, float aIn = 1.0f)
    {
        r = rIn; g = gIn; b = bIn; a = aIn;
    }

    void Set(float rIn, float gIn, float bIn, float aIn = 1.0f)
    {
        r = rIn; g = gIn; b = bIn; a = aIn;
    }

    float r, g, b, a;
};

class DebugDrawEx
{
public:
    DebugDrawEx();
    ~DebugDrawEx();

    void Create();
    void Destroy();

    void DrawPolygon(const glm::vec2* vertices, int vertexCount, const exColor& color) ;

    void DrawSolidPolygon(const glm::vec2* vertices, int vertexCount, const exColor& color) ;

    void DrawCircle(const glm::vec2& center, float radius, const exColor& color) ;

    void DrawSolidCircle(const glm::vec2& center, float radius, const glm::vec2& axis, const exColor& color) ;

    void DrawSegment(const glm::vec2& p1, const glm::vec2& p2, const exColor& color) ;

    //void DrawTransform(const b2Transform& xf) ;

    void DrawPoint(const glm::vec2& p, float size, const exColor& color) ;

    void DrawAABB(exAABB* aabb, const exColor& color);

    void Flush(glm::mat4 viewMatrix);

    bool m_showUI;
    GLRenderPointsEx* m_points;
    GLRenderLinesEx* m_lines;
    GLRenderTrianglesEx* m_triangles;
};

extern DebugDrawEx g_DebugDrawEx;

#endif /* debugdrawex_hpp */

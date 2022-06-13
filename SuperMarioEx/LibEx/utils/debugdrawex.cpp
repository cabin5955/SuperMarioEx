//
//  debugdrawex.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/11.
//

#include "debugdrawex.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "resource_manager.hpp"
#include "global.hpp"

#define BUFFER_OFFSET(x)  ((const void*) (x))
#define GL_PROGRAM_POINT_SIZE 0x8642

DebugDrawEx g_DebugDrawEx;

//
static void exCheckGLError()
{
    GLenum errCode = glGetError();
    if (errCode != GL_NO_ERROR)
    {
        fprintf(stderr, "OpenGL error = %d\n", errCode);
        assert(false);
    }
}

//
struct GLRenderPointsEx
{
    void Create()
    {
        Shader shader = ResourceManager::GetShader("b2points");
        m_programId = shader.ID;
        shader.use();
        m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
        m_viewUniform = glGetUniformLocation(m_programId, "viewMatrix");
        m_vertexAttribute = 0;
        m_colorAttribute = 1;
        m_sizeAttribute = 2;

        // Generate
        glGenVertexArrays(1, &m_vaoId);
        glGenBuffers(3, m_vboIds);

        glBindVertexArray(m_vaoId);
        glEnableVertexAttribArray(m_vertexAttribute);
        glEnableVertexAttribArray(m_colorAttribute);
        glEnableVertexAttribArray(m_sizeAttribute);

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
        glVertexAttribPointer(m_sizeAttribute, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_sizes), m_sizes, GL_DYNAMIC_DRAW);

        exCheckGLError();

        // Cleanup
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_count = 0;
    }

    void Destroy()
    {
        if (m_vaoId)
        {
            glDeleteVertexArrays(1, &m_vaoId);
            glDeleteBuffers(3, m_vboIds);
            m_vaoId = 0;
        }

        if (m_programId)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }
    }

    void Vertex(const glm::vec2& v, const exColor& c, float size)
    {
        if (m_count == e_maxVertices)
            Flush(m_view);

        m_vertices[m_count] = v;
        m_colors[m_count] = c;
        m_sizes[m_count] = size;
        ++m_count;
    }

    void Flush(glm::mat4 viewMatrix)
    {
        if (m_count == 0)
            return;

        m_view = viewMatrix;
        glUseProgram(m_programId);
        
        int w = Global::ScreenWidth;
        int h = Global::ScreenHeight;
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w/PPM),
                                            0.0f, static_cast<GLfloat>(h/PPM),
                                                -1.0f, 1.0f);

        glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glBindVertexArray(m_vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec2), m_vertices);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(exColor), m_colors);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(float), m_sizes);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, m_count);
        glDisable(GL_PROGRAM_POINT_SIZE);

        //sCheckGLError();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        m_count = 0;
    }

    enum { e_maxVertices = 512 };
    glm::vec2 m_vertices[e_maxVertices];
    exColor m_colors[e_maxVertices];
    float m_sizes[e_maxVertices];

    int m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[3];
    GLuint m_programId;
    GLint m_projectionUniform;
    GLint m_viewUniform;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
    GLint m_sizeAttribute;
    
    glm::mat4 m_view = glm::mat4(1.0);
};

//
struct GLRenderLinesEx
{
    void Create()
    {
        Shader shader = ResourceManager::GetShader("b2lines");
        m_programId = shader.ID;
        shader.use();
        m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
        m_viewUniform = glGetUniformLocation(m_programId, "viewMatrix");
        m_vertexAttribute = 0;
        m_colorAttribute = 1;

        // Generate
        glGenVertexArrays(1, &m_vaoId);
        glGenBuffers(2, m_vboIds);

        glBindVertexArray(m_vaoId);
        glEnableVertexAttribArray(m_vertexAttribute);
        glEnableVertexAttribArray(m_colorAttribute);

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

        exCheckGLError();

        // Cleanup
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_count = 0;
    }

    void Destroy()
    {
        if (m_vaoId)
        {
            glDeleteVertexArrays(1, &m_vaoId);
            glDeleteBuffers(2, m_vboIds);
            m_vaoId = 0;
        }

        if (m_programId)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }
    }

    void Vertex(const glm::vec2& v, const exColor& c)
    {
        if (m_count == e_maxVertices)
            Flush(m_view);

        m_vertices[m_count] = v;
        m_colors[m_count] = c;
        ++m_count;
    }

    void Flush(glm::mat4 viewMatrix)
    {
        if (m_count == 0)
            return;

        m_view = viewMatrix;
        glUseProgram(m_programId);
        
        int w = Global::ScreenWidth;
        int h = Global::ScreenHeight;
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w/PPM),
                                            0.0f, static_cast<GLfloat>(h/PPM),
                                                -1.0f, 1.0f);

        glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        glBindVertexArray(m_vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec2), m_vertices);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(exColor), m_colors);

        glDrawArrays(GL_LINES, 0, m_count);

        //sCheckGLError();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        m_count = 0;
    }

    enum { e_maxVertices = 2 * 512 };
    glm::vec2 m_vertices[e_maxVertices];
    exColor m_colors[e_maxVertices];

    int m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[2];
    GLuint m_programId;
    GLint m_projectionUniform;
    GLint m_viewUniform;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
    
    glm::mat4 m_view = glm::mat4(1.0);
};

//
struct GLRenderTrianglesEx
{
    void Create()
    {
        Shader shader = ResourceManager::GetShader("b2triangles");
        m_programId = shader.ID;
        shader.use();
        m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
        m_viewUniform = glGetUniformLocation(m_programId, "viewMatrix");
        m_vertexAttribute = 0;
        m_colorAttribute = 1;

        // Generate
        glGenVertexArrays(1, &m_vaoId);
        glGenBuffers(2, m_vboIds);

        glBindVertexArray(m_vaoId);
        glEnableVertexAttribArray(m_vertexAttribute);
        glEnableVertexAttribArray(m_colorAttribute);

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

        exCheckGLError();

        // Cleanup
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_count = 0;
    }

    void Destroy()
    {
        if (m_vaoId)
        {
            glDeleteVertexArrays(1, &m_vaoId);
            glDeleteBuffers(2, m_vboIds);
            m_vaoId = 0;
        }

        if (m_programId)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }
    }

    void Vertex(const glm::vec2& v, const exColor& c)
    {
        if (m_count == e_maxVertices)
            Flush(m_view);

        m_vertices[m_count] = v;
        m_colors[m_count] = c;
        ++m_count;
    }

    void Flush(glm::mat4 viewMatrix)
    {
        if (m_count == 0)
            return;

        m_view = viewMatrix;
        glUseProgram(m_programId);
        
        int w = Global::ScreenWidth;
        int h = Global::ScreenHeight;
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w/PPM),
                                          0.0f, static_cast<GLfloat>(h/PPM),
                                          -1.0f, 1.0f);
        
        glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glBindVertexArray(m_vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec2), m_vertices);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(exColor), m_colors);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, m_count);
        glDisable(GL_BLEND);

        //sCheckGLError();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        m_count = 0;
    }

    enum { e_maxVertices = 3 * 512 };
    glm::vec2 m_vertices[e_maxVertices];
    exColor m_colors[e_maxVertices];

    int m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[2];
    GLuint m_programId;
    GLint m_projectionUniform;
    GLint m_viewUniform;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
    glm::mat4 m_view = glm::mat4(1.0);
};

//
DebugDrawEx::DebugDrawEx()
{
    m_showUI = true;
    m_points = NULL;
    m_lines = NULL;
    m_triangles = NULL;
}

//
DebugDrawEx::~DebugDrawEx()
{
    assert(m_points == NULL);
    assert(m_lines == NULL);
    assert(m_triangles == NULL);
}

//
void DebugDrawEx::Create()
{
    m_points = new GLRenderPointsEx;
    m_points->Create();
    m_lines = new GLRenderLinesEx;
    m_lines->Create();
    m_triangles = new GLRenderTrianglesEx;
    m_triangles->Create();
}

//
void DebugDrawEx::Destroy()
{
    m_points->Destroy();
    delete m_points;
    m_points = NULL;

    m_lines->Destroy();
    delete m_lines;
    m_lines = NULL;

    m_triangles->Destroy();
    delete m_triangles;
    m_triangles = NULL;
}

//
void DebugDrawEx::DrawPolygon(const glm::vec2* vertices, int vertexCount, const exColor& color)
{
    glm::vec2 p1 = vertices[vertexCount - 1];
    for (int i = 0; i < vertexCount; ++i)
    {
        glm::vec2 p2 = vertices[i];
        m_lines->Vertex(p1, color);
        m_lines->Vertex(p2, color);
        p1 = p2;
    }
}

//
void DebugDrawEx::DrawSolidPolygon(const glm::vec2* vertices, int vertexCount, const exColor& color)
{
    exColor fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

    for (int i = 1; i < vertexCount - 1; ++i)
    {
        m_triangles->Vertex(vertices[0], fillColor);
        m_triangles->Vertex(vertices[i], fillColor);
        m_triangles->Vertex(vertices[i + 1], fillColor);
    }

    glm::vec2 p1 = vertices[vertexCount - 1];
    for (int i = 0; i < vertexCount; ++i)
    {
        glm::vec2 p2 = vertices[i];
        m_lines->Vertex(p1, color);
        m_lines->Vertex(p2, color);
        p1 = p2;
    }
}

//
void DebugDrawEx::DrawCircle(const glm::vec2& center, float radius, const exColor& color)
{
    const float k_segments = 16.0f;
    const float k_increment = 2.0f * ex_pi / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    glm::vec2 r1(1.0f, 0.0f);
    glm::vec2 v1 = center + radius * r1;
    for (int i = 0; i < k_segments; ++i)
    {
        // Perform rotation to avoid additional trigonometry.
        glm::vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        glm::vec2 v2 = center + radius * r2;
        m_lines->Vertex(v1, color);
        m_lines->Vertex(v2, color);
        r1 = r2;
        v1 = v2;
    }
}

//
void DebugDrawEx::DrawSolidCircle(const glm::vec2& center, float radius, const glm::vec2& axis, const exColor& color)
{
    const float k_segments = 16.0f;
    const float k_increment = 2.0f * ex_pi / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    glm::vec2 v0 = center;
    glm::vec2 r1(cosInc, sinInc);
    glm::vec2 v1 = center + radius * r1;
    exColor fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    for (int i = 0; i < k_segments; ++i)
    {
        // Perform rotation to avoid additional trigonometry.
        glm::vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        glm::vec2 v2 = center + radius * r2;
        m_triangles->Vertex(v0, fillColor);
        m_triangles->Vertex(v1, fillColor);
        m_triangles->Vertex(v2, fillColor);
        r1 = r2;
        v1 = v2;
    }

    r1.x = 1.0f;
    r1.y = 0.0f;
    v1 = center + radius * r1;
    for (int i = 0; i < k_segments; ++i)
    {
        glm::vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        glm::vec2 v2 = center + radius * r2;
        m_lines->Vertex(v1, color);
        m_lines->Vertex(v2, color);
        r1 = r2;
        v1 = v2;
    }

    // Draw a line fixed in the circle to animate rotation.
    glm::vec2 p = center + radius * axis;
    m_lines->Vertex(center, color);
    m_lines->Vertex(p, color);
}

//
void DebugDrawEx::DrawSegment(const glm::vec2& p1, const glm::vec2& p2, const exColor& color)
{
    m_lines->Vertex(p1, color);
    m_lines->Vertex(p2, color);
}

//
void DebugDrawEx::DrawPoint(const glm::vec2& p, float size, const exColor& color)
{
    m_points->Vertex(p, color, size);
}

//
void DebugDrawEx::DrawAABB(exAABB* aabb, const exColor& c)
{
    glm::vec2 p1 = aabb->lowerBound;
    glm::vec2 p2 = glm::vec2(aabb->upperBound.x, aabb->lowerBound.y);
    glm::vec2 p3 = aabb->upperBound;
    glm::vec2 p4 = glm::vec2(aabb->lowerBound.x, aabb->upperBound.y);

    m_lines->Vertex(p1, c);
    m_lines->Vertex(p2, c);

    m_lines->Vertex(p2, c);
    m_lines->Vertex(p3, c);

    m_lines->Vertex(p3, c);
    m_lines->Vertex(p4, c);

    m_lines->Vertex(p4, c);
    m_lines->Vertex(p1, c);
}

//
void DebugDrawEx::Flush(glm::mat4 viewMatrix)
{
    m_triangles->Flush(viewMatrix);
    m_lines->Flush(viewMatrix);
    m_points->Flush(viewMatrix);
}

#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexPosition;
    glm::vec3 Tangent;
};

struct VertexT{
    glm::vec3 Position;
    glm::vec2 TexPosition;
    glm::vec3 Normal;
};

struct VertexN{
    glm::vec3 Position;
    glm::vec3 Normal;
};


enum class GlTypes { Float, Int };
enum class GlBool { True, False };


class VertexArray{
public:
    VertexArray(){}
    ~VertexArray(){}

    int
    mapToGl( GlTypes t) {
        const int lut[] = {
                GL_FLOAT, GL_INT };
        return lut[ static_cast<int>( t )];
    }

    int
    mapToGl( GlBool b ) {
        const int lut[] = {
                GL_FALSE, GL_TRUE };
        return lut[ static_cast<int>( b )];
    }

    template<typename T>
    void
    createVertexArray(const std::vector<T> &vertices ){
       glGenVertexArrays(1, &m_VAO);
       glGenBuffers(1, &m_VBO);
       glBindVertexArray(m_VAO);
       glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
       glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(T), &vertices[ 0 ], GL_STATIC_DRAW);
       glBindBuffer(GL_ARRAY_BUFFER, 0);
       glBindVertexArray(0);
       m_Size = vertices.size();
    }

    template<typename T>
    void
    createIndexBuffer(const std::vector<T> & vertices, std::vector<unsigned int> const & indices ) {
       glGenVertexArrays(1, &m_VAO);
       glGenBuffers(1, &m_VBO);
       glGenBuffers(1, &m_IBO);
       glBindVertexArray(m_VAO);
       glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
       glEnableVertexAttribArray(0);
       glBindVertexArray(0);
    }

    template<typename T>
    void
    bindSimpleVBO(std::vector<T> const & vertices, GlTypes t, int shaderPos) {
       GLuint vbo;
       glBindVertexArray( m_VAO );
       glGenBuffers( 1, &vbo );
       glBindBuffer( GL_ARRAY_BUFFER, vbo );
       glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(T), &vertices[ 0 ], GL_STATIC_DRAW);
       switch(t) {
          case GlTypes::Float : glVertexAttribPointer( shaderPos, vertices.size(), GL_FLOAT, GL_FALSE, vertices.size()*sizeof( GLfloat ), (GLvoid*)(0) ); break;
          case GlTypes::Int   : glVertexAttribIPointer(shaderPos, vertices.size(), GL_INT, vertices.size()*sizeof(GLint), (GLvoid*)(0)); break;
       }
       glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4*sizeof( GLfloat ), (GLvoid*)(0 * sizeof(GLfloat) ) );
       glEnableVertexAttribArray(3);
       glBindBuffer( GL_ARRAY_BUFFER, 0 );
       glBindVertexArray( 0 );
       m_Size += vertices.size();
    }

    void
    bindVertexArray() {
       glBindVertexArray(m_VAO);
    }

    void
    describeVertexArray(int shaderPos, int numVertexElements, GlTypes t, int vecSize, GlBool normalized, int stride);

    int
    getSize() {
       return m_Size;
    }


private:
    GLuint m_VBO;
    GLuint m_VAO;
    GLuint m_IBO;
    int    m_Size;
};

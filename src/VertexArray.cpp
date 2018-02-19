#include "VertexArray.hpp"
int
map_to_gl( GlTypes t) {
    const int lut[] = {
            GL_FLOAT };
    return lut[ static_cast<int>( t )];
}

int
map_to_gl( GlBool b ) {
    const int lut[] = {
            GL_FALSE, GL_TRUE };
    return lut[ static_cast<int>( b )];
}


void
VertexArray::createVertexArray(std::vector<float> const &vertices) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof( GLfloat ), &vertices[ 0 ], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    m_NoV = vertices.size();
}

void
VertexArray::createVertexArray(std::vector<glm::vec3> const &vertices) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof( glm::vec3 ), &vertices[ 0 ], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    m_NoV = vertices.size();
}

void
VertexArray::createVertexArray( const std::vector<Vertex> &vertices ) {
    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof( Vertex ), &vertices[ 0 ], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    m_NoV = vertices.size();
}

void
VertexArray::createVertexArray( const std::vector<VertexN> &vertices ) {
    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof( VertexN ), &vertices[ 0 ], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    m_NoV = vertices.size();
}

void
VertexArray::createIndexBuffer(const std::vector<float> &vertices, const std::vector<unsigned int> &indices )
 {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void
VertexArray::createIndexBuffer(const std::vector<VertexT> &vertices, const std::vector<unsigned int> &indices ) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexT)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

}

GLuint
VertexArray::getVertexArray() {
    return m_VAO;
}
void
VertexArray::describeVertexArray(int shaderPos, int number, GlTypes t, int nov, GlBool normalized, int stride ) {
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glEnableVertexAttribArray( shaderPos );
    glVertexAttribPointer( shaderPos, number, map_to_gl( t ), map_to_gl( normalized ), nov * sizeof( GLfloat ), (GLvoid*)(stride * sizeof(GLfloat) ) );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}
void
VertexArray::bindVertexArray() {
    glBindVertexArray(m_VAO);
}

void
VertexArray::bindVBO(std::vector<glm::ivec3> const & vertices) {
   GLuint vbo;
   glBindVertexArray( m_VAO );
   glGenBuffers( 1, &vbo );
   glBindBuffer( GL_ARRAY_BUFFER, vbo );
   glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof( glm::ivec3 ), &vertices[ 0 ], GL_STATIC_DRAW );
   glVertexAttribIPointer(2,3,GL_INT, 0, nullptr);
   glEnableVertexAttribArray(2);
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindVertexArray( 0 );
}

void
VertexArray::bindVBO(std::vector<glm::fvec3> const & vertices) {
   GLuint vbo;
   glBindVertexArray( m_VAO );
   glGenBuffers( 1, &vbo );
   glBindBuffer( GL_ARRAY_BUFFER, vbo );
   glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::fvec3 ), &vertices[ 0 ], GL_STATIC_DRAW);
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof( GLfloat ), (GLvoid*)(0 * sizeof(GLfloat) ) );
   glEnableVertexAttribArray(3);
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindVertexArray( 0 );
}

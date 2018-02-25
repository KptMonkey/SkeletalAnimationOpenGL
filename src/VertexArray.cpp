#include "VertexArray.hpp"

void
VertexArray::describeVertexArray(int shaderPos, int numVertexElements, GlTypes t, int vecSize, GlBool normalized, int stride) {
   glBindVertexArray(m_VAO);
   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glEnableVertexAttribArray(shaderPos);
   switch(t) {
      case GlTypes::Float : glVertexAttribPointer( shaderPos, numVertexElements, mapToGl( t ), mapToGl( normalized ), vecSize*sizeof( GLfloat ), (GLvoid*)(stride * sizeof(GLfloat) ) ); break;
      case GlTypes::Int   : glVertexAttribIPointer(shaderPos, numVertexElements, mapToGl( t ), vecSize*sizeof(GLint), (GLvoid*)(stride*sizeof(GLint) )); break;
   }
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindVertexArray( 0 );
}

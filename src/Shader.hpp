#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

class Shader{
public:
   Shader();
   ~Shader(){}

   void
   bindShader(std::string path);

   bool
   readShader(std::string path, std::string & shader);

   void
   activate();

   Shader
   operator[](std::string const & name);

   void
   operator= (glm::mat4 const & mat);

   void
   operator= (glm::vec4 const & vec);

   void
   operator= (glm::vec3 const & vec);

   void
   operator= (int const & num);

   void
   operator= (float const & num);

   GLuint m_Program;
private:
   GLuint m_Location;
};

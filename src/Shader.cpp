#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader():m_Program( glCreateProgram() ){}

bool
Shader::readShader(std::string path, std::string & shader){

   std::ifstream file( path );
   std::string line;
   std::ostringstream oss;
   if ( !file )
   {
     std::cerr << "Couldn't read shader file! \n";
     return false;
   }
   else{
     while( std::getline( file, line ) ){
     oss << line << "\n";
     }
     shader = std::move(oss.str());
     return true;
   }
}
void
Shader::bindShader( std::string path ) {

   std::string shaderString;
   readShader(path, shaderString);;
   const GLchar * shaderSource = shaderString.c_str();
   auto shaderType = path.substr(( path.size() - 4 ), path.size());

   GLuint shader;
   if (shaderType == "frag") shader = glCreateShader(GL_FRAGMENT_SHADER);
   if (shaderType == "vert") shader = glCreateShader(GL_VERTEX_SHADER);
   if (shaderType == "geom") shader = glCreateShader(GL_GEOMETRY_SHADER);

   glShaderSource(shader, 1, &shaderSource, nullptr);
   glCompileShader(shader);
   GLint success;
   GLchar infoLog[512];
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (!success) {
     glGetShaderInfoLog(shader, 512, NULL, infoLog);
     std::cout << path << "\n";
     std::cout << "ERROR::SHADER::" + shaderType +" ::COMPILATION_FAILED\n" << infoLog << "\n";
   }

   glAttachShader(m_Program, shader);
   glLinkProgram( m_Program );
   glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
   if (!success) {
     glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
     std::cout << path << "\n";
     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
   }
   glDeleteShader(shader);
}

void
Shader::activate() {
   glUseProgram( m_Program );
}

Shader
Shader::operator[](std::string const & name) {
   m_Location = glGetUniformLocation(m_Program, name.c_str());
   return *this;
}

//TODO Combine this stuff into one operator
void
Shader::operator=(glm::mat4 const & mat){
   glUniformMatrix4fv(m_Location, 1, GL_FALSE, glm::value_ptr(mat));
}

void
Shader::operator=(glm::vec4 const & vec){
   glUniform4fv(m_Location, 1, glm::value_ptr(vec));
}

void
Shader::operator=(glm::vec3 const & vec){
   glUniform3fv(m_Location, 1, glm::value_ptr(vec));
}

void
Shader::operator=(int const & num){
   glUniform1i(m_Location, num);
}

void
Shader::operator=(float const & num){
   glUniform1f(m_Location, num);
}

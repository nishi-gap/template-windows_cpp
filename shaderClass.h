#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/gl.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const std::string& filename);
GLboolean printProgramInfoLog(GLuint program);

class Shader
{
public:
	
	GLuint ID;// Reference ID of the Shader Program
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const std::string& vertexFile, const std::string& fragmentFile);

	
	void Activate();// Activates the Shader Program
	void Delete();// Deletes the Shader Program
};
#endif
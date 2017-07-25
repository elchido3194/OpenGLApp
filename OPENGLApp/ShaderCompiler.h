#ifndef SHADER_H
#define SHADER_H
//#include <glew.h>
//GLAD
#include <glad\glad.h>
//GLFW
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Shader {
public:
	//id del programa
	unsigned int ID;
	//constructor que recibe los parametros de los shaders (fragment y vertex)
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		//Obtener el codigo fuente de los shaders del path
		std::string vertexShaderCode;
		std::string fragmentShaderCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//abrir los archivos
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//leer los contenidos de los archivos dentro de los streams

			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();

			//cerrar los handlers de los archivos
			vertexShaderFile.close();
			fragmentShaderFile.close();

			//convertir el stream en string

			vertexShaderCode = vShaderStream.str();
			fragmentShaderCode = fShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR: No se pudo leer el archivo del Shader" << std::endl;
		}

		const char* vShaderCode = vertexShaderCode.c_str();
		const char* fShaderCode = fragmentShaderCode.c_str();

		//2. Compilar los shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		//Compilar el vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		//Imprime errores de compilacion, si existen
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "Error en la compilacion del vertex Shader\n"<< infoLog << std::endl;
		}

		//Compilar el fragment Shader

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "Error en la compilacion del fragment Shader\n" << infoLog << std::endl;
		}


		//Crear el programa Shader y linkear los shaders

		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);
		glLinkProgram(this->ID);

		//imprimir los errores en caso de haberlos

		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "Ërror en el link del programa de shader" << infoLog << std::endl;
		}

		//borrar los shaders que han sido linkeados al programa de shader

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	

	void use() {
		glUseProgram(ID);
	}

	void setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMat4x4(const std::string &name, float* value) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
	}
};

#endif






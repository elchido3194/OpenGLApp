//GLAD
#include <glad\glad.h>
//GLFW
#include <glfw3.h>
#include <iostream>
//Shaders
#include "ShaderCompiler.h"
//textures
#include "stb_image.h"
//Math
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#define LOG(x) std::cout << x << std::endl;
using namespace std;

float valor = 0.5f;

void add() {
	valor = valor + 0.1f;
}

void sub() {

	valor = valor - 0.1f;
}

void clearGreen(GLFWwindow* window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_UP) {
		add();
	}
	if (key == GLFW_KEY_DOWN) {
		sub();
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}


int main() {

	//Inicializamos GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearningOpengl", nullptr, nullptr);
	if (window == nullptr) {
		LOG("Failed to create GLFW window");
		glfwTerminate;
		return -1;
	}
	//Hacemos el contexto
	glfwMakeContextCurrent(window);

	//Cargamos GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG("Failed to initializa GLAD");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	GLfloat vertices[] = {
		//posiciones 		 //Colores 			//Textura
		0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};

	//Definimos los indices
	GLuint indices[] = {
		0, 1, 3, //Primer triangulo (superior)
		1, 2, 3  //Segundo Triangulo (inferior)
	};

	

	//Creamos los buffers para pasar el array hacia el vertex Shader
	GLuint VBO1;
	glGenBuffers(1, &VBO1);

	//Compilamos y linkeamos los shaders
	Shader shader("vertexShader.vs", "fragmentShader.fs");

	//Creamos el vertex array para bindear los VBOs
	GLuint VAO1;
	glGenVertexArrays(1, &VAO1);

	//El EBO permite utilizar la matriz de vertices
	GLuint EBO;
	glGenBuffers(1, &EBO);

	//Bindeamos el VAO para definir sus caracteristicas
	glBindVertexArray(VAO1);
	//Como tenemos ya bindeado el VAO1, damos las caracteristicas.
	//Bindeamos el VBO1 al VAO1
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	//Le ensenamos al ArrayBuffer (VBO) que datos debe leer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Luego se realiza el bind del Element Buffer Objects EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Le ensenamos como leer los buffers
	//Para el location 0, utilizamos 3 datos (vertices del triangulo), de tipo float, con normalizacion falsa,
	//el sizeof(float) es 8 ya que tenemos 8 datos (3 vertices, 3 colores y 2 texturas) y el offset es 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//location es 1 (para los colores) y el offset es de 3 (ya que para cada vertex set, los primeros 3 datos son de vertices
	//y el segundo es de colores)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//location 2 es para las texturas 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Unbind el VAO
	glBindVertexArray(0);


	//Creamos las texturas
	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	//----------------Para la textura 1
	//Damos la caracteristicas a la textura
	glBindTexture(GL_TEXTURE_2D, texture1);
	//Parametros de la textura
	//Para el eje X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//Para el eje Y
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Este color unicamente se define si CLAMP_TO_BORDER ha sido implementado
	float borderColor[] = {
		1.0f, 0.0f, 0.0f, 0.0f
	};

	//Pasamos el color a los parametros de la textura

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Ahora utilizamos stb_image para cargas las texturas
	int width, height, nrChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannel, 0);

	//Generamos la textura mediante la fucion glteximage2d
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//generamos el Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	//------------------------------TEXTURA 2

	glBindTexture(GL_TEXTURE_2D, texture2);
	//Seteamos los parametros para la textura
	//Para el eje X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//Para el eje Y
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannel, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//generamos el Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	//utilizaremos el shader
	shader.use();

	//Seteamos los sampler uniforms de los shaders a las texture units 1 y 2

	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat time = glfwGetTime();

		GLfloat time2 = (sin((double)glfwGetTime())/2)+0.5;

		shader.use();
		glm::mat4 trans;
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		trans = glm::rotate(trans, (float)time, glm::vec3(0.0f, 0.0f, 1.0f));
		
		shader.setMat4x4("transform", glm::value_ptr(trans));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glm::mat4 trans2;
		trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans2 = glm::scale(trans2, glm::vec3(time2, time2, time2));
		shader.setMat4x4("transform", glm::value_ptr(trans2));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSetKeyCallback(window, key_callback);

		shader.setFloat("visible", valor);
		glfwSwapBuffers(window);

	}
	glfwTerminate();
	return 0;


}
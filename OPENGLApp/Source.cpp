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

#define LOG(x) std::cout << x << std::endl
#define screenWidth 800
#define screenHeight 600
//using namespace std;

float valor = 0.5f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw = -90.0f;
float pitch = 0.0f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 front;

void clearGreen(GLFWwindow* window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void inputProcess(GLFWwindow* window);

int main() {

	//Inicializamos GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearningOpengl", nullptr, nullptr);
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

	glViewport(0, 0, screenWidth, screenHeight);

	/*GLfloat vertices[] = {
		//posiciones 		 //Colores 			//Textura
		0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};*/

	glEnable(GL_DEPTH_TEST);
	/*GLfloat vertices[] = {
		0.5f,   0.5f, 0.0f, 1.0f, 1.0f,
		0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,   0.5f, 0.0f, 0.0f, 1.0f
	};*/
	//36 vertices para el CUBO (6 caras * 2 triangulos * 3 vertices)
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//location es 1 (para los colores) y el offset es de 3 (ya que para cada vertex set, los primeros 3 datos son de vertices
	//y el segundo es de colores)
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	//location 2 es para las texturas 
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

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
	unsigned char* data = stbi_load("cat.jpg", &width, &height, &nrChannel, 0);

	//Generamos la textura mediante la fucion glteximage2d
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//generamos el Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	//------------------------------TEXTURA 2
	/*
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
	*/
	//utilizaremos el shader
	shader.use();

	//Seteamos los sampler uniforms de los shaders a las texture units 1 y 2

	shader.setInt("texture1", 0);
	//shader.setInt("texture2", 1);
	
	float angle;
	glm::mat4 projec;
	projec = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);


	while (!glfwWindowShouldClose(window)) {

		float currentframe = glfwGetTime();
		deltaTime = currentframe - lastFrame;
		lastFrame = currentframe;

		//glfwPollEvents();
		inputProcess(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat time = glfwGetTime();

		// Esto permite calcular el tiempo delta, para determinar cuando se demora en renderizar el anterior frame
		//y poder syncronizar el movimiento dentro de la escena.

		GLfloat time2 = (sin((double)glfwGetTime())/2)+0.5;

		shader.use();
		//glm::mat4 trans;
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		//trans = glm::rotate(trans, (float)time, glm::vec3(0.0f, 0.0f, 1.0f));
		//shader.setMat4x4("transform", glm::value_ptr(trans));
		//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

		
		front.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
		

		//MAtrices de Transformacion de Sistemas Coordenados

		//1era Model
		glm::mat4 model;
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		//2da View
		//Trasladamos la escena hacia atras para dar la impresion que la camara se movio hacia adelante.
		
		glm::mat4 view;
		//view = glm::rotate(view, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));
		//Para girar la matriz de vista en el tiempo
		//GLfloat radius = 10.0f;
		//GLfloat CamX, CamZ;
		//CamX = cos(glfwGetTime())*radius;
		//CamZ = sin(glfwGetTime())*radius;

		//view = glm::lookAt(glm::vec3(CamX, 0.0f, CamZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		//3era Projection
		


		//Pasamos la Matriz de transformacion
		//shader.setMat4x4("transform", glm::value_ptr(trans));

		//Pasamos la matriz de model, view y projection
		shader.setMat4x4("model", glm::value_ptr(model));
		shader.setMat4x4("view", glm::value_ptr(view));
		shader.setMat4x4("projection", glm::value_ptr(projec));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO1);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		

		//glm::mat4 trans1 = trans;
		//glm::mat4 trans2 = trans;
		
		for (int  i = 0; i < 10; i++)
		{
			/*trans1 = glm::translate(trans1, glm::vec3(1.0f, 0.0f, 0.0f));
			trans1 = glm::scale(trans1, glm::vec3(0.5f, 0.5f, 0.5f));
			//trans1 = glm::rotate(trans1, (float)time, glm::vec3(0.0f, 0.0f, 1.0f));
			shader.setMat4x4("transform", glm::value_ptr(trans1));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			
			trans2 = glm::translate(trans2, glm::vec3(0.0f, 1.0f, 0.0f));
			trans2 = glm::scale(trans2, glm::vec3(0.5f, 0.5f, 0.5f));
			shader.setMat4x4("transform", glm::value_ptr(trans2));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			*/
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			angle = 50.0f * i;
			

			if (i % 3 ==0) {
				angle = (float)glfwGetTime()*50.0f;
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.5f, 0.3f));
			shader.setMat4x4("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
			
		}
		
		//glm::mat4 trans2;
		//trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
		//trans2 = glm::scale(trans2, glm::vec3(time2, time2, time2));
		//shader.setMat4x4("transform", glm::value_ptr(trans2));

		glBindVertexArray(0);


		shader.setFloat("visible", valor);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;


}

void inputProcess(GLFWwindow* window) {

	float cameraAcceleration = 1.0f*deltaTime;
	

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += cameraFront*cameraAcceleration;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition -= cameraFront*cameraAcceleration;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraAcceleration;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraAcceleration;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pitch += 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pitch -= 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		yaw += 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		yaw -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}
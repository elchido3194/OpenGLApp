//GLEW
#define GLEW_STATIC
#include <glew.h>
//GLFW
#include <glfw3.h>
#include <iostream>
using namespace std;

void clearGreen(GLFWwindow* window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//glfwSwapBuffers(window);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	/*if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

	}
	
	/*if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

	}*/

	
	//Cuando un usuario presione la tecla de escape, WindowShouldClose se pondra en true
	//Cerrando la aplicacion

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearningOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {

		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);

	//Vertices del triangulo a dibujar
	/*GLfloat vertices[] = {
		 0.5f,  0.5f, 0.0f, //Top Right
		 0.5f, -0.5f, 0.0f, //Bottom Right
		-0.5f, -0.5f, 0.0f, //Bottom Left
		-0.5f,  0.5f, 0.0f	//Top Left
	};*/

	GLfloat vertices[] = {
		 0.5f, -0.5f, 0.0f,
		 0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f 
	};

	/*GLfloat vertices[] = {
		//Primer Triangulo
		-0.25f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.1f, -0.5f, 0.0f,
		//Segundo Triangulo
		0.25f, 0.5f, 0.0f,
		0.1f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};*/

	GLuint indices[] = {
		0, 1, 3, //Primer triangulo (superior)
		1, 2, 3  //Segundo Triangulo (inferior)
	};

	//Creacion del buffer
	GLuint VBO1;
	//Genera un nombre o ID para el buffer
	glGenBuffers(1, &VBO1);

	GLuint VBO2;
	glGenBuffers(1, &VBO2);
	//VER CAST EN VAO
	//Binds a buffer to a buffer type
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Copia el contenido el array vertices al buffer VBO, siendo del mismo tamano
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Codigo del vertex shader en GLSL
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
//		"out vec4 vertexColor;\n"
		"void main ()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
//		"vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
		"}\0";

	const GLchar* fragmentShaderSource1 = "#version 330 core\n"
//		"in vec4 vertexColor;\n"
		"out vec4 color;\n"
		"uniform vec4 uniColor;\n"
		"void main()\n"
		"{\n"
		"color = uniColor;\n"
		//"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\0";

	const GLchar* fragmentShaderSource2 = "#version 330 core\n"
		"in vec4 vertexColor;\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vertexColor;\n"
		//"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\0";
	//Compilacion del VertexShader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Compilacion del Fragment Shader
	GLuint fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);

	GLuint fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	//Es necesario unir estos 2 shaders en un program
	GLuint shaderProgram1, shaderProgram2;
	shaderProgram1 = glCreateProgram();
	shaderProgram2 = glCreateProgram();
	
	//Adjuntamos los shaders antes programados al programa
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);

	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);

	//Linkeamos al programa
	glLinkProgram(shaderProgram1);
	glLinkProgram(shaderProgram2);

	//Borramos los shaders del program para evitar errores
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);

	//Linking vertex attributes
	//Le decimos al VBO como interpretar los datos de la matriz de vertices
	//1. El primer parametro es el vertex attribute que vamos a utilizar, en este caso es el 0 dado por location=0 en el vertex shader
	//2. Este parametro muestra el tamano del vertex attribute, en este caso es 3 por vec3 dado en el vertex shader
	//3. Tipo de datos a utilizar, es float por defecto en vec
	//4. Permite determinar si queremos normalizar los datos o no
	//5. Muestra el "stride", que es el tamano entre los diferentes datos que son pasados. En este caso se utiliza 3 porque son 3 flotantes.
	//6. Es el "offset" de donde comienza los datos en el buffer. Para este caso es 0 ya que no tiene offset. Necesita ese cast raro de GLvoid*
	//VER CAST EN VAO
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	//Creamos un VAO - Vertex Array Object que permite hacer un bind para que todos los subsiguientes llamadas a las VBO se guarden en dicho array
	GLuint VAO1;
	glGenVertexArrays(1, &VAO1);

	GLuint VAO2;
	glGenVertexArrays(1, &VAO2);
	//HAcemos el Bind del VAO (Vertex Array Object)
	glBindVertexArray(VAO1);

	//Esta parte estaba hecha antes, cuando no se utilizaba un VAO
	//En este caso se utiliza el VAO para evitar llamadas para cada objeto a dibujar.

	//Primero se realiza el bind del Vertex Buffer Object
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Luego se realiza el bind del Element Buffer Objects EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Realizamos los atributos del VAO 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

	//Unbind el VAO
	glBindVertexArray(0);

	glBindVertexArray(VAO2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

	glBindVertexArray(0);



	while (!glfwWindowShouldClose(window))
	{
		//Chequea si exitieron eventos (botones, cambios en el porte)
		glfwPollEvents();
		//Comandos para el rendering
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat time = glfwGetTime();

		
		GLfloat rojo = (sin(time) / 2) + 0.5;
		cout << rojo << endl;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram1, "uniColor");
		glUseProgram(shaderProgram1);
		glUniform4f(vertexColorLocation, rojo, rojo, rojo, 1.0f);

		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
/*
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		glBindVertexArray(0);
*/
		glfwSetKeyCallback(window, key_callback);
		//Intercambio de buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;

}

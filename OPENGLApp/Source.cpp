//GLEW
#define GLEW_STATIC
#include <glew.h>
//GLFW
#include <glfw3.h>
#include <iostream>

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
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f

	};

	//Creacion del buffer
	GLuint VBO;
	//Genera un nombre o ID para el buffer
	glGenBuffers(1, &VBO);
	//VER CAST EN VAO
	//Binds a buffer to a buffer type
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Copia el contenido el array vertices al buffer VBO, siendo del mismo tamano
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Codigo del vertex shader en GLSL
	const GLchar* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main ()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";

	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\0";

	//Compilacion del VertexShader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Compilacion del Fragment Shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Es necesario unir estos 2 shaders en un program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	
	//Adjuntamos los shaders antes programados al programa
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Linkeamos al programa
	glLinkProgram(shaderProgram);

	//Borramos los shaders del program para evitar errores
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Linking vertex attributes
	//Le decimos al vertex shader como debe leer los datos datos por la VBO
	//1. El primer parametro es el vertex attribute que vamos a utilizar, en este caso es el 0 dado por location=0 en el vertex shader
	//2. Este parametro muestra el tamano del vertex attribute, en este caso es 3 por vec3 dado en el vertex shader
	//3. Tipo de datos a utilizar, es float por defecto en vec
	//4. Permite determinar si queremos normalizar los datos o no
	//5. Muestra el "stride", que es el tamano entre los diferentes datos que son pasados. En este caso se utiliza 3 porque son 3 flotantes.
	//6. Es el "offset" de donde comienza los datos en el buffer. Para este caso es 0 ya que no tiene offset. Necesita ese cast raro de GLvoid*
	//VER CAST EN VAO
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	//Creamos un VAO - Vertex Array Object que permite hacer un bind para que todos los subsiguientes llamadas a las VBO se guarden en dicho array
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	//HAcemos el Bind del VAO
	glBindVertexArray(VAO);

	//Esta parte estaba hecha antes, cuando no se utilizaba un VAO
	//En este caso se utiliza el VAO para evitar llamadas para cada objeto a dibujar.

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

	//Unbind el VAO
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		//Chequea si exitieron eventos (botones, cambios en el porte)
		glfwPollEvents();
		//Comandos para el rendering
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSetKeyCallback(window, key_callback);
		//Intercambio de buffers
		glfwSwapBuffers(window);
		
	}
	glfwTerminate();
	return 0;

}

#version 330 core
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexture;

//out vec3 ourColor;
out vec2 textureCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main (){
	//Tomar en consideracion que la multiplicacion de las matrices esta hacia atras.
	//Primero multiplicamos la proyeccion, vista y modelo
	//gl_Position = transform * model vec4(position, 1.0);
	gl_Position = projection * view * model * vec4(position, 1.0);
	//ourColor = aColor;
	textureCoord = aTexture;
}
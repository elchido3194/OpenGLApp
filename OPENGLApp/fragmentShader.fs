#version 330 core
in vec3 ourColor;
in vec2 textureCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float visible;

void main()
{
	vec2 textureCoordInv = vec2(textureCoord.x, textureCoord.y);
	//color = vec4(ourColor, 1.0);
	//color = texture(ourTexture1, textureCoord) * vec4(ourColor, 1.0);
	color = mix(texture(texture1, textureCoord), texture(texture2, textureCoordInv), visible);
}
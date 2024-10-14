#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texturewall;
uniform sampler2D texturesmile;
uniform sampler2D textureblending;
uniform float radio;
void main()
{
    FragColor = mix(texture(texturewall, TexCoord),texture(texturesmile, TexCoord),radio);
}
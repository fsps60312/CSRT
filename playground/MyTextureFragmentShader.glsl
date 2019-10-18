#version 330 core

// Input data (from the vertex shaders)
in vec2 UV;

// Ouput data
layout (location = 0) out vec4 color;

// Values
uniform sampler2D RenderedTexture;

void main()
{
	color = texture(RenderedTexture, UV);
}

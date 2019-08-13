#version 330 core

// Input data (from code)
layout(location = 0) in vec3 vertexPosition_model;

// Ouput data
out vec2 UV;

void main()
{
	gl_Position = vec4(vertexPosition_model, 1.0f);
	
	UV = (vertexPosition_model.xy + vec2(1.0f)) / 2.0f;
}
#version 330

layout(location = 1) in vec4 vertex_position;
layout(location = 2) in vec4 vertex_color;

uniform mat4 model_mat;

out vec4 frag_color;
void main()
{
	gl_Position = model_mat * vertex_position;

	frag_color = vertex_color;
}
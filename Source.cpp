#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>

using namespace std;
using namespace glm;

const GLint WIDTH = 800, HEIGHT = 800;

struct Vertex
{
	vec3 Position;
	vec3 Color;
};

GLuint BasicProgramId;
mat4 ModelMat;
GLuint Theta_Location, model_mat_location;
GLuint VBO_Cube;
GLuint IBO_Cube;
float theta = 0;

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);


void CreateCube()
{
	Vertex cube_core_vertices[] = {
		{vec3(-0.5, 0.5, 0.5),vec3(1,0,0)},   //0
		{vec3(-0.5, -0.5, 0.5),vec3(0,1,0)},  //1
		{vec3(0.5, -0.5, 0.5),vec3(1,0,1)},   //2
		{vec3(0.5, 0.5, 0.5),vec3(0,0,1)},    //3
		{vec3(0.5, 0.5, -0.5),vec3(1,1,0)},   //4
		{vec3(0.5, -0.5, -0.5),vec3(0,1,1)},  //5
		{vec3(-0.5, -0.5, -0.5),vec3(1,1,1)}, //6
		{vec3(-0.5, 0.5, -0.5),vec3(0,0,0)}   //7
	};
	int vertices_Indeces[] = {
	0,1,2, 0,2,3,
	3,2,5, 3,5,4,
	4,5,6, 4,6,7,
	7,6,1, 7,1,0,
	7,0,3, 7,3,4,
	2,1,6, 2,6,5
	};

	glGenBuffers(1, &VBO_Cube);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_core_vertices), cube_core_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO_Cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_Indeces), vertices_Indeces, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(2);
}


void BindCube()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Cube);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(2);
}

void DrawCat()
{
	// Body
	BindCube();
	ModelMat = translate(vec3(0, 0, 0)) * rotate (theta,vec3(0,1,0)) * scale(vec3(1.0f, 0.6f, 0.4f));
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, value_ptr(ModelMat));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Head
	BindCube();
	ModelMat = translate(vec3(0, 0.6f, 0)) * rotate(theta, vec3(0, 1, 0)) * scale(vec3(0.6f));
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, value_ptr(ModelMat));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Left Ear
	BindCube();
	ModelMat = translate(vec3(-0.2f, 1.0f, 0)) * rotate(theta, vec3(0, 1, 0)) * scale(vec3(0.2f));
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, value_ptr(ModelMat));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Right Ear
	BindCube();
	ModelMat = translate(vec3(0.2f, 1.0f, 0)) * scale(vec3(0.2f));
	glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, value_ptr(ModelMat));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Legs (4)
	vec3 legOffsets[4] = {
		vec3(-0.5f, -0.5f, 0.3f), vec3(0.5f, -0.5f, 0.3f),
		vec3(-0.5f, -0.5f, -0.3f), vec3(0.5f, -0.5f, -0.3f)
	};
	for (int i = 0; i < 4; ++i)
	{
		BindCube();
		ModelMat = translate(legOffsets[i]) *rotate(theta, vec3(0, 1, 0)) * scale(vec3(0.2f, 0.4f, 0.2f));
		glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, value_ptr(ModelMat));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	}
}

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_name);
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", BasicProgramId);
	CreateCube();
	model_mat_location = glGetUniformLocation(BasicProgramId, "model_mat");
	glEnable(GL_DEPTH_TEST);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawCat();
}

void Update()
{
	// add all tick code
	theta += 0.01f;
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);

	if (Init()) return 1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{

			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		Update();
		Render();

		window.display();
	}
	return 0;
}
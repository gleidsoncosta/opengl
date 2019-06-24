
#include <stdio.h>
#include <string>


#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Include/ogldev_util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;


const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

static void RenderSceneCB ()
{
	glClear (GL_COLOR_BUFFER_BIT);

	static float Scale = 1.0f;

	Scale += 0.001f;

	mat4 trans = mat4 (1.0f);
	trans = rotate (trans, radians (Scale*10.0f), vec3 (0.5f, 0.1f, 1.0f));
	vec4 result = trans * vec4 (1.0f, 0.0f, 0.0f, 1.0f);
	
	//mat4 view = lookAt (vec3 (Scale*1.0f, Scale*1.0f, Scale*1.0f), vec3 (0.0f, 0.0f, 0.0f), vec3 (0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv (gWorldLocation, 1, GL_TRUE, value_ptr(trans));

	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, IBO);


	glDrawElements (GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray (0);

	glutSwapBuffers ();
}


static void InitializeGlutCallbacks ()
{
	glutDisplayFunc (RenderSceneCB);
	glutIdleFunc (RenderSceneCB);
}

static void CreateVertexBuffer ()
{
	vec3 Vertices[4];
	Vertices[0] = vec3 (-1.0f, -1.0f, 0.0f);
	Vertices[1] = vec3 (0.0f, -1.0f, 1.0f);
	Vertices[2] = vec3 (1.0f, -1.0f, 0.0f);
	Vertices[3] = vec3 (0.0f, 1.0f, 0.0f);

	glGenBuffers (1, &VBO);
	glBindBuffer (GL_ARRAY_BUFFER, VBO);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer ()
{
	unsigned int Indices[] = { 0, 3, 1,
							   1, 3, 2,
							   2, 3, 0,
							   0, 1, 2 };

	glGenBuffers (1, &IBO);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (Indices), Indices, GL_STATIC_DRAW);
}


static void AddShader (GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader (ShaderType);

	if (ShaderObj == 0) {
		fprintf (stderr, "Error creating shader type %d\n", ShaderType);
		exit (1);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen (pShaderText);
	glShaderSource (ShaderObj, 1, p, Lengths);
	glCompileShader (ShaderObj);
	GLint success;
	glGetShaderiv (ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog (ShaderObj, 1024, NULL, InfoLog);
		fprintf (stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit (1);
	}

	glAttachShader (ShaderProgram, ShaderObj);
}

static void CompileShaders ()
{
	GLuint ShaderProgram = glCreateProgram ();

	if (ShaderProgram == 0) {
		fprintf (stderr, "Error creating shader program\n");
		exit (1);
	}

	string vs, fs;

	vs = R"(
			#version 330

layout (location = 0) in vec3 Position;

uniform mat4 gWorld;

out vec4 Color;

void main()
{
    gl_Position = gWorld * vec4(Position, 1.0);
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
}
)";



	fs = R"(
				#version 330

in vec4 Color;

out vec4 FragColor;

void main()
{
    FragColor = Color;
}

				)";

	AddShader (ShaderProgram, vs.c_str (), GL_VERTEX_SHADER);
	AddShader (ShaderProgram, fs.c_str (), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram (ShaderProgram);
	glGetProgramiv (ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog (ShaderProgram, sizeof (ErrorLog), NULL, ErrorLog);
		fprintf (stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit (1);
	}

	glValidateProgram (ShaderProgram);
	glGetProgramiv (ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog (ShaderProgram, sizeof (ErrorLog), NULL, ErrorLog);
		fprintf (stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit (1);
	}

	glUseProgram (ShaderProgram);

	gWorldLocation = glGetUniformLocation (ShaderProgram, "gWorld");
	assert (gWorldLocation != 0xFFFFFFFF);
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (1024, 768);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Tutorial 05");

	InitializeGlutCallbacks ();

	// Must be done after glut is initialized!
	GLenum res = glewInit ();
	if (res != GLEW_OK) {
		fprintf (stderr, "Error: '%s'\n", glewGetErrorString (res));
		return 1;
	}

	printf ("GL version: %s\n", glGetString (GL_VERSION));

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer ();
	CreateIndexBuffer ();

	CompileShaders ();

	glutMainLoop ();

	return 0;
}
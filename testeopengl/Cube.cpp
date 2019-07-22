#include <stdio.h>
#include <string>

#include <math.h>

#include <GL/glew.h>
#include <soil.h>
#include <GL/freeglut.h>

#include "Include/ogldev_util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

GLuint VBO;
GLuint EBO;

GLuint model;
GLuint proj;
GLuint view;


//RENDERIZAR AS CENAS
static void RenderSceneCB ()
{
	//FIXO
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//OBJETO
	mat4 trans = mat4 (1.0f);
	trans = rotate (trans, radians (180.0f), vec3 (0.0f, 0.0f, 1.0f));
	
	glUniformMatrix4fv (model, 1, GL_FALSE, value_ptr (trans));

	//VISUALIZANDO
	mat4 look = lookAt (
		vec3 (1.2f, 1.2f, 1.2f), //posicao da camera
		vec3 (0.0f, 0.0f, 0.0f), //ponto centralizado da camera (pra onde esta olhando)
		vec3 (0.0f, 0.0f, 1.0f) // up axis, definido pelo ponto Z. xy é o plano e Z altura
	);

	glUniformMatrix4fv (view, 1, GL_FALSE, value_ptr (look));

	//MUNDO REAL
	mat4 projec = perspective (glm::radians (45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
	glUniformMatrix4fv (proj, 1, GL_FALSE, value_ptr (projec));


	// Draw a rectangle from the 2 triangles using 6 indices
	//glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays (GL_TRIANGLES, 0, 36);

	//FIXO
	glutSwapBuffers ();
}

//Inicializar as cenas de renderização
static void InitializeGlutCallbacks ()
{
	glutDisplayFunc (RenderSceneCB);
	glutIdleFunc (RenderSceneCB);
}

//criar os vertices e inserir dentro do buffer de vertices
static void CreateVertexBuffer ()
{
	GLfloat vertices[] = {
		/*//  Position      Color             Texcoords
	0	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FRONT Top-left
	1	 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // FRONT Top-right
	2	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // FRONT Bottom-right
	3	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // FRONT Bottom-left
	
	4	-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FRONT Top-left
	5	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // FRONT Top-right
	6	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // FRONT Bottom-right
	7	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // FRONT Bottom-left
	
	*/
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	glGenBuffers (1, &VBO);
	glBindBuffer (GL_ARRAY_BUFFER, VBO);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
}

//inserir o controle de conexão de vertices pelos indices
static void CreateIndexBuffer ()
{
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glGenBuffers (1, &EBO);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (elements), elements, GL_STATIC_DRAW);
}

// int main (int argc, char** argv)
int main2(int argc, char** argv)
{
	//Inicializar a janela
	 glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (1024, 768);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("OPENGL TESTE");

	//Inicializar as cenas
	InitializeGlutCallbacks ();

	//Precisa ser feito antes do glut ser inicializado
	GLenum res = glewInit ();
	if (res != GLEW_OK) {
		fprintf (stderr, "Error: '%s'\n", glewGetErrorString (res));
		return 1;
	}

	printf ("GL version: %s\n", glGetString (GL_VERSION));

	glEnable (GL_DEPTH_TEST);

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

	//Cria os vértices
	CreateVertexBuffer ();
	//Cria o buffer de indices
	//CreateIndexBuffer ();
	
	//Cria os shaders e texturas
	// Shader sources

	const GLchar* vertexSource = R"glsl(
											#version 150 core
    in vec3 position;
    in vec3 color;
    in vec2 texcoord;

    out vec3 Color;
    out vec2 Texcoord;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 proj;

    void main()
    {
        Color = color;
        Texcoord = texcoord;
        gl_Position = proj * view * model * vec4(position, 1.0);
    }
										)glsl";

	const GLchar* fragmentSource = R"glsl(
											#version 150 core
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D tex;
    void main()
    {
		if (Texcoord.y < 0.5)
            outColor = texture(tex, Texcoord);
        else
            outColor = texture(tex, vec2(Texcoord.x, 1.0 - Texcoord.y));
    }
										)glsl";
	
	GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShader, 1, &vertexSource, NULL);
	glCompileShader (vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader (fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram ();
	glAttachShader (shaderProgram, vertexShader);
	glAttachShader (shaderProgram, fragmentShader);
	glBindFragDataLocation (shaderProgram, 0, "outColor");
	glLinkProgram (shaderProgram);
	glUseProgram (shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation (shaderProgram, "position");
	glEnableVertexAttribArray (posAttrib);
	glVertexAttribPointer (posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);

	GLint colAttrib = glGetAttribLocation (shaderProgram, "color");
	glEnableVertexAttribArray (colAttrib);
	glVertexAttribPointer (colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), (void*)(3 * sizeof (GLfloat)));

	GLint texAttrib = glGetAttribLocation (shaderProgram, "texcoord");
	glEnableVertexAttribArray (texAttrib);
	glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), (void*)(6 * sizeof (GLfloat)));


	//CARREGAR Texturas
	GLuint textures[1];
	glGenTextures (1, textures);

	int width, height;
	unsigned char* image;

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, textures[0]);
	image = SOIL_load_image ("sample.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data (image);
	glUniform1i (glGetUniformLocation (shaderProgram, "texKitten"), 0);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	model = glGetUniformLocation (shaderProgram, "model");
	proj = glGetUniformLocation (shaderProgram, "proj");
	view = glGetUniformLocation (shaderProgram, "view");

	//Loop Principal
	glutMainLoop ();

	return 0;
}
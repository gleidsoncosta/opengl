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

GLfloat ctlpoints[16][16][3];
GLfloat ctltxpoints[16][16][2];

int sizex = 16;
int sizey = 16;

GLfloat anglerotx = 0.0f;
GLfloat angleroty = 0.0f;
GLfloat anglerotz = 180.0f;

GLfloat posx = 0.0f;
GLfloat posy = 0.0f;

int showPoints = 0;

GLUnurbsObj *theNurb;
GLuint textures[1];

void init_surface (void)
{
	float minx = 0.0f;
	float maxx = 10.0f;
	float stepx = ((float)10 / (float)(sizex-1));

	float miny = 0.0f;
	float maxy = 10.0f;
	float stepy = ((float)10 / (float)(sizey - 1));

	float minz = 0.0f;
	float maxz = 3.0f;
	float stepz = ((float)3 / (float)(sizey - 1));

	float minu = 0.0f;
	float maxu = 1.0f;
	float stepu = ((float)1 / (float)(sizex - 1));

	float minv = 0.0f;
	float maxv = 1.0f;
	float stepv = ((float)1 / (float)(sizey - 1));


	GLfloat x, y, z, u, v;

	for (int i = 0; i < sizex; i++) {
		for (int j = 0; j < sizey; j++) {
			x = minx + (i*stepx);
			y = miny + (j*stepy);
			if (i == 0 || i == sizex || (i>((int)sizex*0.4)&& i < ((int)sizex*0.6))) {
				z = -0.0f;
			}
			else{
				if (j >(int)sizey*0.5  && j<(int)sizey*0.9) {
					z = minz + (j*(stepz));
				}
				else {
					z = minz + (j*(stepz)*0.3);
				}
			}
			ctlpoints[i][j][0] = x; ctlpoints[i][j][1] = y; ctlpoints[i][j][2] = z;
			
			u = minu+(i*stepu);
			v = minv+(j*stepv);
			ctltxpoints[i][j][0] = u; ctltxpoints[i][j][1] = v;

			printf ("(%.3f %.3f %.3f)", x, y, z);
		}
		printf ("\n");
	}
	
}

void init (void)
{
	GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	
	glClearColor (0.0, 0.0, 0.0, 0.0);

	glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv (GL_FRONT, GL_SHININESS, mat_shininess);
	
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_AUTO_NORMAL);
	glEnable (GL_NORMALIZE);
	glEnable (GL_TEXTURE_2D);

	init_surface ();

	theNurb = gluNewNurbsRenderer ();
	gluNurbsProperty (theNurb, GLU_SAMPLING_TOLERANCE, 20.0);
	gluNurbsProperty (theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	
	//CARREGAR Texturas
	int width, height;
	unsigned char* image;
	image = SOIL_load_image ("breastthermo.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	
	glGenTextures (1, &textures[0]);
	glBindTexture (GL_TEXTURE_2D, textures[0]);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data (image);


}

void display (void)
{
	int knotsysize = 2 * sizey;
	int knotsxsize = 2 * sizex;

	GLfloat knotsy[32];
	GLfloat knotsx[32];
	GLfloat knotsytx[32];
	GLfloat knotsxtx[32];
	
	for (int i = 0; i < knotsysize; i++) {
		if (i < sizex) {
			knotsy[i] = 0.0;
			knotsytx[i] = 0.0;
		}
		else {
			knotsy[i] = 1.0;
			knotsytx[i] = 1.0;
		}
	}

	for (int i = 0; i < knotsxsize; i++) {
		if (i < sizex) {
			knotsx[i] = 0.0;
			knotsxtx[i] = 0.0;
		}
		else {
			knotsx[i] = 1.0;
			knotsxtx[i] = 1.0;
		}
	}

	int i, j;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix ();
	glTranslatef (posy, posx, 0.0f);
	glRotatef (anglerotx, 1, 0, 0);
	glRotatef (angleroty, 0, 1, 0);
	glRotatef (anglerotz, 0, 0, 1);
	glScalef (0.2, 0.2, 0.2);

	glBindTexture (GL_TEXTURE_2D, textures[0]);
	//glActiveTexture (GL_TEXTURE0);
	gluBeginSurface (theNurb);

	gluNurbsSurface (theNurb, knotsxsize, knotsxtx, knotsysize, knotsytx, sizey * 2, 2, &ctltxpoints[0][0][0], sizex, sizey, GL_MAP2_TEXTURE_COORD_2);
	gluNurbsSurface (theNurb, knotsxsize, knotsx, knotsysize, knotsy, sizey *3, 3, &ctlpoints[0][0][0], sizex, sizey, GL_MAP2_VERTEX_3);

	gluEndSurface (theNurb);

	if (showPoints) {
		glPointSize (6.0);
		glDisable (GL_LIGHTING);
		glColor3f (1.0, 1.0, 0.0);
		glBegin (GL_POINTS);
		for (i = 0; i < sizex; i++) {
			for (j = 0; j < sizey; j++) {
				glVertex3f (ctlpoints[i][j][0],
					ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd ();
		glEnable (GL_LIGHTING);
	}
	glPopMatrix ();
	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0, (GLdouble)w / (GLdouble)h, 3.0, 8.0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef (0.0, 0.0, -5.0);
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'c':
		case 'C':
		showPoints = !showPoints;
		if(!showPoints)	gluNurbsProperty (theNurb, GLU_DISPLAY_MODE, GLU_FILL);
		else gluNurbsProperty (theNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
		glutPostRedisplay ();
		break;
		case'r':
		case'R':
			ctlpoints[4][6][2] = ctlpoints[4][6][2]--;
			glutPostRedisplay ();
			break;
		case'f':
		case'F':
			ctlpoints[4][6][2] = ctlpoints[4][6][2]++;
			glutPostRedisplay ();
			break;
		case '8':
			posx+=0.1f;
			glutPostRedisplay ();
			break;
		case '2':
			posx-= 0.1f;
			glutPostRedisplay ();
			break;
		case '4':
			posy+= 0.1f;
			glutPostRedisplay ();
			break;
		case '6':
			posy-= 0.1f;
			glutPostRedisplay ();
			break;
		case'a':
		case'A':
			anglerotx++;
			glutPostRedisplay ();
		break;
		case's':
		case'S':
			angleroty++;
			glutPostRedisplay ();
			break;
		case'd':
		case'D':
			anglerotz++;
			glutPostRedisplay ();
			break;
		case'q':
		case'Q':
			anglerotx--;
			glutPostRedisplay ();
			break;
		case'w':
		case'W':
			angleroty--;
			glutPostRedisplay ();
			break;
		case'e':
		case'E':
			anglerotz--;
			glutPostRedisplay ();
			break;
		case'y':
		case'Y':
			ctltxpoints[0][0][0] += 0.1f;
			ctltxpoints[0][0][1] += 0.1f;
			glutPostRedisplay ();
			break;
		case'u':
		case'U':
			ctltxpoints[0][0][0] -= 0.1f;
			ctltxpoints[0][0][1] -= 0.1f;
			glutPostRedisplay ();
			break;
		case'i':
		case'I':
			ctltxpoints[3][3][0] += 0.1f;
			ctltxpoints[3][3][1] += 0.1f;
			glutPostRedisplay ();
			break;
		case'o':
		case'O':
			ctltxpoints[3][3][0] -= 0.1f;
			ctltxpoints[3][3][1] -= 0.1f;
			glutPostRedisplay ();
			break;
		case'h':
		case'H':
			ctltxpoints[4][0][0] += 0.1f;
			ctltxpoints[4][0][1] += 0.1f;
			glutPostRedisplay ();
			break;
		case'j':
		case'J':
			ctltxpoints[4][0][0] -= 0.1f;
			ctltxpoints[4][0][1] -= 0.1f;
			glutPostRedisplay ();
			break;
		case'k':
		case'K':
			ctltxpoints[4][4][0] += 0.1f;
			ctltxpoints[4][4][1] += 0.1f;
			glutPostRedisplay ();
			break;
		case'l':
		case'L':
			ctltxpoints[4][4][0] -= 0.1f;
			ctltxpoints[4][4][1] -= 0.1f;
			glutPostRedisplay ();
			break;
		case 27:
			exit (0);
			break;
		default:
			break;
	}
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutReshapeFunc (reshape);
	glutDisplayFunc (display);
	glutKeyboardFunc (keyboard);

	glutMainLoop ();
	return 0;
}
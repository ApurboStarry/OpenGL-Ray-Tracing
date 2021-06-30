#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>
#include "1605082_classes.h"

using namespace std;

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int levelOfRecursion;
int pixelCount;

vector<Object*> allObjects;
int numberOfObjects;

point pos;
point lookVector;
point rightVector;
point upVector;

void drawAxes()
{
	if(drawaxes==1)
	{
		glBegin(GL_LINES);{
		  glColor3f(1.0, 0, 0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

		  glColor3f(0, 1, 0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

		  glColor3f(0, 0, 1);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	for(int i = -100; i < 100; i++) {
		for(int j = -100; j < 100; j++) {
			if((i+j) % 2 == 0) {
				glColor3f(1.0, 1.0, 1.0);
			} else {
				glColor3f(0.0, 0.0, 0.0);
			}

			glBegin(GL_QUADS);
				{
					glVertex3f(1.0 * i, 1.0 * j, 0.0);
					glVertex3f(1.0 * i+1, 1.0 * j, 0.0);
					glVertex3f(1.0 * i+1, 1.0 * j+1, 0.0);
					glVertex3f(1.0 * i, 1.0 * j+1, 0.0);
				}
			glEnd();
		}
	}
}

void drawSquare(double a)
{
  glColor3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);{
		// WARNING: the order of the vertices matters
		// correct order: (i, j) -> (i+1, j) -> (i+1, j+1) -> (i, j+1)
		glVertex3f( a + 50, a + 100, 0);
		glVertex3f( a + 50,-a, 0);
		glVertex3f(-a - 50,-a, 0);
		glVertex3f(-a - 50, a + 100, 0);
	}glEnd();
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
					glVertex3f(0,0,height);
					glVertex3f(points[i].x,points[i].y,0);
					glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
    // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
    if(i%2) {
      glColor3f(1, 1, 1);
    } else {
      glColor3f(0, 0, 0);
    }

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
        //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
        //lower hemisphere
        glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawTheWeirdCone(double radius, int slices, int stacks) {
  point points[100][100];

  for(int j = 0; j <= stacks; j++) {
    for(int i=0;i<=slices;i++) {
        points[j][i].x = j;
        points[j][i].y=(radius + j)*cos(((double)i/(double)slices)*2*pi);
        points[j][i].z=(radius + j)*sin(((double)i/(double)slices)*2*pi);
    }
  }

  for(int i=0;i<stacks;i++)
	{
    // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
    if(i%2) {
      glColor3f(0, 0, 0);
    } else {
      glColor3f(1, 1, 1);
    }

		for(int j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
        //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
        //lower hemisphere
        // glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				// glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				// glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				// glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius, double height, int slices, int stacks) {
  point points[100][100];

  for(int j = 0; j <= stacks; j++) {
    for(int i=0;i<=slices;i++) {
        points[j][i].x = j;
        points[j][i].y=radius*cos(((double)i/(double)slices)*2*pi);
        points[j][i].z=radius*sin(((double)i/(double)slices)*2*pi);
    }
  }

  for(int i=0;i<stacks;i++)
	{
    // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
    if(i%2) {
      glColor3f(0, 0, 0);
    } else {
      glColor3f(1, 1, 1);
    }

		for(int j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
        //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
        //lower hemisphere
        // glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				// glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				// glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				// glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawHemiSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
    // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
    if(i%2) {
      glColor3f(1, 1, 1);
    } else {
      glColor3f(0, 0, 0);
    }

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
        //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
        //lower hemisphere
        // glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				// glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				// glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				// glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

point crossProduct(point a, point b) {
  point resultantPoint;
  resultantPoint.x = a.y * b.z - a.z * b.y;
  resultantPoint.y = a.z * b.x - a.x * b.z;
  resultantPoint.z = a.x * b.y - a.y * b.x;
  return resultantPoint;
}

void rotateVector(point *vectorToBeRotated, point referenceVector, double angleInDegree) {
  double angleInRadian = angleInDegree * pi / 180.0;
  point perpendicularVector = crossProduct(*vectorToBeRotated, referenceVector);
  point oldVectorToBeRotated;
  oldVectorToBeRotated.x = vectorToBeRotated->x * cos(angleInRadian);
  oldVectorToBeRotated.y = vectorToBeRotated->y * cos(angleInRadian);
  oldVectorToBeRotated.z = vectorToBeRotated->z * cos(angleInRadian);

  perpendicularVector.x = perpendicularVector.x * sin(angleInRadian);
  perpendicularVector.y = perpendicularVector.y * sin(angleInRadian);
  perpendicularVector.z = perpendicularVector.z * sin(angleInRadian);

  vectorToBeRotated->x = oldVectorToBeRotated.x + perpendicularVector.x;
  vectorToBeRotated->y = oldVectorToBeRotated.y + perpendicularVector.y;
  vectorToBeRotated->z = oldVectorToBeRotated.z + perpendicularVector.z;
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			// rotate lookVector and rightVector by 5 deg counter clockwise with
      // respect to upVector
      rotateVector(&lookVector, upVector, -5);
      rotateVector(&rightVector, upVector, -5);
			break;

    case '2':
      rotateVector(&lookVector, upVector, 5);
      rotateVector(&rightVector, upVector, 5);
			break;

    case '3':
      rotateVector(&lookVector, rightVector, -5);
      rotateVector(&upVector, rightVector, -5);
      break;

    case '4':
      rotateVector(&lookVector, rightVector, 5);
      rotateVector(&upVector, rightVector, 5);
      break;
    case '5':
      rotateVector(&upVector, lookVector, -5);
      rotateVector(&rightVector, lookVector, -5);
      break;
    case '6':
      rotateVector(&upVector, lookVector, 5);
      rotateVector(&rightVector, lookVector, 5);
      break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			// cameraHeight -= 3.0;
      pos.x -= lookVector.x;
      pos.y -= lookVector.y;
      pos.z -= lookVector.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
			// cameraHeight += 3.0;
      pos.x += lookVector.x;
      pos.y += lookVector.y;
      pos.z += lookVector.z;

			break;

		case GLUT_KEY_RIGHT:
			// cameraAngle += 0.03;
      pos.x += rightVector.x;
      pos.y += rightVector.y;
      pos.z += rightVector.z;
			break;
		case GLUT_KEY_LEFT:
			// cameraAngle -= 0.03;
      pos.x -= rightVector.x;
      pos.y -= rightVector.y;
      pos.z -= rightVector.z;
			break;

		case GLUT_KEY_PAGE_UP:
      pos.x += upVector.x;
      pos.y += upVector.y;
      pos.z += upVector.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
      pos.x -= upVector.x;
      pos.y -= upVector.y;
      pos.z -= upVector.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void loadData() {
	freopen("scene.txt", "r", stdin);

	cin >> levelOfRecursion;
	cin >> pixelCount;

	cin >> numberOfObjects;

	int objectIndex = 0;

	string objectType;
	double referencePoint[3];
	double height, width, length;
	double color[3];
	double coefficients[4];
	int shininess;

	Object* t;
	t = new Floor(1000, 20);
	allObjects.push_back(t);

	for(int i = 1; i <= numberOfObjects; i++) {
		cin >> objectType;
		if(objectType == "sphere") {
			cin >> referencePoint[0] >> referencePoint[1] >> referencePoint[2];
			cin >> length;
			cin >> color[0] >> color[1] >> color[2];
			cin >> coefficients[0] >> coefficients[1] >> coefficients[2] >> coefficients[3];
			cin >> shininess;

			Object* temp;
			temp = new Sphere(referencePoint, length, color, coefficients, shininess);
			allObjects.push_back(temp);
			// cout << objectIndex << " " << allObjects[objectIndex - 1]->length << endl;
		}
		else if(objectType == "triangle") {
			point point1;
			point point2;
			point point3;

			cin >> point1.x >> point1.y >> point1.z;
			cin >> point2.x >> point2.y >> point2.z;
			cin >> point3.x >> point3.y >> point3.z;
			
			cin >> color[0] >> color[1] >> color[2];
			cin >> coefficients[0] >> coefficients[1] >> coefficients[2] >> coefficients[3];
			cin >> shininess;

			Object* temp;
			temp = new Triangle(point1, point2, point3, color, coefficients, shininess);
			allObjects.push_back(temp);
		}
	}
}

void drawObjects() {
	for(int i = 0; i < numberOfObjects; i++) {
		allObjects[i]->draw();
	}
}

void display(){
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	gluLookAt(pos.x,pos.y,pos.z,	lookVector.x,lookVector.y,lookVector.z,	upVector.x,upVector.y,upVector.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	// drawGrid();

	drawObjects();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=2.0;
	cameraAngle=1.0;
	angle=0;

  pos.x = 100;
  pos.y = 100;
  pos.z = 100;

  lookVector.x = -2*cos(pi / 4);
  lookVector.y = -2*cos(pi / 4);
  lookVector.z = 0;

  rightVector.x = -2*cos(pi / 4);
  rightVector.y = 2*cos(pi / 4);
  rightVector.z = 0;

  upVector.x = 0;
  upVector.y = 0;
  upVector.z = 1;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	// give PERSPECTIVE parameters
  // Parameters:
	// field of view along the Y axis (vertically) in degree
	// aspect ratio that determines the field of view along the X axis (horizontally)
	// near distance
	// far distance
	gluPerspective(80,	1,	1,	1000.0);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Offline 3");

	init();
	loadData();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

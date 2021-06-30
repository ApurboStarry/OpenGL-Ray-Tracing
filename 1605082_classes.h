#include <bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))

struct point
{
	double x,y,z;
};

class Object {
public:
  double referencePoint[3];
  double height, width, length;
  double color[3];
  double coefficients[4];
  int shininess;

  Object() {}
  
  virtual void draw() {
    cout << "draw of object" << endl;
  }

  void setReferencePoint(double referencePoint[]) {
    this->referencePoint[0] = referencePoint[0];
    this->referencePoint[1] = referencePoint[1];
    this->referencePoint[2] = referencePoint[2];
  }

  void setColor(double color[]) {
    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
  }

  void setShininess(int shininess) {
    this->shininess = shininess;
  }

  void setCoefficients(double coefficients[]) {
    this->coefficients[0] = coefficients[0];
    this->coefficients[1] = coefficients[1];
    this->coefficients[2] = coefficients[2];
    this->coefficients[3] = coefficients[3];
  }
};

class Floor : public Object {
public:
  int floorWidth;
  int tileWidth;

  Floor(int floorWidth, int tileWidth) {
    this->referencePoint[0] = - floorWidth / 2;
    this->referencePoint[1] = - floorWidth / 2;
    this->referencePoint[2] = 0;

    this->length = tileWidth;
    this->floorWidth = floorWidth;
    this->tileWidth = tileWidth;
  }

  void draw() {
    int limit = floorWidth % tileWidth == 0 ? floorWidth / tileWidth : floorWidth / tileWidth + 1;

    for(int i = referencePoint[0], colorX = 0; i < floorWidth / 2; i += tileWidth, colorX++) {
      for(int j = referencePoint[1], colorY = 0; j < floorWidth / 2; j += tileWidth, colorY++) {
        if((colorX + colorY) % 2 == 0) {
          glColor3f(1.0, 1.0, 1.0);
        } else {
          glColor3f(0.0, 0.0, 0.0);
        }

        glBegin(GL_QUADS);
        {
          glVertex3f(1.0 * i, 1.0 * j, 0.0);
          glVertex3f(1.0 * i + tileWidth, 1.0 * j, 0.0);
          glVertex3f(1.0 * i + tileWidth, 1.0 * j + tileWidth, 0.0);
          glVertex3f(1.0 * i, 1.0 * j + tileWidth, 0.0);
        }
        glEnd();
      }
    }
  }
};

class Sphere : public Object {
public:
  Sphere(double center[], double radius, double color[], double coefficients[], int shininess) {
    this->setReferencePoint(center);
    this->length = radius;
    this->setColor(color);
    this->setCoefficients(coefficients);
    this->setShininess(shininess);
  }

  void draw() {
    int slices = 24, stacks = 20;
    int radius = this->length;

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
        points[i][j].x = this->referencePoint[0] + r*cos(((double)j/(double)slices)*2*pi);
        points[i][j].y = this->referencePoint[1] + r*sin(((double)j/(double)slices)*2*pi);
        points[i][j].z = h;
      }
    }
    
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
      // set the color of the sphere
      glColor3f(this->color[0], this->color[1], this->color[2]);

      for(j=0;j<slices;j++) {
        glBegin(GL_QUADS);
        {
          //upper hemisphere
          glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z + this->referencePoint[2]);
          glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z + this->referencePoint[2]);
          glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z + this->referencePoint[2]);
          glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z + this->referencePoint[2]);
          
          //lower hemisphere
          glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z + this->referencePoint[2]);
          glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z + this->referencePoint[2]);
          glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z + this->referencePoint[2]);
          glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z + this->referencePoint[2]);
        }
        glEnd();
      }
    }
  }
};

class Triangle : public Object {
public: 
  point point1;
  point point2;
  point point3;

  Triangle(point point1, point point2, point point3, double color[], double coefficients[], int shininess) {
    this->point1 = point1;
    this->point2 = point2;
    this->point3 = point3;

    this->setColor(color);
    this->setCoefficients(coefficients);
    this->setShininess(shininess);
  }

  void draw() {
    glColor3f(this->color[0],this->color[1],this->color[2]);

    glBegin(GL_TRIANGLES);
    {
      glVertex3f(point1.x, point1.y, point1.z);
      glVertex3f(point2.x, point2.y, point2.z);
      glVertex3f(point3.x, point3.y, point3.z);
    }
    glEnd();
  }
};
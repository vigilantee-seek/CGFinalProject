#include <iostream>
#include <algorithm>
#include "GL\glut.h"
#include <vector>

#define ITER 1

const GLint windowLeft = 0, windowTop = 0;
const GLint winWidth = 800, winHeight = 600;
const GLint wid = winWidth / (2 * ITER);
const GLint wih = winHeight / (2 * ITER);

class Point {
protected:
	GLint fx;
	GLint fy;
private:
	// Forbid copy constructor
	Point(const Point&);
public:

	Point(GLint x = 0, GLint y = 0) {
		fx = x; fy = y;
	}

	Point(const std::vector<GLint>& pt) {
		fx = pt[0]; fy = pt[1];
	}

	// The pts contains two elements: fx, fy
	void setPoint(GLint x = 0, GLint y = 0) {
		fx = x;	fy = y;
	}

	void setPoint(const std::vector<GLint>& pt) {
		fx = pt[0]; fy = pt[1];
	}

	void copyPoint(Point* dest) const {
		dest->setPoint(fx, fy);
	}

	// Return the co-ordinates of "this" point
	bool getPoint(std::vector<GLint>* pt) const {
		int vec_size = pt->size();
		if (!vec_size) {
			pt->push_back(fx); pt->push_back(fy);
			return true;
		}
		else if (vec_size >= 2) {
			(*pt)[0] = fx; (*pt)[1] = fy;
			return true;
		}
		return false;
	}

	const std::vector<GLint> getPoint() const{
		std::vector<GLint> pt;
		pt.push_back(fx); pt.push_back(fy);
		return pt;
	}
} cp, old_mp, mp, fp;
// Global variables for the "fixed" point, center point and motion point

class Point3d :public Point {
private:
	GLint fz;
	Point3d(const Point3d&);
public:
	Point3d(GLint x = 0, GLint y = 0, GLint z = 0) : Point(x, y), fz(z) {}
	Point3d(const std::vector<GLint>& pt3d) : Point(pt3d), fz(pt3d[2]) {}

	// The pts contains three elements: fx, fy and fz
	void setPoint(GLint x = 0, GLint y = 0, GLint z = 0) {
		setPoint(x, y);
		fz = z;
	}

	void copyPoint(Point3d* dest) const {
		dest->setPoint(fx, fy, fz);
	}

	// Return the co-ordinates of "this" point
	bool getPoint(std::vector<GLint>* pt3d) const {
		int vec_size = pt3d->size();
		if (!vec_size) {
			pt3d->push_back(fx); pt3d->push_back(fy); pt3d->push_back(fz);
			return true;
		}
		else if (vec_size == 3) {
			(*pt3d)[0] = fx; (*pt3d)[1] = fy; (*pt3d)[2] = fz;
			return true;
		}
		return false;
	}

	const std::vector<GLint> getPoint() const {
		std::vector<GLint> pt3d;
		pt3d.push_back(fx); pt3d.push_back(fy); pt3d.push_back(fz);
		return pt3d;
	}
} lp;

void init() {

	// Initialize the background color of the main window
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glOrtho(-5, 5, -5, 5, 5, 10);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 0.5, 0);

	// Initialize teapot material property
	GLfloat reflect_specular[] = { 1.0f, 0.6f, 0.3f, 1.0f };
	GLfloat reflect_diffuse[] = { 1.0f, 0.6f, 0.3f, 1.0f };
	//GLfloat emission[] = { 0.3f, 0.2f, 0.5f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, reflect_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, reflect_diffuse);
	//glMaterialfv(GL_FRONT, GL_EMISSION, emission);

	// Initialize light
	GLfloat point_light_pos[] = { 5.0f, 4.0f, 2.0f, 1.0f };
	GLfloat parallel_light_pos[] = { -1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat no_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat white_light[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat orange_light[] = { 0.5f, 1.0f, 0.3f, 1.0f };
	GLfloat blue_light[] = { 0.1f, 0.3f, 0.8f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, point_light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, no_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, orange_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);

	glLightfv(GL_LIGHT1, GL_POSITION, parallel_light_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, orange_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, no_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);


	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
}

// 3-dimention rotation
void rotate3d(
	const Point& fixedp, 
	const Point& oldmp,
	const Point& motionp,
	const Point& centerp
) {
	std::vector<GLint> fpp, mpp, cpp, opp;
	fixedp.getPoint(&fpp);
	motionp.getPoint(&mpp);
	centerp.getPoint(&cpp);
	oldmp.getPoint(&opp);

	GLint x = mpp[0] - opp[0];	GLint y = mpp[1] - opp[1];

	// The object should rotate angleY around Y axis and angleX around X axis
	GLdouble angleY = (x % wid) / GLdouble(wid) * 360;
	GLdouble angleX = -(y % wih) / GLdouble(wih) * 360;


	glRotated(angleX, 1.0, 0, 0);
	glRotated(angleY, 0, 1.0, 0);
}

void display() {

	// Set color of the teapot
	//glColor3f(1.0, 0, 0);
	glutSolidTeapot(2);

	rotate3d(fp, old_mp, mp, cp);

	glFlush();
}

void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		fp.setPoint(x, y);
		old_mp.setPoint(x, y);
		mp.setPoint(x, y);
	}
}

void onMotion(int x, int y) {
	mp.copyPoint(&old_mp);
	mp.setPoint(x, y);

	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

// reflect light and we assume the z-coordinate of the center point is 0
void reflect(Point3d lightp, Point centerp) {

}

int main(int argc, char* argv[]) {
	const char* winName = "Rotating Box";
	cp.setPoint(winWidth / 2, winHeight / 2);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(windowLeft, windowTop);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow(winName);

	init();
	glutDisplayFunc(display);
	glutMouseFunc(&onMouse);
	glutMotionFunc(&onMotion);
	glutMainLoop();

	return 0;
}
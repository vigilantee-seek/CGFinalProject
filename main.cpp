#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <math.h>
#define ITER 1

const GLint windowLeft = 0, windowTop = 0;
const GLint winWidth = 800, winHeight = 600;
const GLint wid = winWidth / (2 * ITER);
const GLint wih = winHeight / (2 * ITER);
GLdouble Mt[4][4]{ 0 };

class Point {
private:
	GLint fx;
	GLint fy;
public:

	Point() {
		fx = 0; fy = 0;
	}

	// The pts contains two elements: fx, fy
	void setPoint(GLint x = 0, GLint y = 0) {
		fx = x;	fy = y;
	}

	// Return the co-ordinates of "this" point
	bool getPoint(std::vector<GLint>& pts) {
		int vec_size = pts.size();
		if (!vec_size) {
			pts.push_back(fx); pts.push_back(fy);
			return true;
		}
		else if (vec_size == 2) {
			pts[0] = fx; pts[1] = fy;
			return true;
		} 
		return false;
	}
}cp, old_mp, mp, fp;
// Global variable for the "fixed" point, center point and motion point

void init() {

	// Initialize the background color of the main window
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glOrtho(-5, 5, -5, 5, 5, 10);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 0.5, 0);
}

// 3-dimention rotation
void rotate3d(Point fixedp, Point oldmp,Point motionp, Point centerp) {
	std::vector<GLint> fpp, mpp, cpp, opp;
	fixedp.getPoint(fpp);
	motionp.getPoint(mpp);
	centerp.getPoint(cpp);
	oldmp.getPoint(opp);

	GLint ax = fpp[0] - cpp[0];
	GLint ay = fpp[1] - cpp[1];

	GLdouble angle;

	GLint x = mpp[0] - opp[0];	GLint y = mpp[1] - opp[1];

	// The object should rotate angleY around Y axis and angleX around X axis
	GLdouble angleY = (x % wid) / GLdouble(wid) * 360;
	GLdouble angleX = -(y % wih) / GLdouble(wih) * 360;

	std::cout << angleY << ' ' << angleX << std::endl;

	glRotated(angleX, 1.0, 0, 0);
	glRotated(angleY, 0, 1.0, 0);
}

void display() {

	// Set color of the teapot
	glColor3f(1.0, 0, 0);
	glutWireTeapot(2);

	rotate3d(fp, old_mp, mp, cp);

	glFlush();
}

void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			fp.setPoint(x, y);
			old_mp.setPoint(x, y);
	}
}

void onMotion(int x, int y) {
	std::vector<GLint> temp;
	mp.getPoint(temp);
	old_mp.setPoint(temp[0], temp[1]);
	mp.setPoint(x, y);

	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	const char* winName = "Rotating Box";
	// Initialize Mt
	Mt[3][3] = 1;
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
#include<iostream>
#include<GL/glut.h>
#include<vector>
#include<bitset>

void render_scene();

/*
class color
{

public:
float r, g, b;
color(float r, float g, float b) :
r(r),
g(g),
b(b)
{
}
};
static std::vector<color> COLORS{ { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 0.0, 1.0, 1.0 },{ 1.0, 1.0, 0.0 },{ 1.0, 0.0, 1.0 } };
*/
void bresenhamDraw0_90(int x1, int y1, int x2, int y2);
void bresenhamDraw0_neg90(int x1, int y1, int x2, int y2);

class Line
{
public:
	int v1x, v1y, v2x, v2y;

	Line(int v1x, int v1y, int v2x, int v2y) :
		v1x(v1x),
		v1y(v1y),
		v2x(v2x),
		v2y(v2y)
	{

	}
};

std::bitset<4> getBitsetForPoint(int x, int y, int xmin, int xmax, int ymin, int ymax)
{
	std::bitset<4> code{ false };

	if (y > ymax)
		code[3] = true;

	if (y < ymin)
		code[2] = true;

	if (x > xmax)
		code[1] = true;

	if (x < xmin)
		code[0] = true;

	return code;
}

class program_state
{
	std::vector<Line> lines;

public:
	int window_width;
	int window_height;
	int mousex, mousey;
	bool control;
	bool clip;
	bool isDrawing;
	int drawingx, drawingy;
	program_state()
	{
		program_state(0, 0);
	}

	program_state(int w, int h) :
		lines(),
		window_width(w),
		window_height(h),
		control(false),
		clip(false),
		isDrawing(false),
		drawingx(0),
		drawingy(0)
	{

	}

	void changed_window_dimensions(int w, int h)
	{
		window_width = w;
		window_height = h;
		glutPostRedisplay();
	}


	void changedMouse(int x, int y)
	{
		mousex = x;
		mousey = y;
		glutPostRedisplay();
	}

	std::vector<Line> getLines()
	{
		return lines;
	}

	void addLine(Line l)
	{
		lines.push_back(l);
		glutPostRedisplay();
	}

	void clicked()
	{
		if (isDrawing)
		{
			isDrawing = false;
			addLine(Line(drawingx, drawingy, mousex, mousey));
			drawingx = 0;
			drawingy = 0;
		}
		else {
			isDrawing = true;
			drawingx = mousex;
			drawingy = mousey;
		}

		glutPostRedisplay();
	}

	void changeControl()
	{
		control = !control;
		glutPostRedisplay();
	}

	void changeClipping()
	{
		clip = !clip;
		glutPostRedisplay();

	}
};

program_state GLOBAL_STATE;

void bresenhamDraw(int x1, int y1, int x2, int y2)
{
	if (x1 <= x2)
	{
		if (y1 <= y2)
		{
			bresenhamDraw0_90(x1, y1, x2, y2);
		}
		else {
			bresenhamDraw0_neg90(x1, y1, x2, y2);
		}
	}
	else {
		if (y1 >= y2)
		{
			bresenhamDraw0_90(x2, y2, x1, y1);
		}
		else {
			bresenhamDraw0_90(x2, y2, x1, y1);
		}
	}
}

void bresenhamDraw0_90(int x1, int y1, int x2, int y2)
{
	int x, yc, korekcija;
	int a, yf;
	glBegin(GL_POINTS);
	if (y2 - y1 <= x2 - x1)
	{
		a = 2 * (y2 - y1);
		yc = y1;
		yf = -(x2 - x1);
		korekcija = -2 * (x2 - x1);

		for (x = x1; x <= x2; ++x)
		{
			glVertex2i(x, yc);
			yf += a;

			if (yf >= 0)
			{
				yf += korekcija;
				yc++;
			}
		}
	}
	else {
		x = x2;
		x2 = y2;
		y2 = x;

		x = x1;
		x1 = y1;
		y1 = x;

		a = 2 * (y2 - y1);
		yc = y1;
		yf = -(x2 - x1);
		korekcija = -2 * (x2 - x1);
		for (x = x1; x <= x2; x++)
		{
			glVertex2i(yc, x);
			yf += a;
			if (yf >= 0)
			{
				yf += korekcija;
				yc++;
			}
		}
	}
	glEnd();
}


void bresenhamDraw0_neg90(int x1, int y1, int x2, int y2)
{
	int x, yc, korekcija;
	int a, yf;
	glBegin(GL_POINTS);
	if (-(y2 - y1) <= x2 - x1)
	{
		a = 2 * (y2 - y1);
		yc = y1;
		yf = (x2 - x1);
		korekcija = 2 * (x2 - x1);

		for (x = x1; x <= x2; ++x)
		{
			glVertex2i(x, yc);
			yf += a;

			if (yf <= 0)
			{
				yf += korekcija;
				yc--;
			}
		}
	}
	else {
		x = x2;
		x2 = y1;
		y1 = x;

		x = x1;
		x1 = y2;
		y2 = x;

		a = 2 * (y2 - y1);
		yc = y1;
		yf = (x2 - x1);
		korekcija = 2 * (x2 - x1);
		for (x = x1; x <= x2; x++)
		{
			glVertex2i(yc, x);
			yf += a;
			if (yf <= 0)
			{
				yf += korekcija;
				yc--;
			}
		}
	}
	glEnd();
}


int getControlLinePointForX(int x, int x1, int y1, int x2, int y2)
{
	double k = (y2 - y1) / ((double)x2 - x1);
	double l = -k * x1 + y1;
	return (int)(k * x + l + 4 * sqrt(1 - k * k));
}

void drawControlLine(int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(x1, getControlLinePointForX(x1, x1, y1, x2, y2));
	glVertex2i(x2, getControlLinePointForX(x2, x1, y1, x2, y2));
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
}

int getYforX(int x, Line l)
{
	double k = (l.v2y - l.v1y) / ((double)l.v2x - l.v1x);
	double l_ = -k * l.v1x + l.v1y;

	return (int)(k * x + l_);
}

int getXforY(int y, Line l)
{
	double k = (l.v2y - l.v1y) / ((double)l.v2x - l.v1x);
	double l_ = -k * l.v1x + l.v1y;

	return (int)((y - l_) / k);
}


Line getClippedLine(Line original, int xmin, int xmax, int ymin, int ymax, bool& line_none)
{
	std::bitset<4> point1bs = getBitsetForPoint(original.v1x, original.v1y, xmin, xmax, ymin, ymax);
	std::bitset<4> point2bs = getBitsetForPoint(original.v2x, original.v2y, xmin, xmax, ymin, ymax);

	if (point1bs.none() && point2bs.none())
	{
		return original;
	}
	else if ((point1bs & point2bs).any())
	{
		line_none = true;
		return Line(0, 0, 0, 0);
	}
	else
	{
		//We need to clip
		Line clipped = original;

		if (point1bs[3]) {
			int xForYmax = getXforY(ymax, original);
			clipped.v1x = xForYmax;
			clipped.v1y = ymax;
		}

		if (point1bs[2]) {
			int xForYmin = getXforY(ymin, original);
			clipped.v1x = xForYmin;
			clipped.v1y = ymin;
		}

		if (point1bs[1]) {
			int yForXmax = getYforX(xmax, original);
			clipped.v1x = xmax;
			clipped.v1y = yForXmax;
		}

		if (point1bs[0]) {
			int yForXmin = getYforX(xmin, original);
			clipped.v1x = xmin;
			clipped.v1y = yForXmin;
		}

		if (point2bs[3]) {
			int xForYmax = getXforY(ymax, original);
			clipped.v2x = xForYmax;
			clipped.v2y = ymax;
		}

		if (point2bs[2]) {
			int xForYmin = getXforY(ymin, original);
			clipped.v2x = xForYmin;
			clipped.v2y = ymin;
		}

		if (point2bs[1]) {
			int yForXmax = getYforX(xmax, original);
			clipped.v2x = xmax;
			clipped.v2y = yForXmax;
		}

		if (point2bs[0]) {
			int yForXmin = getYforX(xmin, original);
			clipped.v2x = xmin;
			clipped.v2y = yForXmin;
		}

		return clipped;
	}
}

void render_scene()
{

	//Render all current lines (no control, no clipping)
	glPointSize(1.0f);

	std::vector<Line> lines = GLOBAL_STATE.getLines();
	glColor3f(0.0, 0.0, 0.0);

	double q_ww = GLOBAL_STATE.window_width / 4.0;
	double q_wh = GLOBAL_STATE.window_height / 4.0;

	for (auto line : lines)
	{
		//Clip this part

		bool doNotDraw = false;
		Line clipped = line;

		if (GLOBAL_STATE.clip) {

			clipped = getClippedLine(line, (int)q_ww, (int)(3 * q_ww), (int)q_wh, (int)(3 * q_wh), doNotDraw);
		}

		if (!doNotDraw) {
			bresenhamDraw(clipped.v1x, clipped.v1y, clipped.v2x, clipped.v2y);
			//glVertex2i(clipped.v1_x, clipped.v1y);
			//glVertex2i(clipped.v2x, clipped.v2y);
		}

		if (GLOBAL_STATE.control)
		{
			drawControlLine(line.v1x, line.v1y, line.v2x, line.v2y);
		}

	}

	//Render line (if we have one point selected)
	glColor3f(0.0, 0.0, 0.0);

	if (GLOBAL_STATE.isDrawing)
	{
		glBegin(GL_LINES);
		glVertex2i(GLOBAL_STATE.drawingx, GLOBAL_STATE.drawingy);
		glVertex2i(GLOBAL_STATE.mousex, GLOBAL_STATE.mousey);
		glEnd();
	}

	if (GLOBAL_STATE.clip)
	{
		glColor3f(0.0, 1.0, 0.0);
		//Draw clipping box
		glBegin(GL_LINE_LOOP);


		glVertex2i((int)q_ww, (int)q_wh);
		glVertex2i((int)(3 * q_ww), (int)q_wh);
		glVertex2i((int)(3 * q_ww), (int)(3 * q_wh));
		glVertex2i((int)q_ww, (int)(3 * q_wh));

		glEnd();
		glColor3f(0.0, 0.0, 0.0);
	}

}





void display(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.9f, -0.9f, -0.9f);
	glVertex3f(0.9f, -0.9f, -0.9f);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.9f, -0.7f, -0.9f);
	glVertex3f(0.9f, -0.7f, 3.1f);
	glEnd();

	//render_scene();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glViewport(0, 0, width, height);

	GLOBAL_STATE.changed_window_dimensions(width, height);
}

void keyPressed(unsigned char key, int x, int y)
{

}

void keyReleased(unsigned char key, int x, int y)
{
	if (key == 'k')
	{
		GLOBAL_STATE.changeControl();
	}

	if (key == 'o')
	{
		GLOBAL_STATE.changeClipping();
	}
}

void keySpecialPressed(int key, int x, int y)
{

}

void keySpecialReleased(int key, int x, int y)
{

}

void mousePressedOrRelased(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		GLOBAL_STATE.clicked();
	}
}

void mouseMoved(int x, int y)
{
	GLOBAL_STATE.changedMouse(x, y);
}

void mouseDragged(int x, int y)
{

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE);  // double buffers
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Bresenham");
	GLOBAL_STATE.changed_window_dimensions(100, 100);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutSpecialFunc(keySpecialPressed);
	glutSpecialUpFunc(keySpecialReleased);
	glutMouseFunc(mousePressedOrRelased);
	glutMotionFunc(mouseDragged);
	glutPassiveMotionFunc(mouseMoved);
	glutMainLoop();

	return 0;
}


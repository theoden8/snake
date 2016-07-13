#pragma once

#include "glut_config.h"
#include "opengl_config.h"

#include "State.hpp"
#include "Snake.hpp"
#include "Wall.hpp"
#include "Fruit.hpp"

class Graphics {
public:
	static State *state;
private:
	static Snake *sn;
	static Wall *wls;
	static Fruit *frs;
public:
	static void SetOpenGLContext(State *st, Snake *sn, Wall *w, Fruit *f, int &argc, char **argv);
	static void SetOpenGLFunctions();
	static void StartGraphics();

	static void DrawText(float x, float y, char *s);
	static void DrawObject(int x, int y, GLuint id, double degree);

private:
	static void Display();
	static void Timer(int);
	static void Keyboard(unsigned char key, int x, int y);
	static void Arrow_Keys(int key, int x, int y);
	static void Mouse(int button, int state, int x, int y);
	static void Reshape(int new_width, int new_height);
};

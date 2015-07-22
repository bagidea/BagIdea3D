#ifndef BI3D_INCLUDE_H
#define BI3D_INCLUDE_H

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include "BI3D_KeyCode.h"
#include "BI3D_Scene.h"

#define BI3D_EVENT_START 0
#define BI3D_EVENT_UPDATE 1
#define BI3D_EVENT_INPUT 2

#define Event SDL_Event
#define KEYDOWN SDL_KEYDOWN
#define KEYUP SDL_KEYUP

using namespace std;

struct Mouse
{
	int x, y;
};

struct Color
{
	GLfloat r, g, b, a;
	Color();
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
};

class BI3D
{
public:
	BI3D(string title = "BagIdea3D", int width = 800, int height = 600);
	~BI3D();

	bool Start();

	void SetEvent(void* event = NULL, int type = BI3D_EVENT_START);
	void AddScene(Scene* scene);
	void SetMouseInWindow(int x, int y);

	Mouse GetMousePosition();
private:
	string title;
	int width, height;

	bool quit;
	SDL_Event e;

	Color bgColor;

	SDL_Surface* iconImg;
	SDL_Window* window;
	SDL_GLContext gl;

	void (*StartEvent)();
	void (*UpdateEvent)();
	void (*InputEvent)(Event e);

	vector<Scene*> sceneList;
};

#endif
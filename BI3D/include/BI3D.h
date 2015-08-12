#ifndef BI3D_INCLUDE_H
#define BI3D_INCLUDE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

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
#define KEYUP SDL_KEYUP

#define MOUSEDOWN SDL_MOUSEBUTTONDOWN
#define MOUSEUP SDL_MOUSEBUTTONUP
#define MOUSELEFT SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)
#define MOUSERIGHT SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)
#define MOUSEMIDDLE SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)

using namespace std;

struct Mouse
{
	int x, y;
};

class BI3D
{
public:
	BI3D(string title = "BagIdea3D", int width = 800, int height = 600);
	~BI3D();

	bool Start();

	void SetEvent(void* event = NULL, int type = BI3D_EVENT_START);
	void AddScene(Scene* scene);
	void DeleteScene(Scene* scene);
	void SetMouseInWindow(int x, int y);

	Mouse GetMousePosition();
	void ShowMouseCursor(bool value);

	GLfloat Randomf(GLfloat min, GLfloat max);

	GLfloat GetScreenWidth();
	GLfloat GetScreenHeight();

	void Close();
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
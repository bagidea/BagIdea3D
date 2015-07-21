#include "BI3D.h"

Color::Color()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

BI3D::BI3D(string title, int width, int height)
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if(!SDL_Init(SDL_INIT_VIDEO) < 0)
		cerr << "Initialized SDL2 fail. : Error - " << SDL_GetError() << endl;

	if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
	{
		cerr << "Initialized SDL2 Image fail. : Error - " << IMG_GetError() << endl;
	}else{
		iconImg = IMG_Load("source/icon.png");
		if(iconImg == NULL)
			cerr << "Load Icon Fail : Error - " << IMG_GetError() << endl;
	}

	this->title = title;
	this->width = width;
	this->height = height;

	quit = false;

	window = NULL;

	StartEvent = NULL;
	UpdateEvent = NULL;
}

BI3D::~BI3D()
{
	for(GLint i = 0; i < sceneList.size(); i++)
	{
		delete sceneList[i];
		sceneList[i] = NULL;
	}

	sceneList.clear();

	if(iconImg != NULL)
	{
		SDL_FreeSurface(iconImg);
		iconImg = NULL;
	}

	if(window != NULL)
	{
		SDL_DestroyWindow(window);
		SDL_GL_DeleteContext(gl);
		window = NULL;
	}

	SDL_Quit();
}

bool BI3D::Start()
{
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if(window == NULL)
	{
		cerr << "Create Window Fail : Error - " << SDL_GetError() << endl;
		return false;
	}else{
		gl = SDL_GL_CreateContext(window);

			if(glewInit() != GLEW_OK)
				cerr << "Glew Error." << endl;

		if(iconImg != NULL)
			SDL_SetWindowIcon(window, iconImg);
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	if(StartEvent != NULL)
		StartEvent();

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			switch(e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if(e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
					break;
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(UpdateEvent != NULL)
			UpdateEvent();

		for(GLint i = 0; i < sceneList.size(); i++)
			sceneList[i]->Update();

		SDL_GL_SwapWindow(window);
	}

	return true;
}

void BI3D::SetEvent(void* event, int type)
{
	if(event != NULL)
	{
		if(type == BI3D_EVENT_START)
			StartEvent = (void(*)(void))event;
		else if(type == BI3D_EVENT_UPDATE)
			UpdateEvent = (void(*)(void))event;
	}
}

void BI3D::AddScene(Scene* scene)
{
	sceneList.push_back(scene);
}
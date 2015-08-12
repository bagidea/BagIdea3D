#include "BI3D.h"

BI3D::BI3D(string title, int width, int height)
{
	srand(time(NULL));

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); 

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
	InputEvent = NULL;
}

BI3D::~BI3D()
{
	for(GLint i = 0; i < sceneList.size(); i++)
	{
		if(sceneList[i] != NULL)
		{
			delete sceneList[i];
			sceneList[i] = NULL;
		}
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

		SDL_GL_SetSwapInterval(1);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW); 
	//glEnable(GL_FRAMEBUFFER_SRGB);

	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	if(StartEvent != NULL)
		StartEvent();

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(InputEvent != NULL)
				InputEvent(e);

			switch(e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
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
		else if(type == BI3D_EVENT_INPUT)
			InputEvent = (void(*)(Event))event;
	}
}

void BI3D::AddScene(Scene* scene)
{
	scene->SetScreen(width, height);
	sceneList.push_back(scene);
}

void BI3D::DeleteScene(Scene* scene)
{
	for(GLint i = 0; i < sceneList.size(); i++)
	{
		if(sceneList[i] == scene)
		{
			if(sceneList[i] != NULL)
			{
				delete sceneList[i];
				sceneList[i] = NULL;
			}

			sceneList.erase(sceneList.begin()+i);
		}
	}
}

void BI3D::SetMouseInWindow(int x, int y)
{
	SDL_WarpMouseInWindow(window, x, y);
}

Mouse BI3D::GetMousePosition()
{
	Mouse mouseObject;
	SDL_GetMouseState(&mouseObject.x, &mouseObject.y);

	return mouseObject;
}

void BI3D::ShowMouseCursor(bool value)
{
	if(value)
		SDL_ShowCursor(SDL_ENABLE);
	else
		SDL_ShowCursor(SDL_DISABLE);

}

GLfloat BI3D::Randomf(GLfloat min, GLfloat max)
{
	if(min < 0.0f | max < 0.0f)
		return 0.0f;

	GLfloat ran = (((GLfloat)rand()/(GLfloat)RAND_MAX)*(min+max))-min;

	return ran;
}

GLfloat BI3D::GetScreenWidth(){return width;}
GLfloat BI3D::GetScreenHeight(){return height;}

void BI3D::Close()
{
	delete this;
}
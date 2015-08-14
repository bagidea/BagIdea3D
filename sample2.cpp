#include "BI3D.h"

#include <cmath>

BI3D* bis;

Scene* scene;
Camera* camera;
Object* plane;
Object* model;

PointLight* pl1;
PointLight* pl2;
PointLight* pl3;
PointLight* pl4;

SpotLight* sl;

Sprite2D* howTo;
Sprite2D* logo;

bool upK, downK, leftK, rightK;

bool mouseLock;

void Start()
{
	mouseLock = true;

	upK = false;
	downK = false;
	leftK = false;
	rightK = false;

	scene = new Scene();

	//SupportShadowMap
	//scene->supportShadowMap = true;

	camera = new Camera(45.0f, 0.1f, 500.0f);

	camera->y = 5.0f;
	camera->z = -5.0f;

	scene->SetCamera(camera);
	//scene->ClearCamera();

	plane = new Object();
	plane->LoadTexture("source/model/plane.jpg", BI3D_TEXTURE_DIFFUSE);
	plane->LoadTexture("source/model/planeSpec.jpg", BI3D_TEXTURE_SPECULAR);
	plane->Load("source/model/plane.obj");
	plane->SetMaterialMode(BI3D_SUPPORT_LIGHT);
	plane->SetShininess(128.0f);

	plane->scaleX = 10.0f;
	plane->scaleZ = 10.0f;

	scene->AddChild(plane);

	model = new Object();
	model->Load("source/model/nanosuit/nanosuit.obj");
	model->SetMaterialMode(BI3D_SUPPORT_LIGHT);

	model->rotationY = 180.0f;
	model->scaleX = 0.2f;
	model->scaleY = 0.2f;
	model->scaleZ = 0.2f;

	scene->AddChild(model);
	//scene->DeleteChild(model);

	//DirectionalLight
	//scene->SetDirectionalLightDirection(3.0f, -1.0f, 5.0f);
	scene->SetDirectionalLightColor(Color(1.0f, 1.0f, 1.0f));
	scene->SetDirectionalLightSpecular(Color(0.5f, 0.5f, 0.5f));

	//PointLight
	pl1 = new PointLight(-5.0f, 2.0f, 5.0f);
	pl1->SetColor(Color(0.0f, 0.0f, 2.0f));
	pl1->SetSpecular(Color(0.0f, 0.0f, 2.0f));
	pl2 = new PointLight(5.0f, 2.0f, 5.0f);
	pl2->SetColor(Color(2.0f, 0.0f, 0.0f));
	pl2->SetSpecular(Color(2.0f, 0.0f, 0.0f));
	pl3 = new PointLight(-5.0f, 2.0f, -5.0f);
	pl3->SetColor(Color(0.0f, 2.0f, 0.0f));
	pl3->SetSpecular(Color(0.0f, 2.0f, 0.0f));
	pl4 = new PointLight(5.0f, 2.0f, -5.0f);
	pl4->SetColor(Color(2.0f, 2.0f, 2.0f));
	pl4->SetSpecular(Color(2.0f, 2.0f, 2.0f));

	//Add PointLight
	scene->AddPointLight(pl1);
	scene->AddPointLight(pl2);
	scene->AddPointLight(pl3);
	scene->AddPointLight(pl4);

	//SpotLight
	sl = new SpotLight(camera->x, camera->y, camera->z);
	sl->SetColor(Color(1.0f, 1.0f, 0.5f));

	//Add SpotLight
	scene->AddSpotLight(sl);

	//Sprite2D
	howTo = new Sprite2D();
	howTo->Load("source/howTo.png");

	scene->AddChild(howTo);

	logo = new Sprite2D();
	logo->Load("source/logo.png");

	logo->x = bis->GetScreenWidth() - (logo->width + 10.0f);
	logo->y = bis->GetScreenHeight() - logo->height;

	scene->AddChild(logo);

	bis->AddScene(scene);
	//bis->DeleteScene(scene);

	bis->ShowMouseCursor(false);
	bis->SetMouseInWindow(bis->GetScreenWidth() / 2.0f, bis->GetScreenHeight() / 2.0f);

	camera->LookAt(0.0f, 1.0f, 0.0f);
}

void CameraRotate()
{
	Mouse mo = bis->GetMousePosition();

	camera->rotationY -= 0.2f * (400-mo.x);
	camera->rotationX -= 0.2f * (300-mo.y);

	bis->SetMouseInWindow(400, 300);

	if(camera->rotationX >= 90.0f)
		camera->rotationX = 90.0f;
	else if(camera->rotationX <= -90.0f)
		camera->rotationX = -90.0f;
}

void CameraMove()
{
	if(upK)
		camera->MoveForward(0.1f);
	else if(downK)
		camera->MoveForward(-0.1f);
	if(leftK)
		camera->MoveRight(-0.1f);
	else if(rightK)
		camera->MoveRight(0.1f);
}

void Update()
{
	if(mouseLock)
	{
		CameraRotate();
		CameraMove();
	}

	sl->x = camera->x;
	sl->y = camera->y;
	sl->z = camera->z;
	sl->rotationX = camera->rotationX;
	sl->rotationY = camera->rotationY;
	sl->rotationZ = camera->rotationZ;
}

void Input(Event num)
{
	switch(num.type)
	{
	case KEYDOWN:
		switch(num.key.keysym.sym)
		{
		case KEY_UP:
		case KEY_W:
			upK = true;
			break;
		case KEY_DOWN:
		case KEY_S:
			downK = true;
			break;
		case KEY_LEFT:
		case KEY_A:
			leftK = true;
			break;
		case KEY_RIGHT:
		case KEY_D:
			rightK = true;
			break;
		case KEY_ESCAPE:
			bis->ShowMouseCursor(true);
			mouseLock = false;
			break;
		case KEY_SPACE:
			camera->LookAt(0.0f, 0.0f, 0.0f);
			break;
		}
		break;
	case KEYUP:
		switch(num.key.keysym.sym)
		{
		case KEY_UP:
		case KEY_W:
			upK = false;
			break;
		case KEY_DOWN:
		case KEY_S:
			downK = false;
			break;
		case KEY_LEFT:
		case KEY_A:
			leftK = false;
			break;
		case KEY_RIGHT:
		case KEY_D:
			rightK = false;
			break;
		}
		break;
	case MOUSEDOWN:
		if(MOUSELEFT)
		{
			bis->ShowMouseCursor(false);
			bis->SetMouseInWindow(bis->GetScreenWidth() / 2.0f, bis->GetScreenHeight() / 2.0f);
			mouseLock = true;
			break;
		}
		else if(MOUSERIGHT)
		{
			bis->ShowMouseCursor(true);
			mouseLock = false;
			break;
		}
		break;
	}
}

int main(int argc, char* argv[])
{
	bis = new BI3D("BagIdea3D", 800, 600);

	bis->SetEvent((void*)&Start, BI3D_EVENT_START);
	bis->SetEvent((void*)&Update, BI3D_EVENT_UPDATE);
	bis->SetEvent((void*)&Input, BI3D_EVENT_INPUT);

	bis->Start();

	bis->Close();

	return 0;
}
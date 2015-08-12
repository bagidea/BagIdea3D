#include "BI3D.h"

#include <cmath>

BI3D* bis;

Scene* scene;
Camera* camera;
Object* model;

Sprite2D* howTo;
Sprite2D* logo;
Sprite2D* frameScreen;

FrameBuffer* frameBuffer;

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

	camera = new Camera(45.0f, 0.1f, 500.0f);

	camera->y = 2.75f;
	camera->z = -4.0f;
	camera->rotationX = 20.0f;

	scene->SetCamera(camera);
	//scene->ClearCamera();

	model = new Object();
	model->Load("source/model/nanosuit/nanosuit.obj");

	model->x = 0.0f;
	//model2->y = 0.0f;
	//model2->z = 0.0f;
	model->rotationY = 180.0f;
	//model->rotationZ = 45.0f;
	//model->rotationX = 45.0f;
	model->scaleX = 0.2f;
	model->scaleY = 0.2f;
	model->scaleZ = 0.2f;

	scene->AddChild(model);
	//scene->DeleteChild(model);

	Object* ob = new Object();
	ob->Load("source/model/nanosuit/nanosuit.obj");

	//ob->y = -1.75f;
	ob->scaleX = 0.2f;
	ob->scaleY = 0.2f;
	ob->scaleZ = 0.2f;

	scene->CreatePrefab(ob, "sample_model");
	//scene->DeletePrefab(ob, "sample_model");

	for(int i = 0; i < 50; i++)
	{
		Object* ob_ = new Object();
		ob_->LoadPrefab(scene->GetPrefab("sample_model"));

		ob_->x = bis->Randomf(0.0f, 20.0f)-10.0f;
		ob_->z = bis->Randomf(0.0f, 20.0f);
		ob_->rotationY = bis->Randomf(0.0f, 360.0f)-180.0f;

		scene->AddChild(ob_);
	}

	//Sprite2D
	howTo = new Sprite2D();
	howTo->Load("source/howTo.png");

	scene->AddChild(howTo);

	logo = new Sprite2D();
	logo->Load("source/logo.png");

	logo->x = bis->GetScreenWidth() - (logo->width + 10.0f);
	logo->y = bis->GetScreenHeight() - logo->height;

	scene->AddChild(logo);

	//FrameBuffer
	frameScreen = new Sprite2D();

	frameScreen->FlipVertical = true;
	frameScreen->x = bis->GetScreenWidth() - 300.0f;
	frameScreen->width = 300.0f;
	frameScreen->height = 225.0f;

	scene->AddChild(frameScreen);

	frameBuffer = new FrameBuffer(bis->GetScreenWidth(), bis->GetScreenHeight());

	scene->AddFrameBuffer(frameBuffer);
	//scene->DeleteFrameBuffer(frameBuffer);

	bis->AddScene(scene);
	//bis->DeleteScene(scene);

	bis->ShowMouseCursor(false);
	bis->SetMouseInWindow(bis->GetScreenWidth() / 2.0f, bis->GetScreenHeight() / 2.0f);
}

void CameraRotate()
{
	Mouse mo = bis->GetMousePosition();

	camera->rotationY -= 0.25f * (400-mo.x);
	camera->rotationX -= 0.25f * (300-mo.y);

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

	//model->rotationY += 2.0f;
	model->LookAt(camera->x, camera->y, camera->z);

	frameScreen->SetTexture(frameBuffer->GetTexture());
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
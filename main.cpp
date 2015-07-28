#include "BI3D.h"

#include <cmath>

BI3D* bis;

Scene* scene;
Camera* camera;
Object* model;
Object* model2;

bool upK, downK, leftK, rightK;
bool upR, downR, leftR, rightR;

void Start()
{
	upK = false;
	downK = false;
	leftK = false;
	rightK = false;

	upR = false;
	downR = false;
	leftR = false;
	rightR = false;

	scene = new Scene();

	camera = new Camera(45.0f, 0.1f, 500.0f);

	camera->y = 1.75f;
	camera->z = -4.0f;
	camera->rotationX = 20.0f;

	scene->SetCamera(camera);
	//scene->ClearCamera();

	model = new Object();
	model->Load("source/model/nanosuit/nanosuit.obj");

	model->x = 2.0f;
	model->y = -1.75f;
	model->z = -1.0f;
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

	ob->y = -1.75f;
	ob->scaleX = 0.2f;
	ob->scaleY = 0.2f;
	ob->scaleZ = 0.2f;

	scene->CreatePrefab(ob, "sample_model");
	//scene->DeletePrefab(ob, "sample_model");

	for(int i = 0; i < 100; i++)
	{
		Object* ob_ = new Object();
		ob_->LoadPrefab(scene->GetPrefab("sample_model"));

		ob_->x = bis->Randomf(0.0f, 40.0f)-20.0f;
		ob_->z = bis->Randomf(0.0f, 40.0f);
		ob_->rotationY = bis->Randomf(0.0f, 360.0f)-180.0f;

		scene->AddChild(ob_);
	}

	model2 = new Object();
	model2->LoadPrefab(scene->GetPrefab("sample_model"));

	model2->x = -2.0f;
	model2->y = -1.75f;
	model2->z = -2.0f;
	model2->rotationY = 180.0f;
	//model->rotationZ = 45.0f;
	//model->rotationX = 45.0f;
	model2->scaleX = 0.2f;
	model2->scaleY = 0.2f;
	model2->scaleZ = 0.2f;

	scene->AddChild(model2);
	//scene->DeleteChild(model2);

	bis->AddScene(scene);
	//bis->DeleteScene(scene);

	bis->SetMouseInWindow(400, 300);
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
	CameraRotate();
	CameraMove();

	//model->rotationY += 2.0f;
	model->LookAt(camera->x, camera->y, camera->z);
	model2->LookAt(camera->x, camera->y, camera->z);
}

void Input(Event num)
{
	switch(num.type)
	{
	case KEYDOWN:
		switch(num.key.keysym.sym)
		{
		case KEY_UP:
			upR = true;
			break;
		case KEY_W:
			upK = true;
			break;
		case KEY_DOWN:
			downR = true;
			break;
		case KEY_S:
			downK = true;
			break;
		case KEY_LEFT:
			leftR = true;
			break;
		case KEY_A:
			leftK = true;
			break;
		case KEY_RIGHT:
			rightR = true;
			break;
		case KEY_D:
			rightK = true;
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
			upR = false;
			break;
		case KEY_W:
			upK = false;
			break;
		case KEY_DOWN:
			downR = false;
			break;
		case KEY_S:
			downK = false;
			break;
		case KEY_LEFT:
			leftR = false;
			break;
		case KEY_A:
			leftK = false;
			break;
		case KEY_RIGHT:
			rightR = false;
			break;
		case KEY_D:
			rightK = false;
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
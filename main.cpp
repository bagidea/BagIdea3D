#include "BI3D.h"

BI3D* bis;

Scene* scene;
Camera* camera;
Object* ob;

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

	camera->y = 3.0f;
	camera->z = -3.0f;

	camera->rotationX = 30.0f;

	scene->AddCamera(camera);

	ob = new Object();
	ob->Load("source/model/nanosuit/nanosuit.obj");

	ob->x = 0.0f;
	ob->y = -1.75f;
	ob->z = 0.0f;

	ob->rotationY = 180.0f;
	
	ob->scaleX = 0.2f;
	ob->scaleY = 0.2f;
	ob->scaleZ = 0.2f;

	scene->AddChild(ob);
	bis->AddScene(scene);

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

	ob->rotationY += 1.0f;
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

	delete bis;
	return 0;
}
#include "BI3D.h"

BI3D* bis;

Scene* scene;
Object* ob;

void Start()
{
	scene = new Scene();

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
}

void Update()
{
}

int main(int argc, char* argv[])
{
	bis = new BI3D("BagIdea3D", 800, 600);

	bis->SetEvent((void*)&Start, BI3D_EVENT_START);
	bis->SetEvent((void*)&Update, BI3D_EVENT_UPDATE);

	bis->Start();

	delete bis;

	return 0;
}
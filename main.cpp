#include "BI3D.h"

BI3D* bis;

void Start()
{
}

void Update()
{
}

int main(int argc, char* argv[])
{
	bis = new BI3D("BagIdea3D", 800, 600);

	bis->SetEvent((void*)&Update, BI3D_EVENT_START);
	bis->SetEvent((void*)&Update, BI3D_EVENT_UPDATE);

	bis->Start();

	delete bis;

	return 0;
}
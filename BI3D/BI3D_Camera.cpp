#include "BI3D_Camera.h"

Camera::Camera(GLfloat fov, GLfloat near, GLfloat far)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;

	this->fov = fov;
	this->near = near;
	this->far = far;
	ratio = 0.0f;
}

void Camera::SetRatio(GLfloat ratio)
{
	this->ratio = ratio;
}

void Camera::MoveForward(GLfloat speed)
{
	glm::vec3 camDir(cosf(-rotationX*(MATH_PI/180.0f)) * sinf(-rotationY*(MATH_PI/180.0f)), sinf(-rotationX*(MATH_PI/180.0f)), cosf(-rotationX*(MATH_PI/180.0f)) * cosf(-rotationY*(MATH_PI/180.0f)));

	glm::vec3 moveTo(x, y, z);
	moveTo += speed * camDir;

	x = moveTo.x;
	y = moveTo.y;
	z = moveTo.z;
}

void Camera::MoveRight(GLfloat speed)
{
	glm::vec3 camRight(sinf(-rotationY*(MATH_PI/180.0f) - MATH_PI/2.0f), 0.0f, cosf(-rotationY*(MATH_PI/180.0f) - MATH_PI/2.0f));

	glm::vec3 moveTo(x, y, z);
	moveTo += speed * camRight;

	x = moveTo.x;
	y = moveTo.y;
	z = moveTo.z;
}

void Camera::LookAt(GLfloat x, GLfloat y, GLfloat z)
{
	glm::vec3 Pos(this->x, this->y, this->z);
	glm::vec3 Target(x, y, z);

	glm::mat4 m = glm::lookAt(Pos, Target, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::quat Rotate = glm::conjugate(glm::quat_cast(m));
	glm::vec3 v = glm::eulerAngles(Rotate);

	rotationZ = glm::degrees(0.0f);

	if(Pos.z > Target.z)
		rotationX = glm::degrees(-v.x);
	else
		if(Pos.y > Target.y)
			rotationX = glm::degrees(-v.x)+180.0f;
		else
			rotationX = glm::degrees(-v.x)-180.0f;

	if(Pos.z > Target.z)
		if(Pos.x > Target.x)
			rotationY = glm::degrees(-v.y)-180.0f;
		else
			rotationY = glm::degrees(-v.y)+180.0f;
	else
		rotationY = glm::degrees(v.y);

	rotationZ = glm::degrees(0.0f);
}

glm::mat4 Camera::GetTransform()
{
	if(rotationX >= 360.0f)
		rotationX = 360.0f;
	else if(rotationX <= -360.0f)
		rotationX = -360.0f;

	if(rotationY >= 360.0f)
		rotationY = 360.0f;
	else if(rotationY <= -360.0f)
		rotationY = -360.0f;

	if(rotationZ >= 360.0f)
		rotationZ = 360.0f;
	else if(rotationZ <= -360.0f)
		rotationZ = -360.0f;
	
	if(rotationX >= 180.0f)
		rotationX = -180.0f+(rotationX-180.0f);
	else if(rotationX <= -180.0f)
		rotationX = 180.0f+(rotationX+180.0f);

	if(rotationY >= 180.0f)
		rotationY = -180.0f+(rotationY-180.0f);
	else if(rotationY <= -180.0f)
		rotationY = 180.0f+(rotationY+180.0f);

	if(rotationZ >= 180.0f)
		rotationZ = -180.0f+(rotationZ-180.0f);
	else if(rotationZ <= -180.0f)
		rotationZ = 180.0f+(rotationZ+180.0f);

	glm::mat4 trans;

	glm::vec3 camPos(x, y, z);

	glm::vec3 camDir(cosf(-rotationX*(MATH_PI/180.0f)) * sinf(-rotationY*(MATH_PI/180.0f)), sinf(-rotationX*(MATH_PI/180.0f)), cosf(-rotationX*(MATH_PI/180.0f)) * cosf(-rotationY*(MATH_PI/180.0f)));
	glm::vec3 camRight(sinf(-rotationY*(MATH_PI/180.0f) - MATH_PI/2.0f), 0.0f, cosf(-rotationY*(MATH_PI/180.0f) - MATH_PI/2.0f));
	glm::vec3 camUp = cross(camRight, camDir);

	trans = glm::lookAt(camPos, camPos + camDir, camUp);
	trans = glm::rotate(trans, rotationZ*(MATH_PI/180.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	return trans;
}
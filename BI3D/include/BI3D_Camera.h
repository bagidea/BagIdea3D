#ifndef BI3D_CAMERA_INCLUDE_H
#define BI3D_CAMERA_INCLUDE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MATH_PI 3.14159265359f

class Camera
{
public:
	GLfloat x, y, z, rotationX, rotationY, rotationZ;

	Camera(GLfloat fov, GLfloat near, GLfloat far);

	void SetRatio(GLfloat ratio);
	void MoveForward(GLfloat speed);
	void MoveRight(GLfloat speed);

	glm::mat4 GetTransform();
private:
	GLfloat fov, near, far, ratio;
};

#endif
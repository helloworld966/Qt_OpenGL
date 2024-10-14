#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include<QMatrix4x4>
#include<vector>


//移动方向,这里是wasd

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float sensitivity = 0.1f;
const float ZOOM = 45.0f;

//一个camera抽象类，处理输入并计算相应的Euler角度、向量和矩阵，以便在OpenGL中使用
class Camera {
public:
	QVector3D Position;
	QVector3D Front;
	QVector3D up;
	QVector3D Right;
	QVector3D WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float Mousesensitivity;
	float Zoom;

	Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,float yaw,float pitch):Front(QVector3D(0.0f, 0.0f, 0.0f))
	{
		Position = QVector3D(posX, posY, posZ);
		WorldUp = QVector3D(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	QMatrix4x4 GetViewMatrix()
	{
		QMatrix4x4 theMatrix;
		theMatrix.lookAt(Position, Position + Front, up);
		return theMatrix;
	}

	//处理任何从键盘中的输入系统接收的输入
	void ProssKeyboard(Camera_Movement direction, float deltatime)
	{
		float velocity = MovementSpeed * deltatime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}
	//处理鼠标输入，需要x和y方向上的偏移量
	void ProssKeyMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= Mousesensitivity;
		yoffset *= Mousesensitivity;

		Yaw += xoffset;
		Pitch += yoffset;


		if (Yaw > 89.0f)
			Yaw = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;


		updateCameraVectors();
	}
	//处理鼠标滚轮
	void ProssMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 75.0f)
			Zoom = 75.0f;
	}

private:
	void updateCameraVectors()
	{
		float PI = 3.1415926;
		QVector3D front;
		front.setX(cos(Yaw*PI / 180.0)*cos(Pitch*PI / 180.0));
		front.setY(sin(Pitch*PI / 180.0));
		front.setZ(sin(Yaw*PI / 180.0)*cos(Pitch*PI / 180.0));
		front.normalize();
		Front = front;
		Right = QVector3D::crossProduct(Front, WorldUp);
		Right.normalize();
		up = QVector3D::crossProduct(Right, Front);
		up.normalize();
	}
};



#endif // !CAMERA_H

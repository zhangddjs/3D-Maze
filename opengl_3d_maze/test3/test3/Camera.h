#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Prim.h"

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 cameraF, cameraR;


	GLfloat Yaw;
	GLfloat Pitch;

	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;
		//碰撞检测
		cameraF = cameraR = this->Position;
		int x1 = (int)(((cameraF).x) / 2);
		int z1 = (int)(((cameraF).z) / 2);
		int xf0 = (int)(((cameraF).x) / 2 + 2 * this->Front.x * velocity);
		int zf0 = (int)(((cameraF).z) / 2 + 2 * this->Front.z * velocity);
		int xb0 = (int)(((cameraF).x) / 2 - 2 * this->Front.x * velocity);
		int zb0 = (int)(((cameraF).z) / 2 - 2 * this->Front.z * velocity);
		int xr0 = (int)(((cameraF).x) / 2 + 2 * this->Right.x * velocity);
		int zr0 = (int)(((cameraF).z) / 2 + 2 * this->Right.z * velocity);
		int xl0 = (int)(((cameraF).x) / 2 - 2 * this->Right.x * velocity);
		int zl0 = (int)(((cameraF).z) / 2 - 2 * this->Right.z * velocity);
		if (direction == FORWARD)
		{
			//前方是否有墙
			if (this->collide(x1, z1, xf0, zf0, velocity, 1))
			{
				this->Position += this->Front * velocity;
			}			
		}
		if (direction == BACKWARD)
		{
			//后方是否有墙
			if (this->collide(x1, z1, xb0, zb0, velocity, 2))
			{
				this->Position -= this->Front * velocity;
			}
		}
		if (direction == LEFT)
		{
			if (this->collide(x1, z1, xl0, zl0, velocity, 4))
			{
				this->Position -= this->Right * velocity;
			}
		}
		if (direction == RIGHT)
		{
			if (this->collide(x1, z1, xr0, zr0, velocity, 3))
			{
				this->Position += this->Right * velocity;
			}
		}
	 	if (direction != UP)
	 	{
		 	this->Position.y = 1.0f; // <-- this one-liner keeps the user at the ground level (xz plane)
	 	}
		if (direction == UP)
		{
			
				this->Position.y += 0.3 * velocity;
			
		}
	}

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		this->updateCameraVectors();
	}

	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f	&& this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

private:
	bool collide(int x1, int z1, int xd, int zd, GLfloat velocity, int dire)
	{
		if (((b[z1][xd] == 0 && c[z1][xd] == 0)
			|| (b[zd][x1] == 0 && c[zd][x1] == 0)
			|| (b[zd][xd] == 0 && c[zd][xd] == 0))
			
			)
		{
			return false;
		}
		//判断是否出界……
		else
		{
			switch (dire)
			{
			case 1:
				if (cameraF.x + 2 *this->Front.x * velocity < 0
					|| cameraF.z + 2 * this->Front.z * velocity < 0)
				{
					return false;
				}
				else
				{
					return true;
				}
			case 2:
				if (cameraF.x - 2 * this->Front.x * velocity < 0
					|| cameraF.z - 2 * this->Front.z * velocity < 0)
				{
					return false;
				}
				else
				{
					return true;
				}
			case 3:
				if (cameraR.x + 2 * this->Right.x * velocity < 0
					|| cameraR.z + 2 * this->Right.z * velocity < 0)
				{
					return false;
				}
				else
				{
					return true;
				}
			case 4:
				if (cameraR.x - 2 * this->Right.x * velocity < 0
					|| cameraR.z - 2 * this->Right.z * velocity < 0)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		}
	}
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};
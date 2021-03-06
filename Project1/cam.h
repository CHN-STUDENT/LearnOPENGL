﻿#ifndef CAMERA_H
#define CAMERA_H

/*原版C++:https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
此版本修改于原版为了适应无法用C++的必要*/

#include <cglm/cglm.h>
//使用CGLM替代GLM库为了适应无法用C++的必要

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;
// Camera Attributes
vec3 Position;
vec3 Front= { 0.0f, 0.0f, -1.0f };
vec3 Up;
vec3 Right;
vec3 WorldUp;
// Eular Angles
float Yaw;
float Pitch;
// Camera options
float MovementSpeed;
float MouseSensitivity;
float Zoom;
void updateCameraVectors();

float fov_back()
{
	return Zoom;
}

void Camera(vec3 position,vec3 up,float yaw,float pitch)
{
	MovementSpeed = SPEED; 
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;
	glm_vec_copy(position, Position);
	glm_vec_copy(up,WorldUp);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}
void UCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	vec3 up = { upX, upY, upZ };
	vec3 position = { posX, posY, posZ };
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;
	glm_vec_copy(position, Position);
	glm_vec_copy(up, WorldUp);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

void GetViewMatrix(mat4 dest)
{//直接参数返回观察矩阵
	vec3 temp;
	glm_vec_add(Position, Front, temp);
	glm_lookat(Position,temp, Up, dest);
}

void ProcessKeyboard(char * direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	vec3 velocity_vec = { velocity ,velocity ,velocity };
	vec3 t;
	
	if ( strcmp(direction,"FORWARD")==0 )
	{
		glm_vec_mulv(Front, velocity_vec, t);
		glm_vec_add(Position, t, Position);
	}
	
	if (strcmp(direction, "BACKWARD") == 0)
	{
		glm_vec_mulv(Front, velocity_vec, t);
		glm_vec_sub(Position, t, Position);
	}
	if (strcmp(direction, "LEFT") == 0)
	{
		glm_vec_mulv(Right, velocity_vec, t);
		glm_vec_sub(Position, t, Position);
		//Position -= Right * velocity;
	}
	if (strcmp(direction, "RIGHT") == 0)
	{
		glm_vec_mulv(Right, velocity_vec, t);
		glm_vec_add(Position, t, Position);
		//Position += Right * velocity;
	}
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void ProcessMouseMovement(float xoffset, float yoffset,GLboolean constrainPitch = true)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void updateCameraVectors()
{
	// Calculate the new Front vector
	vec3 front = { cos(glm_rad(Yaw)) * cos(glm_rad(Pitch)),  sin(glm_rad(Pitch)),sin(glm_rad(Yaw)) * cos(glm_rad(Pitch)) };
	glm_vec_normalize(front);
	glm_vec_copy(front, Front);
	// Also re-calculate the Right and Up vector
	vec3 right_cross_temp;
	glm_vec_cross(Front, WorldUp, right_cross_temp);
	glm_vec_normalize(right_cross_temp);
	glm_vec_copy(right_cross_temp, Right);// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	vec3 up_cross_temp;
	glm_vec_cross(Right, Front, up_cross_temp);
	glm_vec_normalize(up_cross_temp);
	glm_vec_copy(up_cross_temp, Up);
}
	
#endif

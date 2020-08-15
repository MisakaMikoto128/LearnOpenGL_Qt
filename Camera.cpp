#include "Camera.h"
#include "angle.h"
#include <cmath>
Camera::~Camera()
{
}



Camera::Camera(QVector3D position, QVector3D target, QVector3D worldup) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = worldup;
    Front = (target - position).normalized();
    // also re-calculate the Right and Up vector
    Right = QVector3D::crossProduct(Front, WorldUp).normalized();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = QVector3D::crossProduct(Right, Front).normalized();
}

// constructor with vectors
Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}
// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = QVector3D(posX, posY, posZ);
    WorldUp = QVector3D(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}
#include <iostream>
// returns the view matrix calculated using Euler Angles and the LookAt Matrix
QMatrix4x4 Camera::GetViewMatrix()
{
    //Position + Front����˼�����Ŀ�������������еķ���
    QMatrix4x4 view;
    view.lookAt(Position, Position + Front, Up);
    return view;
}

void Camera::processInput(float dt)
{
    if (keys[Qt::Key_W])
      ProcessKeyboard(FORWARD, dt);
    if (keys[Qt::Key_S])
      ProcessKeyboard(BACKWARD, dt);
    if (keys[Qt::Key_A])
      ProcessKeyboard(LEFT, dt);
    if (keys[Qt::Key_D])
      ProcessKeyboard(RIGHT, dt);
    if (keys[Qt::Key_E])
      ProcessKeyboard(UP, dt);
    if (keys[Qt::Key_Q])
      ProcessKeyboard(DOWN, dt);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw     -= xoffset;
    Pitch   -= yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset*MouseSensitivity;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    QVector3D front;
    front.setX(cos(radians(Yaw)) * cos(radians(Pitch)));
    front.setY(sin(radians(Pitch)));
    front.setZ(sin(radians(Yaw)) * cos(radians(Pitch)));
    Front = front.normalized();
    // also re-calculate the Right and Up vector
    Right = QVector3D::crossProduct(Front, WorldUp).normalized();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = QVector3D::crossProduct(Right, Front).normalized();
}

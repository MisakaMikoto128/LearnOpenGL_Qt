#ifndef _CAMERA_H
#define _CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QOpenGLFunctions>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:

	// constructor with vectors
    Camera(QVector3D position, QVector3D target,
        QVector3D worldup);

	// constructor with vectors
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	~Camera();

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    QMatrix4x4 GetViewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
    void processInput(float dt);

private:
    QVector3D Position;
    QVector3D Front;
    QVector3D Right;
    QVector3D Up;
    QVector3D WorldUp;

	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;




    // calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
public:
	float Zoom;
    //Keyboard multi-touch
    bool keys[1024];
};



#endif // !_CAMERA_H

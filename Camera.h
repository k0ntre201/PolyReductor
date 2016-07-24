#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace MyEngine
{
	namespace Renderer
	{
		class Camera
		{
		public:
			Camera(const glm::vec3& pos, const glm::vec3& target, float aspectRatio, float FOV, float zNear, float zFar, glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f));
			virtual ~Camera();
			const glm::mat4& getViewMatrix();
			const glm::mat4& getProjectionMatrix();
			const glm::mat4& getViewProjectionMatrix();
			void moveForward(float dt);
			void moveBack(float dt);
			void moveLeft(float dt);
			void moveRight(float dt);
			const glm::vec3& getCameraPosition() const;
			float getCameraFOV();
			float getCameraAspectrario();
			void setAspectRatio(float aspectRatio);
			void setSpeed(float s);
			void addYaw(float yawOffsed);
			void addPitch(float pithOffsed);

			void update();

		private:
			glm::mat4 viewMatrix;
			glm::mat4 _projectionMatrix;
			glm::mat4 VP;
			glm::vec3 pos;
			glm::vec3 up;
			glm::vec3 right;
			glm::vec3 target;
			float aspectRatio;
			float FOV;
			float zNear;
			float zFar;
			float speed{ 2.0f };
			float yaw{ 0.0f };
			float pith{ 0.0f };
			float yawOff{ 0.0f }, pitchOff{ 0.0f };
		private:
			void updatePerspectiveMatrix();
			void updateViewMatrix();
		};
	}
}
#endif //!CAMERA_H

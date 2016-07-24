#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm\glm.hpp>

namespace MyEngine
{
	namespace Renderer
	{
		class Transform
		{
		public:
			Transform();
			Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale = glm::vec3(1.0f));
			virtual ~Transform();

			void move(const glm::vec3& dxyz);
			void setPosition(const glm::vec3 position);
			glm::vec3 getPosiotion();
			const glm::vec3& getPosition() const;

			void rotate(const glm::vec3& dxyz);
			void setRotate(const glm::vec3 rotate);
			glm::vec3 getRotate();
			const glm::vec3& getRotate() const;

			void scale(const glm::vec3& dxyz);
			void setScale(const glm::vec3 scale);
			glm::vec3 getScale();
			const glm::vec3& getScale() const;

			void update();

			glm::mat4 getModelMatrix() { return modelMatrix; }
			const glm::mat4& getModelMatrix() const { return modelMatrix; }
		private:
			glm::mat4 modelMatrix;
			glm::vec3 pos, rot, scal;
		};
	}
}
#endif // !TRANSFORM_H


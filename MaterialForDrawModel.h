#ifndef MATERIAL_FOR_DRAW_MODEL
#define MATERIAL_FOR_DRAW_MODEL

#include "MaterialsLibary.h"
#include "ShaderGLSL.h"
#include <memory>


namespace PolyReductor
{
	namespace Renderer
	{
		class MaterialForDrawModel
		{
		public:
			MaterialForDrawModel();

			void setMaterial(MaterialType type = MaterialType::COPPER);
			void bindMatrial(const glm::vec3& lightPosition /*the best will be eye position*/, const glm::vec3& eyePosition);

			GLint getMaterialProgram() { return shaderProgram; }

			~MaterialForDrawModel() {}
		protected:
		private:
			GLint shaderProgram;
			std::shared_ptr<MyEngine::Renderer::ShaderGlsl> shader;
			Material mat;
		};
	}
}

#endif // !MATERIAL_FOR_DRAW_MODEL

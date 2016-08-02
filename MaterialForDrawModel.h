#ifndef MATERIAL_FOR_DRAW_MODEL
#define MATERIAL_FOR_DRAW_MODEL

#include "MaterialsLibary.h"
#include "ShaderGLSL.h"
#include <memory>

/*
 *Class for drawing the objec in material from MaterialLibary file.
 *This class use Shader class from MyEngine namespace. This class is implements by Sylwester Mróz.
*/

namespace PolyReductor
{
	namespace Renderer
	{
		class MaterialForDrawModel
		{
		public:
			MaterialForDrawModel();

			void setMaterial(MaterialType type = MaterialType::COPPER);/*Set material type for objecs.*/
			void bindMatrial(const glm::vec3& lightPosition /*the best will be eye position*/, const glm::vec3& eyePosition);/*bind material, shaders, uniforms variables... Modre in MaterialForDrawModel.cpp file.*/

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

/*
Class for shader program manipulation.
Basicly, one shader program need minimum one vertex file and fragment file.
Rest are optionally.
*/

#ifndef SHADERGLSL_H
#define SHADERGLSL_H

#include "OpenGLLibaries.h"
#include <string>
#include <vector>

namespace MyEngine
{
	namespace Renderer
	{
		class ShaderGlsl
		{
		public:
			ShaderGlsl();
			~ShaderGlsl();
			//We won't to have basicly mathematic operation on shader program...
			ShaderGlsl& operator+=(ShaderGlsl&) = delete;
			ShaderGlsl& operator-=(ShaderGlsl&) = delete;
			ShaderGlsl& operator*=(ShaderGlsl&) = delete;
			ShaderGlsl& operator/=(ShaderGlsl&) = delete;
		public:
			/*functions to add source of all parts of shader program*/
			void loadVertexShader(const char* filename);
			void loadFragmentShade(const char* filename);
			void loadGeometryShader(const char* filename);
			void loadConstrolTesselationShader(const char* filename);
			void loadEvaluationTesselationShader(const char* filename);

			//after addition of all parts of shader, this parts must be attached to shader program
			void attachAllShader();
			
			void useShader() { glUseProgram(program); }
			void detachShader() { glUseProgram(0); }
			//return shader program
			GLuint getProgram() { return program; }
		protected:
		private:
			std::string readShader(const char* filename);
			void checkShaderError(GLuint& sh);
			GLuint loadShader(const char* filename, GLenum mode);
			void linkProgram();
			void checkLinkedProgramError();
			void validateProgram();
			void checkValidateProgramError();
		private:
			GLuint program;
			std::string filename;
			std::vector<GLuint> shaders;
		};
	}
}
#endif // !SHADERGLSL_H


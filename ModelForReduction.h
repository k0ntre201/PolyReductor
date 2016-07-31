#ifndef MODEL_FOR_REDUCTION_H
#define MODEL_FOR_REDUCTION_H

#include <vector>
#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <glm\glm.hpp>
#include <memory>
#include "OpenGLLibaries.h"

namespace PolyReductor
{
	namespace Renderer
	{
		class ModelForReduction
		{
			struct Triangle;

			struct Vertex
			{
				Vertex(const glm::vec3& pos, int id);
				~Vertex();

				glm::vec3 position;
				int id;
				std::vector<std::shared_ptr<Vertex>> neighbor;
				std::vector<std::shared_ptr<Triangle>> face;
				float cost;
				std::unique_ptr<Vertex> collapse;

				void RemoveIfNonNeighbor(std::unique_ptr<Vertex> n);

				bool operator==(const Vertex& v)
				{
					return id == v.id;
				}

				bool operator==(std::unique_ptr<Vertex> v)
				{
					return id == v->id;
				}
			};

			struct Triangle
			{
				Triangle(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
				~Triangle();
				std::shared_ptr<Vertex> vertex[3];
				glm::vec3 normal;
				void computeNormal();
				void replaceVertex(std::shared_ptr<Vertex> vold, std::shared_ptr<Vertex> vnew);
				int hasVertex(std::shared_ptr<Vertex> v);
			};
		public:
			ModelForReduction();
			ModelForReduction(const std::string& modelName);
			void loadModel(const std::string& modelName);
			void saveModel(const std::string& modelName);

			void prepareModelToDraw();


			void draw(GLuint shaderProgram, const glm::mat4& M, const glm::mat4& V, const glm::mat4& P);

			void deleteBUffers();

		protected:
		private://methods
			void addNeigbhod(std::shared_ptr<Vertex> v, std::shared_ptr<Vertex> neighbor);
			std::shared_ptr<Triangle> addTriangle(int v1, int v2, int v3);

			void prepareVertices();
			void prepareNormals();
			void prepareIndices();


			template<typename T>
			void generateBuffer(const std::unique_ptr<T[]>& v, int vboType)
			{
				glGenBuffers(1, &vbos[vboType]);
				glBindBuffer(GL_ARRAY_BUFFER, vbos[vboType]);
				glBufferData(GL_ARRAY_BUFFER, sizeOfBuffer*sizeof(T), v.get(), GL_STATIC_DRAW);
					
				glVertexAttribPointer(vboType, sizeof(T) / sizeof(float), GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(vboType);
			}

		private:
			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Triangle>> triangles;
			std::unique_ptr<glm::vec3[]> positionSForDrawing;
			std::unique_ptr<glm::vec3[]> normalsForDrawing;
			std::unique_ptr<GLuint[]> indicesForDrawing;

			int sizeOfBuffer;

			GLuint vbos[3];
			GLuint vao;
		};
	}
}

#endif // !MODEL_FOR_REDUCTION_H


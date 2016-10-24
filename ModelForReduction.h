#ifndef MODEL_FOR_REDUCTION_H
#define MODEL_FOR_REDUCTION_H

#include <vector>
#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <glm\glm.hpp>
#include <memory>
#include "OpenGLLibaries.h"
#include "MyVertex.h"
#include "Histograms.h"


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
				glm::vec3 normal;
				int id;
				MyVector<std::shared_ptr<Vertex>> neighbor;
				MyVector<std::shared_ptr<Triangle>> face;
				float cost;
				std::shared_ptr<Vertex> candidate;

				void RemoveIfNonNeighbor();

				void calculateNormal();
				void calculateCostForDelete();

				

				bool operator==(const Vertex& v)
				{
					return id == v.id;
				}

				bool operator==(std::shared_ptr<Vertex> v)
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
				bool hasVertex(std::shared_ptr<Vertex> v);

				bool operator=(const Triangle& t)
				{
					bool result{ false };

					if (t.vertex[0]->id == vertex[0]->id && t.vertex[1]->id == vertex[1]->id && t.vertex[2]->id == vertex[2]->id)
						result = true;
					else if (t.vertex[0]->id == vertex[1]->id && t.vertex[1]->id == vertex[0]->id && t.vertex[2]->id == vertex[2]->id)
						result == true;
					else if (t.vertex[0]->id == vertex[0]->id && t.vertex[1]->id == vertex[2]->id && t.vertex[2]->id == vertex[1]->id)
						result == true;
					else if (t.vertex[0]->id == vertex[2]->id && t.vertex[1]->id == vertex[1]->id && t.vertex[2]->id == vertex[0]->id)
						result == true;


					return result;
				}
			};
		public:
			ModelForReduction();
			ModelForReduction(const std::string& modelName);
			void loadModel(const std::string& modelName);
			void saveModel(const std::string& modelName);

			void prepareModelToDraw();

			void reduce(int numberOfVertices);
			void draw(GLuint shaderProgram, const glm::mat4& M, const glm::mat4& V, const glm::mat4& P);

			void createHistograms(GLuint shaderProgram, std::shared_ptr<MyEngine::Renderer::Transform> transform, const glm::mat4& V, const glm::mat4& P, int width, int height);

			Histograms getHistograms();

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
			void calculateCostForVertices();
			void findCandidate();
			void removeVertex();

			void searchOnAllVerices();
			void searchOnLocalVerices();


		private:
			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Triangle>> triangles;
			std::unique_ptr<glm::vec3[]> positionSForDrawing;
			std::unique_ptr<glm::vec3[]> normalsForDrawing;
			std::unique_ptr<GLuint[]> indicesForDrawing;
			std::shared_ptr<Vertex> forRemove;
			std::shared_ptr<Vertex> prevForRemove;
			Histograms histograms;

			int sizeOfBuffer;

			GLuint vbos[3];
			GLuint vao;

			bool firstVertex;

			float cost = 0.0f;
		};
	}
}

#endif // !MODEL_FOR_REDUCTION_H


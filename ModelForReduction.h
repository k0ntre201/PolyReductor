#ifndef MODEL_FOR_REDUCTION_H
#define MODEL_FOR_REDUCTION_H

#include <vector>
#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <glm\glm.hpp>
#include <memory>

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

			void draw();

		protected:
		private://methods
			void addNeigbhod(std::shared_ptr<Vertex> v, std::shared_ptr<Vertex> neighbor);
			std::shared_ptr<Triangle> addTriangle(int v1, int v2, int v3);


		private:
			std::vector<std::shared_ptr<Vertex>> vertices;
			std::vector<std::shared_ptr<Triangle>> triangles;
		};
	}
}

#endif // !MODEL_FOR_REDUCTION_H


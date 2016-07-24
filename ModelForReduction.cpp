#include "ModelForReduction.h"
#include <assimp\postprocess.h>
#include <algorithm>

PolyReductor::Renderer::ModelForReduction::Vertex::Vertex(const glm::vec3 & pos, int id):
	position(pos),
	id(id)
{
}

PolyReductor::Renderer::ModelForReduction::Vertex::~Vertex()
{
}

void PolyReductor::Renderer::ModelForReduction::Vertex::RemoveIfNonNeighbor(std::unique_ptr<Vertex> n)
{
	//to do
	for (int i = 0; i < neighbor.size(); ++i)
	{
	
	}
}

PolyReductor::Renderer::ModelForReduction::Triangle::Triangle(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2)
{
	vertex[0] = v0;
	vertex[1] = v1;
	vertex[2] = v2;
}

PolyReductor::Renderer::ModelForReduction::Triangle::~Triangle()
{
}

void PolyReductor::Renderer::ModelForReduction::Triangle::computeNormal()
{
	glm::vec3 t1 = vertex[0]->position - vertex[1]->position;
	glm::vec3 t2 = vertex[0]->position - vertex[2]->position;
	normal = glm::normalize(glm::cross(t1, t2));
}

void PolyReductor::Renderer::ModelForReduction::Triangle::replaceVertex(std::shared_ptr<Vertex> vold, std::shared_ptr<Vertex> vnew)
{
	for (int i = 0; i < 3; ++i)
		if (vertex[i]->id == vold->id)
		{
			std::swap(vertex[i], vnew);
			return;
		}
}

int PolyReductor::Renderer::ModelForReduction::Triangle::hasVertex(std::shared_ptr<Vertex> v)
{
	for (int i = 0; i < 3; ++i)
		if (vertex[i]->id == v->id)
			return true;
	return false;
}

PolyReductor::Renderer::ModelForReduction::ModelForReduction()
{
}

PolyReductor::Renderer::ModelForReduction::ModelForReduction(const std::string & modelName)
{
	loadModel(modelName);
}

void PolyReductor::Renderer::ModelForReduction::loadModel(const std::string & modelName)
{
	//we accept scene with only one model
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(modelName, aiProcess_Triangulate);
	const aiMesh* mesh = scene->mMeshes[0];

	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		glm::vec3 position;
		std::shared_ptr<Vertex> vertex(new Vertex(position, i));
		vertices.push_back(std::move(vertex));
	}

	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		int v0 = mesh->mFaces[i].mIndices[0];
		int v1 = mesh->mFaces[i].mIndices[1];
		int v2 = mesh->mFaces[i].mIndices[2];
		addNeigbhod(vertices[v0], vertices[v1]);
		addNeigbhod(vertices[v0], vertices[v2]);
		addNeigbhod(vertices[v1], vertices[v0]);
		addNeigbhod(vertices[v1], vertices[v2]);
		addNeigbhod(vertices[v2], vertices[v1]);
		addNeigbhod(vertices[v2], vertices[v0]);
		triangles.push_back(addTriangle(v0, v1, v2));
		vertices[v0]->face.push_back(triangles[i]);
		vertices[v1]->face.push_back(triangles[i]);
		vertices[v2]->face.push_back(triangles[i]);
	}
	
}

void PolyReductor::Renderer::ModelForReduction::saveModel(const std::string & modelName)
{
}

void PolyReductor::Renderer::ModelForReduction::draw()
{
}

void PolyReductor::Renderer::ModelForReduction::addNeigbhod(std::shared_ptr<Vertex> v, std::shared_ptr<Vertex> neighbor)
{
	auto it = std::find_if(std::begin(v->neighbor), std::end(v->neighbor), [&](std::shared_ptr<Vertex> vert) {return vert->id == neighbor->id; });
	if (it == std::end(v->neighbor))
		v->neighbor.push_back(neighbor);
}

std::shared_ptr<PolyReductor::Renderer::ModelForReduction::Triangle> PolyReductor::Renderer::ModelForReduction::addTriangle(int v1, int v2, int v3)
{	
	std::shared_ptr<Triangle>triangle (new Triangle(vertices[v1], vertices[v2], vertices[v3]));
	triangle->computeNormal();
	return std::move(triangle);
}

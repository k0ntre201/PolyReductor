#include "ModelForReduction.h"
#include <assimp\postprocess.h>
#include <algorithm>
#include <iostream>
#include <glm\gtc\type_ptr.hpp>

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
	
	const aiScene* scene = importer.ReadFile(modelName, aiProcess_Triangulate| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	const aiMesh* mesh = scene->mMeshes[0];

	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
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

void PolyReductor::Renderer::ModelForReduction::prepareModelToDraw()
{
	sizeOfBuffer = vertices.size();
	positionSForDrawing.reset(new glm::vec3[sizeOfBuffer]);
	normalsForDrawing.reset(new glm::vec3[sizeOfBuffer]);
	indicesForDrawing.reset(new GLuint[triangles.size() * 3]);
	prepareVertices();
	prepareNormals();
	prepareIndices();
	//std::cin.get();
	for (short i = 0; i < 3; ++i)
		vbos[i] = 0;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	generateBuffer<glm::vec3>(positionSForDrawing, 0);
	generateBuffer<glm::vec3>(normalsForDrawing, 1);
	//for indicies
	glGenBuffers(1, &vbos[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size()*3*sizeof(GLuint), indicesForDrawing.get(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PolyReductor::Renderer::ModelForReduction::draw(GLuint shaderProgram, const glm::mat4& M,const glm::mat4& V, const glm::mat4& P)
{
	/*std::cout << "Vertices\n";

	//vertices[1] = nullptr;
	//vertices[1].reset();
	for (auto v : vertices)
	{
		std::cout << v->id << " " << v->position.x << " " << v->position.y << " " << v->position.z << "\n";
		std::cout << "Neghbor:\n";
		for (auto n : v->neighbor)
		{
			if(n!=nullptr)
				std::cout << n->id << " ";
		}
		std::cout << "\n";
	}


	std::cout << "\nTriangles\n";
	for (auto t : triangles)
		std::cout << t->vertex[0]->id<< " " << t->vertex[1]->id << " " << t->vertex[2]->id << "\n";*/

	glBindAttribLocation(shaderProgram, 0, "inPosition");
	glBindAttribLocation(shaderProgram, 1, "inNormal");

	glm::mat4 MV = V*M;
	glm::mat4 mvp = P*MV;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, triangles.size()*3, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void PolyReductor::Renderer::ModelForReduction::deleteBUffers()
{

	for (short i = 0; i < 3; ++i)
	{
		if (vbos[i])
			glDeleteBuffers(1, &vbos[i]);
	}

	glDeleteVertexArrays(1, &vao);
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

void PolyReductor::Renderer::ModelForReduction::prepareVertices()
{
	for (auto v : vertices)
	{
		positionSForDrawing[v->id] = v->position;
	}
}

void PolyReductor::Renderer::ModelForReduction::prepareNormals()
{
	for (int i = 0; i < vertices.size(); ++i)
		normalsForDrawing[i] = glm::vec3(0.0f);
	for (auto t : triangles)
	{
		for (int i = 0; i < 3;++i)
			normalsForDrawing[t->vertex[i]->id] += t->normal;
	}
	for (int i = 0; i < vertices.size(); ++i)
		normalsForDrawing[i] = glm::normalize(normalsForDrawing[i]);
}

void PolyReductor::Renderer::ModelForReduction::prepareIndices()
{
	for (int i = 0; i < triangles.size(); ++i)
	{
		for (int j = 0; j < 3;++j)
			indicesForDrawing[3 * i + j] = triangles[i]->vertex[j]->id;
	}
}

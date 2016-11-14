#include "ModelForReduction.h"
#include <assimp\postprocess.h>
#include <algorithm>
#include <iostream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\fast_square_root.hpp>
#include <opencv2\opencv.hpp>

PolyReductor::Renderer::ModelForReduction::Vertex::Vertex(const glm::vec3 & pos, int id):
	position(pos),
	id(id)
{
}

PolyReductor::Renderer::ModelForReduction::Vertex::~Vertex()
{
}

void PolyReductor::Renderer::ModelForReduction::Vertex::RemoveIfNonNeighbor()
{
	/*for (auto n : candidate->neighbor)
	{
		auto it = std::find_if(neighbor.begin(), neighbor.end(), [&](std::shared_ptr<Vertex> v) { return v->id == n->id; });
		if (it == neighbor.end() && n->id != id)
		{
			neighbor.push_back(n);
		}
	}
	auto candi = std::find_if(neighbor.begin(), neighbor.end(), [&](std::shared_ptr<Vertex> v) { return v->id == candidate->id; });
	neighbor.erase(candi);*/
}

void PolyReductor::Renderer::ModelForReduction::Vertex::calculateNormal()
{
	normal = glm::vec3(0.0f);
	/*for (const auto f : face)
	{
		normal += f->normal;
	}*/

	for (int i = 0; i < face.getSize(); ++i)
	{
		normal += face[i]->normal;
	}
	normal = glm::fastNormalize(normal);
}

void PolyReductor::Renderer::ModelForReduction::Vertex::calculateCostForDelete()
{
	cost = 0.0f;

	int N{ 0 };
	/*for (auto n : neighbor)
	{
		if (n != nullptr)
		{
			cost += glm::dot(normal, n->normal);
			N++;
		}
	}*/

	for (int i = 0; i < neighbor.getSize(); ++i)
	{
		auto n = neighbor[i];
		if (n != nullptr)
		{
			cost += glm::dot(normal, n->normal);
			++N;
		}
	}

	if (N > 0)
	{
		cost /= N;
	}

	float candidateCost{ 0.0f };
	candidate = nullptr;

	/*for (const auto& n:neighbor)
	{
		if (n != nullptr)
		{
			float c = (glm::dot(normal, n->normal))/ glm::distance(position, n->position);
			if (c > candidateCost)
			{
				candidateCost = c;
				candidate = n;
			}
		}
	}*/

	for (int i = 0; i < neighbor.getSize(); ++i)
	{
		auto n = neighbor[i];
		if (n != nullptr)
		{
			float c = (glm::dot(normal, n->normal)) / glm::distance(position, n->position);
			if (c > candidateCost)
			{
				candidateCost = c;
				candidate = n;
			}
		}
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
	{
		if (vertex[i]->id == vold->id)
		{
			vertex[i] = vnew;
			return;
		}
	}
}

bool PolyReductor::Renderer::ModelForReduction::Triangle::hasVertex(std::shared_ptr<Vertex> v)
{
	for (int i = 0; i < 3; ++i)
		if (vertex[i]->id == v->id)
			return true;
	return false;
}

PolyReductor::Renderer::ModelForReduction::ModelForReduction()
{
	firstVertex = true;
	forRemove = nullptr;
	prevForRemove = nullptr;
}

PolyReductor::Renderer::ModelForReduction::ModelForReduction(const std::string & modelName)
{
	loadModel(modelName);
	firstVertex = true;
	forRemove = nullptr;
	prevForRemove = nullptr;
}

void PolyReductor::Renderer::ModelForReduction::loadModel(const std::string & modelName)
{
	//we accept scene with only one model
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(modelName, aiProcess_Triangulate| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	const aiMesh* mesh = scene->mMeshes[0];

	std::cout <<"Vertices count "<< mesh->mNumVertices << "\n";

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
		vertices[v0]->face.pushBack(triangles[i]);
		vertices[v1]->face.pushBack(triangles[i]);
		vertices[v2]->face.pushBack(triangles[i]);
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

void PolyReductor::Renderer::ModelForReduction::reduce(int numberOfVertices)
{
	while (vertices.size() > numberOfVertices)
	{
		removeVertex();
	}
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

void PolyReductor::Renderer::ModelForReduction::createHistograms(GLuint shaderProgram, std::shared_ptr<MyEngine::Renderer::Transform> transform, const glm::mat4 & V, const glm::mat4 & P,int width,int height)
{
	/*
	Create histograms from buffer of showed model.
	Model are rotating to make image from all direction.
	*/
	std::vector<glm::vec3> rotates;
	for (int i = 0; i < 8; ++i)
	{
		rotates.push_back(glm::vec3(0.0f, 45.0f*i, 0.0f));
	}
	for (int i = 0; i < 4; ++i)
	{
		rotates.push_back(glm::vec3(45.0f, 90.0f*i + 45.0f, 0.0f));
		rotates.push_back(glm::vec3(-45.0f, 90.0f*i + 45.0f, 0.0f));
	}
	glUseProgram(shaderProgram);

	std::unique_ptr<unsigned char[]> buffer(new unsigned char[3 * width*height]);
	for (int i = 0; i < rotates.size(); ++i)
	{
		std::cout << "Prepare " << i << " histogram\n";
		transform->setRotate(rotates[i]);
		transform->update();
		glm::mat4 m = transform->getModelMatrix();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw(shaderProgram, m, V, P);
		glReadBuffer(GL_BACK_LEFT);
		glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, buffer.get());//read buffer
		cv::Mat image(height, width, CV_8UC3, buffer.get());//crete image from buffer
		/*cv::Mat hsvImage;
		cv::cvtColor(image, hsvImage, CV_BGR2HSV);//convert rgb to hsv
		int h_bins = 50;
		int s_bins = 60;
		int histSise[] = { h_bins,s_bins };
		float h_ranges[] = { 1,180 };
		float s_ranges[] = { 1,256 };
		const float* ranges[] = { h_ranges,s_ranges };
		int channels[] = { 0,1 };

		cv::MatND hist;
		cv::calcHist(&hsvImage, 1, channels, cv::Mat(), hist, 2, histSise, ranges, true, false);
		
		cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
		histograms.histograms[i] = hist;*/


		std::vector<cv::Mat> bgr_planes;
		cv::split(image, bgr_planes);
		float range[] = { 20, 256 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		cv::Mat b_hist, g_hist, r_hist;
		int histSize = 236;

		/// Compute the histograms:
		cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// Draw the histograms for B, G and R
		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

		/// Normalize the result to [ 0, histImage.rows ]
		cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv:: Mat());

		histograms.histograms[i] = r_hist;

		//cv::namedWindow("name");
		//cv::imshow("name", image);
		//cv::waitKey(1000);
		
		/*for (int r = 0; r < image.rows; ++r)
		{
			for (int c = 0; c < image.cols; ++c)
			{
				cv::Vec3b temp = image.at<cv::Vec3b>(r, c);
				histograms.histograms[i][temp[0]].r++;
				histograms.histograms[i][temp[1]].g++;
				histograms.histograms[i][temp[2]].b++;
			}
		}*/
	}
	/*for (int i = 0; i < 255; ++i)
	{
		std::cout << histograms.histograms[0][i].r << " " << histograms.histograms[0][i].g << " " << histograms.histograms[0][i].b << "\n";
	}*/
	//std::cout << cv::compareHist(histograms.histograms[0], histograms.histograms[0], 0);
	transform->setRotate(glm::vec3(0.0f));
}

Histograms PolyReductor::Renderer::ModelForReduction::getHistograms()
{
	return histograms;
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
	/*auto it = std::find_if(v->neighbor.begin(), v->neighbor.end(), [&](std::shared_ptr<Vertex> vert) {return vert->id == neighbor->id; });
	if (it == std::end(v->neighbor))
		v->neighbor.push_back(neighbor);*/

	if (v->neighbor.findElement([&](std::shared_ptr<Vertex> v1) { return v1->id == neighbor->id; }) == -1)
	{
		v->neighbor.pushBack(neighbor);
	}
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
		normalsForDrawing[i] = glm::fastNormalize(normalsForDrawing[i]);
}

void PolyReductor::Renderer::ModelForReduction::prepareIndices()
{
	for (int i = 0; i < triangles.size(); ++i)
	{
		for (int j = 0; j < 3;++j)
			indicesForDrawing[3 * i + j] = triangles[i]->vertex[j]->id;
	}
}

void PolyReductor::Renderer::ModelForReduction::calculateCostForVertices()
{
	//if (firstVertex)
	//{
		for (auto v : vertices)
		{
			v->calculateNormal();
		}

		for (auto v : vertices)
		{
			v->calculateCostForDelete();
		}
	/*}
	else
	{
		for (int i = 0; i < forRemove->candidate->neighbor.getSize(); ++i)
		{
			forRemove->candidate->neighbor[i]->calculateNormal();
		}
		forRemove->candidate->calculateNormal();
		for (int i = 0; i < forRemove->candidate->neighbor.getSize(); ++i)
		{
			forRemove->candidate->neighbor[i]->calculateCostForDelete();
		}
		forRemove->calculateCostForDelete();
	}*/
}

void PolyReductor::Renderer::ModelForReduction::findCandidate()
{
	//float cost{ 0.0f };
	//if (firstVertex)
	//{
		searchOnAllVerices();
		//firstVertex = false;
	/*}
	else
	{
		//find locally
		if (prevForRemove == nullptr)
		{
			searchOnAllVerices();
		}
		else
		{
			searchOnLocalVerices();
		}
	}*/
}

void PolyReductor::Renderer::ModelForReduction::removeVertex()
{
	std::cout << "Remove " << vertices.size() << "\n";
	calculateCostForVertices();
	findCandidate();
	//Vertex is deleting from list of all vertices
	/*auto remVer = std::find_if(std::begin(vertices), std::end(vertices), [&](std::shared_ptr<Vertex> v) { return v->id == forRemove->id; });
	if (remVer != std::end(vertices))
	{
		vertices.erase(remVer);
	}*/

	//std::cout << "Before remove\n";
	vertices.erase(vertices.begin() + forRemove->id);
	//std::cout << "After Remove\n";

	//Neighbors of erasing vetex contain this vertex. Must be deleting from nighbor list of all neighbor.?????
	/*for (auto n : forRemove->neighbor)
	{

		auto remNeigbor = std::find_if(std::begin(n->neighbor), std::end(n->neighbor), [&](std::shared_ptr<Vertex> v) {return v->id == forRemove->id; });
		if (remNeigbor != std::end(n->neighbor))
		{
			//if(n->id == forRemove->candidate->id)
				//std::cout << "Would erase\n";
			n->neighbor.erase(remNeigbor);
		}
	}*/

	//std::cout << "Before erase neighbor\n";
	for (int i = 0; i < forRemove->neighbor.getSize(); ++i)
	{
		forRemove->neighbor[i]->neighbor.eraseElement([&](std::shared_ptr<Vertex> v1) { return v1->id == forRemove->id; });
	}
	//std::cout << "After erase neighbor\n";

	/*auto remNeigbor = std::find_if(std::begin(forRemove->candidate->neighbor), std::end(forRemove->candidate->neighbor), [&](std::shared_ptr<Vertex> v) {return v->id == forRemove->id; });
	if (remNeigbor != std::end(forRemove->candidate->neighbor))
	{
		//if(n->id == forRemove->candidate->id)
		//std::cout << "Would erase\n";
		forRemove->candidate->neighbor.erase(remNeigbor);
	}*/

	//Erase triangles which contain of erasing vertex
	while (true)
	{
		auto remTri = std::find_if(std::begin(triangles), std::end(triangles), [&](std::shared_ptr<Triangle> t) { return (t->hasVertex(forRemove) && t->hasVertex(forRemove->candidate)); });
		if (remTri == std::end(triangles))
		{
			break;
		}
		else
		{
			triangles.erase(remTri);
		}
	}

	//Localy erase triangles for all neighbor of deleting vertex
	/*for (auto n : forRemove->neighbor)
	{
		while (true)
		{
			auto remTriangleNeighbor = std::find_if(std::begin(n->face), std::end(n->face), [&](std::shared_ptr<Triangle> t) { return (t->hasVertex(forRemove) && t->hasVertex(forRemove->candidate)); });
			if (remTriangleNeighbor == std::end(n->face))
			{
				break;
			}
			else
			{
				n->face.erase(remTriangleNeighbor);
			}
		}
	}*/

	for (int i = 0; i < forRemove->neighbor.getSize(); ++i)
	{
		auto n = forRemove->neighbor[i];
		n->face.eraseElement([&](std::shared_ptr<Triangle> t) { return (t->hasVertex(forRemove) && t->hasVertex(forRemove->candidate)); });
	}

	/*while (true)
	{
		auto remTriangleNeighbor = std::find_if(std::begin(forRemove->candidate->face), std::end(forRemove->candidate->face), [&](std::shared_ptr<Triangle> t) { return (t->hasVertex(forRemove) && t->hasVertex(forRemove->candidate)); });
		if (remTriangleNeighbor == std::end(forRemove->candidate->face))
		{
			break;
		}
		else
		{
			forRemove->candidate->face.erase(remTriangleNeighbor);
		}
	}*/

	//Candidate get neighbors of removed vertex
	//std::vector<std::shared_ptr<Vertex>> tempVertex;
	/*for (auto n : forRemove->neighbor)
	{
		auto addNeig = std::find_if(std::begin(forRemove->candidate->neighbor), std::end(forRemove->candidate->neighbor), [&](std::shared_ptr<Vertex> v) { return v->id == n->id; });
		if (addNeig == std::end(forRemove->candidate->neighbor) && n->id != forRemove->id)
		{
			//forRemove->candidate->neighbor.push_back(n); //<---- problem with increment iterator, but i need this.... please....
			//tempVertex.push_back(n);
		}
	}*/


	///why after this my model is containing holes!!!????
	/*for (int i = 0; i < forRemove->neighbor.getSize(); ++i)
	{
		auto n = forRemove->neighbor[i];
		if (forRemove->candidate->neighbor.findElement([&](std::shared_ptr<Vertex> v1) { return v1->id == n->id; }) == -1 && n->id!=forRemove->id)
		{
			forRemove->candidate->neighbor.pushBack(n);
		}
	}*/

	while (true)
	{
		auto replVer = std::find_if(std::begin(triangles), std::end(triangles), [&](std::shared_ptr<Triangle> t) { return t->hasVertex(forRemove); });
		if (replVer == std::end(triangles))
		{
			break;
		}
		else
		{
			(*replVer)->replaceVertex(forRemove, forRemove->candidate);
		}
	}
	
	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i]->id = i;
	}

	
}

void PolyReductor::Renderer::ModelForReduction::searchOnAllVerices()
{
	cost = 0.0f;
	for (auto v : vertices)
	{
		if (v->cost > cost)
		{
			//std::cout << "Found. what is problem?\n";
			cost = v->cost;
			prevForRemove = forRemove;
			forRemove = v;
		}
	}
}

void PolyReductor::Renderer::ModelForReduction::searchOnLocalVerices()
{
	std::shared_ptr<Vertex> tempForRemove = std::move(forRemove);
	forRemove = prevForRemove;
	cost = forRemove->cost;
	prevForRemove = nullptr;
	for (int i = 0; i < tempForRemove->candidate->neighbor.getSize(); ++i)
	{
		std::shared_ptr<Vertex> v = tempForRemove->candidate->neighbor[i];
		if (v->cost > cost)
		{
			cost = v->cost;
			prevForRemove = forRemove;
			forRemove = v;
		}
	}
	if (tempForRemove->candidate->cost > cost)
	{
		cost = tempForRemove->candidate->cost;
		prevForRemove = forRemove;
		forRemove = tempForRemove->candidate;
	}
}

#include "lab4.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace vdvac;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

lab4::lab4()
{
}

lab4::~lab4()
{
}

Mesh* lab4::createQuad(const char* name)
{
	//creeaza un quad mare cat NDC
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec2(0,1)),
		VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec2(1,1)),
		VertexFormat(glm::vec3(1, 1, -1), glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec2(1,0)),
		VertexFormat(glm::vec3(-1, 1, -1), glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec2(0,0)),
	};
	vector<unsigned int> indices =
	{
		0, 1, 2,
		0, 2, 3,
	};

	meshes[name] = new Mesh(name);
	meshes[name]->InitFromData(vertices, indices);
	return meshes[name];
}


void lab4::createNet(const string& path, const char* fileName)
{
	int control_points_no;
	int triangles_no;
	string word;
	std::string::size_type word_size;
	int vertex_id;
	glm::vec3 v1, v2;
	vector<VertexFormat> vertices1, vertices2;
	vector<unsigned int> indices;
	ifstream f;

	f.open((path + (fileName ? (string("/") + fileName) : "")).c_str());
	f >> control_points_no;

	//citire perechile de puncte de control din cele doua retele
	for (int i = 0; i < control_points_no; i++)
	{
		f >> word;
		v1.x = stof(word, &word_size);
		f >> word;
		v1.y = stof(word, &word_size);
		v1.z = 0;
		f >> word;
		f >> word;
		v2.x = stof(word, &word_size);
		f >> word;
		v2.y = stof(word, &word_size);
		v2.z = 0;

		vertices1.push_back(VertexFormat(v1));
		vertices2.push_back(VertexFormat(v2));
	}
	f >> triangles_no;

	//citire triunghiurile formate
	for (int i = 0; i < triangles_no * 3; i++)
	{
		f >> vertex_id;
		indices.push_back(vertex_id);
	}

	//creare retele de triunghiuri
	sourceNet->InitFromData(vertices1, indices);
	destNet->InitFromData(vertices2, indices);
	f.close();
}



void lab4::Init()
{
	iterations = 10;
	counter = 0;

	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "lab4", "shaders");
	{
		Shader* shader = new Shader("MainShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Mesh* mesh = createQuad("quad");
		mesh->UseMaterials(false);
	}

	sourceImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "ImageAnimation/ex1/img1.png"), nullptr, "sourceImage", true, true);
	interImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "ImageAnimation/ex1/img1.png"), nullptr, "interImage", true, true);
	destImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "ImageAnimation/ex1/img2.png"), nullptr, "destImage", true, true);

	sourceNet = new Mesh("sourceNet");
	destNet = new Mesh("destNet");
	createNet(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "ImageAnimation/ex1/net.txt"), nullptr);

	interNet = new Mesh("interNet");
	interNet->InitFromData(sourceNet->vertices, sourceNet->indices);
}

float Area(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2)
{
	return abs((v0.x * (v1.y - v2.y) - v1.x * (v0.y - v2.y) + v2.x * (v0.y - v1.y)) / 2);
}

//intoarce true daca pozitia "pos" e in interiorul triunghiului v0v1v2 
//si calculeaza coordonatele baricentrice
bool Baricentric(glm::vec2 pos, glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, float& u, float& v, float& w)
{
	float A_u = 0, A_v = 0, A_w = 0, A = 0;
	//TODO1 
	//calculeaza ariile 
	A_u = Area(v1, v2, pos);
	A_v = Area(v0, v2, pos);
	A_w = Area(v1, v0, pos);
	A = Area(v0, v1, v2);

	if (A_u + A_v + A_w > A + 0.0001 && A > 0.00001)
		return false;
	{
		//TODO1 calculeaza u,v,w;
		u = 0;
		v = 0;
		w = 0;
		u = A_u / A;
		v = A_v / A;
		w = A_w / A;
		return true;
	}
}

void lab4::ModifyImage(int crt_iteration, int iterations)
{
	vector<VertexFormat> vertices = interNet->vertices;
	vector<unsigned int> indices = interNet->indices;

	float ti = (float)crt_iteration / iterations;
	for (int i = 0; i < interNet->vertices.size(); i++)
	{
		//TODO2
		// calculeaza varfurile retelei intermediare prin interpolare
		//slide 12
		vertices[i].position = sourceNet->vertices[i].position * (1 - ti) + destNet->vertices[i].position * ti;
	}

	unsigned int channels = sourceImage->GetNrChannels();
	unsigned char* sourceData = sourceImage->GetImageData();
	unsigned char* destData = destImage->GetImageData();
	unsigned char* interData = interImage->GetImageData();


	if (channels < 3)
		return;

	glm::ivec2 imageSize = glm::ivec2(sourceImage->GetWidth(), sourceImage->GetHeight());
	int offset, offsetSource, offsetDest;
	glm::vec2 v0, v1, v2, vsource0, vsource1, vsource2, vdest0, vdest1, vdest2;
	glm::vec2 pos, posSource, posDest;
	float u, v, w;
	int isource, jsource;
	int idest, jdest;


	for (int j = 0; j < imageSize.y; j++)
	{
		for (int i = 0; i < imageSize.x; i++)
		{
			offset = channels * (j * imageSize.x + i);
			pos.y = (float)(imageSize.y - j - 1) / imageSize.y;
			pos.x = (float)i / imageSize.x;

			for (int k = 0; k < indices.size(); k += 3)
			{
				//TODO3
				//calculeaza v0, v1 and v2,
				//care sunt varfurile din triunghiul curent in reteaua intermediara
				//Atentie: indicii varfurilor din retele sunt pastrate in tabloul indices[]
				//         iar k itereaza prin tabloudl indices[], nu prin tabloul vertices[] !!

				v0 = glm::vec2(0);
				v1 = glm::vec2(0);
				v2 = glm::vec2(0);
				v0 = vertices[indices[k]].position;
				v1 = vertices[indices[k + 1]].position;
				v2 = vertices[indices[k + 2]].position;

				if (Baricentric(pos, v0, v1, v2, u, v, w))
				{

					//TODO4
					//calculeaza vsource0, vsource1 and vsource2,
					//care sunt varfurile din triunghiul curent in reteaua sursa


					//calculeaza vdest0, vdest1 and vdest2,
					//care sunt varfurile din triunghiul curent in reteaua destinatie


					//are acelasi vector de indecsi, dar e in vectorul de varfuri din imaginea sursa
					vsource0 = glm::vec2(0);
					vsource1 = glm::vec2(0);
					vsource2 = glm::vec2(0);
					vsource0 = sourceNet->vertices[sourceNet->indices[k]].position;
					vsource1 = sourceNet->vertices[sourceNet->indices[k + 1]].position;
					vsource2 = sourceNet->vertices[sourceNet->indices[k + 2]].position;

					vdest0 = glm::vec2(0);
					vdest1 = glm::vec2(0);
					vdest2 = glm::vec2(0);
					vdest0 = destNet->vertices[destNet->indices[k]].position;
					vdest1 = destNet->vertices[destNet->indices[k + 1]].position;
					vdest2 = destNet->vertices[destNet->indices[k + 2]].position;

					//TODO5
					//calculeaza posSource, corespondentul lui pos in reteaua sursa
					//calculeaza posDest, corespondentul lui pos in reteaua destinatie
					//vezi slide 23 -> formulelele de acolo
					posSource = glm::vec2(0);
					posDest = glm::vec2(0);
					posSource = u * vsource0 + v * vsource1 + w * vsource2;
					posDest = u * vdest0 + v * vdest1 + w * vdest2;

					//se scaleaza la rezolutia imaginii
					isource = posSource.x * imageSize.x + 0.5;
					jsource = imageSize.y - (posSource.y * imageSize.y + 0.5) - 1;

					idest = posDest.x * imageSize.x + 0.5;
					jdest = imageSize.y - (posDest.y * imageSize.y + 0.5) - 1;

					if (isource < 0) isource = 0;
					if (jsource < 0) jsource = 0;
					if (idest < 0) idest = 0;
					if (jdest < 0) jdest = 0;
					if (isource >= imageSize.x) isource = imageSize.x - 1;
					if (jsource >= imageSize.y) jsource = imageSize.y - 1;
					if (idest >= imageSize.x) idest = imageSize.x - 1;
					if (jdest >= imageSize.y) jdest = imageSize.y - 1;

					offsetSource = channels * (jsource * imageSize.x + isource);
					offsetDest = channels * (jdest * imageSize.x + idest);

					//TODO6
					//calculeaza culoarea pentru pixelul curent in imaginea intermediara (prin interpolare)
					//Atentie: canalele RGB se obtin prin offset, offset+1, offset+2
					//         offset imi da pozitia in tabloul interData
					//         offsetSource imi da pozitia in tabloul sourceData
					//         offsetDest imi da pozitia in tabloul destData

					interData[offset] = sourceData[offsetSource] * (1 - ti) + destData[offsetDest] * (ti);
					interData[offset + 1] = sourceData[offsetSource + 1] * (1 - ti) + destData[offsetDest + 1] * (ti);
					interData[offset + 2] = sourceData[offsetSource + 2] * (1 - ti) + destData[offsetDest + 2] * (ti);

					break;
				}

			}
		}
	}

	interImage->UploadNewData(interData);
	interNet->InitFromData(vertices, indices);
}

void lab4::ModifyImage2(int crt_iteration, int iterations)
{
	vector<VertexFormat> vertices = interNet->vertices;
	vector<unsigned int> indices = interNet->indices;

	float ti = (float)crt_iteration / iterations;
	for (int i = 0; i < interNet->vertices.size(); i++)
	{
		//TODO2
		// calculeaza varfurile retelei intermediare prin interpolare
		//slide 12
		vertices[i].position = sourceNet->vertices[i].position * (1 - ti) + destNet->vertices[i].position * ti;
	}

	unsigned int channels = sourceImage->GetNrChannels();
	unsigned char* sourceData = sourceImage->GetImageData();
	unsigned char* destData = destImage->GetImageData();
	unsigned char* interData = interImage->GetImageData();


	if (channels < 3)
		return;

	glm::ivec2 imageSize = glm::ivec2(sourceImage->GetWidth(), sourceImage->GetHeight());
	int offset, offsetSource, offsetDest;
	glm::vec2 v0, v1, v2, vsource0, vsource1, vsource2, vdest0, vdest1, vdest2;
	glm::vec2 pos, posSource, posDest;
	float u, v, w;
	int isource, jsource;
	int idest, jdest;

	//patratul in care facem rasterizare
	float yMin1 = 0, yMax1 = 0, xMin1 = 0, xMax1 = 0;
	int yMin = 0, yMax = 0, xMin = 0, xMax = 0;

	//rasterizam fiecare triunghiu
	for (int k = 0; k < indices.size(); k += 3) {

		//varfurile triungiului curent in reteaua intermediara
		v0 = vertices[indices[k]].position;
		v1 = vertices[indices[k + 1]].position;
		v2 = vertices[indices[k + 2]].position;

		//varfurile patratului in care cautam pixelilor triunghiului pt rasterizare
		xMin1 = glm::min(v0.x, glm::min(v1.x, v2.x));
		xMax1 = glm::max(v0.x, glm::max(v1.x, v2.x));
		yMin1 = glm::min(v0.y, glm::min(v1.y, v2.y));
		yMax1 = glm::max(v0.y, glm::max(v1.y, v2.y));

		//la rezolutia imaginii
		xMin = xMin1 * imageSize.x;
		xMax = xMax1 * imageSize.x;
		yMin = imageSize.y - (yMax1 * imageSize.y + 0.5);
		yMax = imageSize.y - (yMin1 * imageSize.y + 0.5);

		if (yMin < 0) yMin = 0;
		if (yMax < 0) yMax = 0;
		if (yMin > imageSize.y) yMin = imageSize.y - 1;
		if (yMax > imageSize.y) yMax = imageSize.y - 1;


		vsource0 = sourceNet->vertices[sourceNet->indices[k]].position;
		vsource1 = sourceNet->vertices[sourceNet->indices[k + 1]].position;
		vsource2 = sourceNet->vertices[sourceNet->indices[k + 2]].position;

		vdest0 = destNet->vertices[destNet->indices[k]].position;
		vdest1 = destNet->vertices[destNet->indices[k + 1]].position;
		vdest2 = destNet->vertices[destNet->indices[k + 2]].position;

		for (int j = yMin; j < yMax; j++) {
			for (int i = xMin; i < xMax; i++) {

				offset = channels * (j * imageSize.x + i);
				pos.y = (float)(imageSize.y - j - 1) / imageSize.y;
				pos.x = (float)i / imageSize.x;

				if (Baricentric(pos, v0, v1, v2, u, v, w))
				{
					posSource = u * vsource0 + v * vsource1 + w * vsource2;
					posDest = u * vdest0 + v * vdest1 + w * vdest2;

					//se scaleaza la rezolutia imaginii
					isource = posSource.x * imageSize.x + 0.5;
					jsource = imageSize.y - (posSource.y * imageSize.y + 0.5) - 1;

					idest = posDest.x * imageSize.x + 0.5;
					jdest = imageSize.y - (posDest.y * imageSize.y + 0.5) - 1;

					if (isource < 0) isource = 0;
					if (jsource < 0) jsource = 0;
					if (idest < 0) idest = 0;
					if (jdest < 0) jdest = 0;
					if (isource >= imageSize.x) isource = imageSize.x - 1;
					if (jsource >= imageSize.y) jsource = imageSize.y - 1;
					if (idest >= imageSize.x) idest = imageSize.x - 1;
					if (jdest >= imageSize.y) jdest = imageSize.y - 1;

					offsetSource = channels * (jsource * imageSize.x + isource);
					offsetDest = channels * (jdest * imageSize.x + idest);

					interData[offset] = sourceData[offsetSource] * (1 - ti) + destData[offsetDest] * (ti);
					interData[offset + 1] = sourceData[offsetSource + 1] * (1 - ti) + destData[offsetDest + 1] * (ti);
					interData[offset + 2] = sourceData[offsetSource + 2] * (1 - ti) + destData[offsetDest + 2] * (ti);
				}
			}
		}
	}

	interImage->UploadNewData(interData);
	interNet->InitFromData(vertices, indices);
}


void lab4::FrameStart()
{

}


void lab4::Update(float deltaTimeSeconds)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	auto resolution = window->GetResolution();
	glViewport(0, 0, resolution.x / 3, resolution.y);


	auto shader = shaders["MainShader"];
	int locTexture = shader->GetUniformLocation("textura1");
	int locImage = shader->GetUniformLocation("is_image");

	//desenare imaginea sursa
	glUniform1i(locTexture, 0);
	sourceImage->BindToTextureUnit(GL_TEXTURE0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform1i(locImage, 1);
	RenderMesh(meshes["quad"], shader, glm::scale(glm::mat4(1), glm::vec3(1, 1, 1)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniform1i(locImage, 0);
	RenderMesh(sourceNet, shader, glm::scale(glm::mat4(1), glm::vec3(1, 1, 1)));


	//desenare imaginea intermediara
	glViewport(resolution.x / 3, 0, resolution.x / 3, resolution.y);
	glUniform1i(locTexture, 0);
	interImage->BindToTextureUnit(GL_TEXTURE0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform1i(locImage, 1);
	RenderMesh(meshes["quad"], shader, glm::scale(glm::mat4(1), glm::vec3(1, 1, 1)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniform1i(locImage, 0);
	RenderMesh(interNet, shader, glm::scale(glm::mat4(1), glm::vec3(1, 1, 1)));

	//desenare imaginea destinatie
	glViewport(resolution.x * 2 / 3, 0, resolution.x / 3, resolution.y);
	glUniform1i(locTexture, 0);
	destImage->BindToTextureUnit(GL_TEXTURE0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform1i(locImage, 1);
	RenderMesh(meshes["quad"], shader, glm::scale(glm::mat4(1), glm::vec3(1, 1, 1)));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniform1i(locImage, 0);
	RenderMesh(destNet, shader, glm::scale(glm::mat4(1), glm::vec3(1, 1, 1)));
}




void lab4::FrameEnd()
{

}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void lab4::OnInputUpdate(float deltaTime, int mods)
{

};

void lab4::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_A)
	{

		if (counter < iterations)
		{
			counter++;
			ModifyImage2(counter, iterations);
		}
	}
	else
		if (key == GLFW_KEY_Z)
		{

			if (counter > 0)
			{
				counter--;
				ModifyImage2(counter, iterations);
			}
		}


};

void lab4::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void lab4::OnWindowResize(int width, int height)
{
	// treat window resize event
}

#include "Tema1.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace vdvac;


Tema1::Tema1()
{
}

Tema1::~Tema1()
{
	//distruge obiecte
	if (volumeData) delete[] volumeData;
}

bool Tema1::loadRAWFile(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z)
{
	FILE* File = NULL;
	cout << fileLocation.c_str() << endl;
	if (!fileLocation.c_str())
	{
		return false;
	}

	fopen_s(&File, fileLocation.c_str(), "rb");

	if (!File)
		return false;

	xsize = x;
	ysize = y;
	zsize = z;
	volumeData = new unsigned char[xsize * ysize * zsize];

	int sliceSize = xsize * ysize;

	for (int slice = 0; slice < zsize; slice++)
	{
		fread(&volumeData[slice * ysize * xsize], sizeof(unsigned char), sliceSize, File);
	}

	fclose(File);
	return true;
}

Mesh* Tema1::createCube(const char* name)
{
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), // 0  -> 0 
		VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)), // 3	-> 1
		VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)), // 4	-> 2
		VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0, 1, 1)), // 7	-> 3
		VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // 1	-> 4
		VertexFormat(glm::vec3(1, 0, 1), glm::vec3(1, 0, 1)), // 2	-> 5
		VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1, 1, 0)), // 5 	-> 6
		VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1))  // 6	-> 7
	};
	vector<unsigned int> indices =
	{
		1, 5, 7,
		7, 3, 1,
		0, 2, 6,
		6, 4, 0,
		0, 1, 3,
		3, 2, 0,
		7, 5, 4,
		4, 6, 7,
		2, 3, 7,
		7, 6, 2,
		1, 0, 4,
		4, 5, 1
	};

	meshes[name] = new Mesh(name);
	meshes[name]->InitFromData(vertices, indices);
	return meshes[name];
}
Mesh* Tema1::createCubePerimeter(const char* name)
{
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), // 0  -> 0 
		VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)), // 3	-> 1
		VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)), // 4	-> 2
		VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0, 1, 1)), // 7	-> 3
		VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // 1	-> 4
		VertexFormat(glm::vec3(1, 0, 1), glm::vec3(1, 0, 1)), // 2	-> 5
		VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1, 1, 0)), // 5 	-> 6
		VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1))  // 6	-> 7
	};
	vector<unsigned int> indices =
	{
		0,4,
		1,5,
		0,1,
		4,5,
		2,6,
		3,7,
		2,3,
		6,7,
		3,1,
		7,5,
		4,6,
		0,2
	};

	meshes[name] = new Mesh(name);
	meshes[name]->InitFromData(vertices, indices);
	return meshes[name];
}

GLuint Tema1::createVolumeTexture(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z) {

	loadRAWFile(fileLocation, x, y, z);

	GLuint g_volTexObj;
	glGenTextures(1, &g_volTexObj);

	glBindTexture(GL_TEXTURE_3D, g_volTexObj);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, xsize, ysize, zsize, 0, GL_RED, GL_UNSIGNED_BYTE, volumeData);
	CheckOpenGLError();
	cout << "Volume texture created" << endl;
	return g_volTexObj;

}

GLuint Tema1::createTFTexture(const string& fileLocation) {
	//se citesc datele functiei de transfer definite de utilizator
	ifstream inFile(fileLocation.c_str(), ifstream::in);
	if (!inFile)
	{
		cerr << "Error openning file: " << fileLocation << endl;
		exit(EXIT_FAILURE);
	}

	const int MAX_CNT = 10000;
	GLubyte* tff = (GLubyte*)calloc(MAX_CNT, sizeof(GLubyte));
	inFile.read(reinterpret_cast<char*>(tff), MAX_CNT);
	if (inFile.eof())
	{
		size_t bytecnt = inFile.gcount();
		*(tff + bytecnt) = '\0';
		cout << "Functia de transfer: byte count = " << bytecnt << endl;

	}
	else if (inFile.fail())
	{
		cout << fileLocation << " nu s-a putut citi" << endl;
	}
	else
	{
		cout << fileLocation << " este prea mare" << endl;
	}
	GLuint tff1DTex;
	glGenTextures(1, &tff1DTex);
	glBindTexture(GL_TEXTURE_1D, tff1DTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, tff);
	free(tff);
	return tff1DTex;
}

float Tema1::CalculateMaxDistanceToCube(glm::vec3 worldPosition) {

	std::vector<glm::vec3> cubeCorners = {
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 1, 0),
		glm::vec3(1, 1, 1)
	};

	float distance = 0;
	float maxDistance = 0;
	for (auto it = cubeCorners.begin(); it != cubeCorners.end(); it++) {

		distance = glm::distance(worldPosition, *it);
		if (distance > maxDistance) {
			maxDistance = distance;
		}
	}

	return maxDistance;
}

void Tema1::DrawVolume() {
	//calculeaza vectorul de vizualizare
	auto camera = GetSceneCamera();
	glm::vec3 cameraPosition = camera->m_transform->GetWorldPosition();
	_viewVec = glm::normalize(camera->m_transform->GetLocalOZVector());

	//calculeaza distanta celui mai indepartat vertex
	//	de acolo vom incepe desenarea planelor
	_proxyDistance = CalculateMaxDistanceToCube(GetSceneCamera()->m_transform->GetWorldPosition());


	auto shader = shaders["GeoemtryShader"];
	shader->Use();

	GLint cameraPosUniform = shader->GetUniformLocation("cameraPos");
	GLint proxyPointPosUniform = shader->GetUniformLocation("proxyPointPos");

	//trimtie pozitia camerei (pentru a calcula vectorul de vizualizare)
	glUniform3fv(cameraPosUniform, 1, glm::value_ptr(cameraPosition));

	//trimite textura 3D cu datele din volum
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _volumeTexture);
	glUniform1i(glGetUniformLocation(shader->program, "VolumeTex"), 0);

	//se trimite textura 1D cu functia de transfer
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, _tfTexture);
	glUniform1i(glGetUniformLocation(shader->program, "TransferFunc"), 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 model_matrix = glm::mat4(1);
	glm::vec3 proxyPointPosition;
	float distance = _proxyDistance;
	for (int i = 0; i < 256; i++) {

		//trimite punctul unde vrei sa desenezi planul de intersectie
		proxyPointPosition = cameraPosition + -_viewVec * distance;
		glUniform3fv(proxyPointPosUniform, 1, glm::value_ptr(proxyPointPosition));

		RenderMesh(meshes["cube"], shader, model_matrix);

		distance -= _proxyPass;
	}
	glDisable(GL_BLEND);
}

void Tema1::DrawPlaneWithColors() {

	//calculeaza vectorul de vizualizare
	auto camera = GetSceneCamera();
	glm::vec3 cameraPosition = camera->m_transform->GetWorldPosition();
	_viewVec = glm::normalize(camera->m_transform->GetLocalOZVector());

	auto shader = shaders["GeoemtryColorShader"];
	shader->Use();

	GLint cameraPosUniform = shader->GetUniformLocation("cameraPos");
	GLint proxyPointPosUniform = shader->GetUniformLocation("proxyPointPos");

	//trimtie pozitia camerei (pentru a calcula vectorul de vizualizare)
	glUniform3fv(cameraPosUniform, 1, glm::value_ptr(cameraPosition));

	glm::mat4 model_matrix = glm::mat4(1);
	glm::vec3 proxyPointPosition;

	//trimite punctul unde vrei sa desenezi planul de intersectie
	proxyPointPosition = cameraPosition + -_viewVec * _proxyDistance;
	glUniform3fv(proxyPointPosUniform, 1, glm::value_ptr(proxyPointPosition));

	RenderMesh(meshes["cube"], shader, model_matrix);
}

void Tema1::DrawPlaneWithTextures() {
	//calculeaza vectorul de vizualizare
	auto camera = GetSceneCamera();
	glm::vec3 cameraPosition = camera->m_transform->GetWorldPosition();
	_viewVec = glm::normalize(camera->m_transform->GetLocalOZVector());

	auto shader = shaders["GeoemtryTextureColorShader"];
	shader->Use();

	GLint cameraPosUniform = shader->GetUniformLocation("cameraPos");
	GLint proxyPointPosUniform = shader->GetUniformLocation("proxyPointPos");

	//trimtie pozitia camerei (pentru a calcula vectorul de vizualizare)
	glUniform3fv(cameraPosUniform, 1, glm::value_ptr(cameraPosition));

	//trimite textura 3D cu datele din volum
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _volumeTexture);
	glUniform1i(glGetUniformLocation(shader->program, "VolumeTex"), 0);

	//se trimite textura 1D cu functia de transfer
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, _tfTexture);
	glUniform1i(glGetUniformLocation(shader->program, "TransferFunc"), 1);

	glm::mat4 model_matrix = glm::mat4(1);
	glm::vec3 proxyPointPosition;

	//trimite punctul unde vrei sa desenezi planul de intersectie
	proxyPointPosition = cameraPosition + -_viewVec * _proxyDistance;
	glUniform3fv(proxyPointPosUniform, 1, glm::value_ptr(proxyPointPosition));

	RenderMesh(meshes["cube"], shader, model_matrix);
}

void Tema1::Init()
{
	ToggleGroundPlane();
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(2, 2, 2), glm::quat(glm::vec3(-45 * TO_RADIANS, 45 * TO_RADIANS, 0)));
	camera->Update();

	

	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "Tema1", "shaders");
	{
		Shader* shader = new Shader("MainShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("GeoemtryShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "TextureFS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("GeoemtryColorShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "ColorFS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("GeoemtryTextureColorShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "TextureColorFS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	Mesh* cube = createCube("cube");

	Mesh* cubePerimeter = createCubePerimeter("cubePerimeter");
	cubePerimeter->SetDrawMode(GL_LINES);

	loadRAWFile(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES, "engine.raw"), 256, 256, 256);
	_volumeTexture = createVolumeTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES, "engine.raw"), 256, 256, 256);
	_tfTexture = createTFTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES, "tff.dat"));

	_proxyDistance = CalculateMaxDistanceToCube(GetSceneCamera()->m_transform->GetWorldPosition());
	_proxyPass = 2.0 / _proxyEsantionsCount;
}

void Tema1::FrameStart()
{

}

void Tema1::Update(float deltaTimeSeconds)
{
	ClearScreen();
	{
		//Draw a wired cube
		auto shader = shaders["MainShader"];
		shader->Use();

		glm::mat4 model_matrix = glm::mat4(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		RenderMesh(meshes["cubePerimeter"], shader, model_matrix);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	{
		//Deseneaza volumul
		switch (_mode)
		{
		case 1:
			DrawVolume();
			break;
		case 2:
			DrawPlaneWithColors();
			break;
		case 3:
			DrawPlaneWithTextures();
			break;
		default:
			break;
		}

	}

}

void Tema1::FrameEnd()
{
	DrawCoordinateSystem();

}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

};

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE) {
		if (_mode != 1) {

			_proxyDistance += 0.05;
		}
	}
	if (key == GLFW_KEY_X) {
		if (_mode != 1) {

			_proxyDistance -= 0.05;
			if (_proxyDistance < 0)
				_proxyDistance = 0;
		}
	}

	if (key == GLFW_KEY_1) {
		_mode = 1;
	}
	if (key == GLFW_KEY_2) {
		_mode = 2;
	}
	if (key == GLFW_KEY_3) {
		_mode = 3;
	}
};

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Tema1::OnWindowResize(int width, int height)
{
	// treat window resize event
}

#include "Tema1.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace vdvac;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

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
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)),
		VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(1, 0, 1), glm::vec3(1, 0, 1)),
		VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1))

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


void Tema1::Init()
{
	ToggleGroundPlane();
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(2, 2, 2), glm::quat(glm::vec3(-30 * TO_RADIANS, 45 * TO_RADIANS, 0)));
	camera->Update();

	loadRAWFile(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES, "engine.raw"), 256, 256, 256);


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
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	Mesh* cube = createCube("cube");

	_proxyDistance = 0.1f;
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
		RenderMesh(meshes["cube"], shader, model_matrix);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	{
		//Draw a polygon proxy
		auto camera = GetSceneCamera();
		glm::vec3 cameraPosition = camera->m_transform->GetWorldPosition();
		glm::vec3 viewVector = glm::normalize(camera->m_transform->GetLocalOZVector());

		_proxyDistance = 2.0f;
		glm::vec3 proxyPointPosition = cameraPosition + -viewVector * _proxyDistance ;

		auto shader = shaders["GeoemtryShader"];
		shader->Use();
		glm::mat4 model_matrix = glm::mat4(1);
		
		GLint cameraPosUniform = shader->GetUniformLocation("cameraPos");
		glUniform3fv(cameraPosUniform, 1, glm::value_ptr(cameraPosition));

		//proxyPointPosition = glm::vec3(0, 0, 0);
		//std::cout << proxyPointPosition << std::endl;
		GLint proxyPointPosUniform = shader->GetUniformLocation("proxyPointPos");
		glUniform3fv(proxyPointPosUniform, 1, glm::value_ptr(proxyPointPosition));

		RenderMesh(meshes["cube"], shader, model_matrix);
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

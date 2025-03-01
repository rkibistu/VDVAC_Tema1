#include "lab6.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace vdvac;
#define MAX_DIM 10000

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

lab6::lab6()
{
}

lab6::~lab6()
{

}


void lab6::Init()
{
	ToggleGroundPlane();
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(0, -0.5, 4), glm::quat(glm::vec3(5 * TO_RADIANS, 0, 0)));
	camera->Update();


	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "lab6", "shaders");
	{
		Shader* shader = new Shader("MainShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}


	{
		Mesh* mesh = new Mesh("girl_sleep1");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "\\characters\\girl", "girl_sleep.obj");
		meshes[mesh->GetMeshID()] = mesh;
		mesh->UseMaterials(false);

	}

	{
		Mesh* mesh = new Mesh("girl_surprise");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "\\characters\\girl", "girl_surprise.obj");
		meshes[mesh->GetMeshID()] = mesh;
		mesh->UseMaterials(false);
	}

	{
		Mesh* mesh = new Mesh("girl_annoyed");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "\\characters\\girl", "girl_annoyed.obj");
		meshes[mesh->GetMeshID()] = mesh;
		mesh->UseMaterials(false);
	}

	{
		Mesh* mesh = new Mesh("girl_sleep2");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "\\characters\\girl", "girl_sleep.obj");
		meshes[mesh->GetMeshID()] = mesh;
		mesh->UseMaterials(false);
	}

	{
		const string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters\\girl");
		Texture2D* texture1 = new Texture2D();
		texture1->Load2D(PATH_JOIN(sourceTextureDir, "girl_texture.bmp").c_str(), GL_REPEAT);
		mapTextures["girl_texture"] = texture1;
	}

	for (int i = 0; i < 4; i++) {
		bind(i);
	}

	timp = indice = 0;

	light_position = glm::vec3(10, 7, 25);
	material_shininess = 30;
	material_kd = 0.5;
	material_ks = 0.5;

}

void lab6::bind(int i) {

	Mesh* crt_mesh, * next_mesh;
	string girl, next_girl;
	switch (i)
	{
	case 0: girl = "girl_sleep1"; next_girl = "girl_surprise"; break;
	case 1: girl = "girl_surprise"; next_girl = "girl_annoyed"; break;
	case 2: girl = "girl_annoyed"; next_girl = "girl_sleep2"; break;
	case 3: girl = "girl_sleep2"; next_girl = "girl_sleep1"; break;
	default: girl = "girl_sleep1"; next_girl = "girl_surprise"; break;
	}

	crt_mesh = meshes[girl];
	next_mesh = meshes[next_girl];


	glBindVertexArray(crt_mesh->GetBuffers()->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, next_mesh->GetBuffers()->m_VBO[0]);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, next_mesh->GetBuffers()->m_VBO[1]);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, next_mesh->GetBuffers()->m_VBO[2]);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void lab6::FrameStart()
{
	timp++;
	if (timp == 250)
		indice++;

	indice %= 4;
	timp %= 250;

	switch (indice)
	{
	case 0: girl = "girl_sleep1"; break;
	case 1: girl = "girl_surprise"; break;
	case 2: girl = "girl_annoyed"; break;
	case 3: girl = "girl_sleep2"; break;
	default: girl = "girl_sleep1"; break;
	}
}

void lab6::Update(float deltaTimeSeconds)
{
	ClearScreen();

	auto shader = shaders["MainShader"];

	shader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mapTextures["girl_texture"]->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	glUniform1f(glGetUniformLocation(shader->program, "time"), timp);

	auto camera = GetSceneCamera();
	glm::vec3 eye_position = camera->m_transform->GetLocalPosition();

	glUniform3f(glGetUniformLocation(shader->program, "light_position"), light_position.x, light_position.y, light_position.z);
	glUniform3f(glGetUniformLocation(shader->program, "eye_position"), eye_position.x, eye_position.y, eye_position.z);
	glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), material_shininess);
	glUniform1f(glGetUniformLocation(shader->program, "material_kd"), material_kd);
	glUniform1f(glGetUniformLocation(shader->program, "material_ks"), material_ks);

	Mesh* mesh_obj = meshes[girl];
	RenderMesh(mesh_obj, shaders["MainShader"], glm::vec3(0, -7, 0), glm::vec3(1, 1, 1));


}



void lab6::FrameEnd()
{
	//DrawCoordinateSystem();
}

void lab6::OnInputUpdate(float deltaTime, int mods)
{

};

void lab6::OnKeyPress(int key, int mods)
{
	// add key press event
};

void lab6::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void lab6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void lab6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void lab6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void lab6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void lab6::OnWindowResize(int width, int height)
{
	// treat window resize event
}

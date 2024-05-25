#include "lab5.h"
#include "FFD.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace vdvac;
#define MAX_DIM 10000

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

lab5::lab5()
{
}

lab5::~lab5()
{

}


void lab5::Init()
{
	ToggleGroundPlane();
	//lumina & material
	light_position = glm::vec3(10, 7, 25);
	material_shininess = 30;
	material_kd = 0.5;
	material_ks = 0.5;

	frame = 0;

	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(2, 2, 2), glm::quat(glm::vec3(-30 * TO_RADIANS, 45 * TO_RADIANS, 0)));
	camera->Update();

	{
		Mesh* mesh = new Mesh("bunny");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
		mesh->UseMaterials(false);
		meshes[mesh->GetMeshID()] = mesh;

		positions = mesh->positions;
		normals = mesh->normals;
		indices = mesh->indices;
	}

	//se aduce mesh-ul cu pozitiile in intervalul [0,1]
	glm::vec3 pos;
	float minx = MAX_DIM, miny = MAX_DIM, minz = MAX_DIM, maxx = -MAX_DIM, maxy = -MAX_DIM, maxz = -MAX_DIM;
	for (int i = 0; i < positions.size(); i++)
	{
		pos = positions[i];
		if (minx > pos.x) minx = pos.x;
		if (miny > pos.y) miny = pos.y;
		if (minz > pos.z) minz = pos.z;
		if (maxx < pos.x) maxx = pos.x;
		if (maxy < pos.y) maxy = pos.y;
		if (maxz < pos.z) maxz = pos.z;
	}

	float scale_factor = max(maxx - minx, maxy - miny);
	scale_factor = max(scale_factor, maxz - minz);
	scale = glm::vec3(scale_factor);

	for (int i = 0; i < positions.size(); i++)
	{
		positions[i] -= glm::vec3(minx, miny, minz);
		positions[i] /= scale;
	}

	control_points1.resize(64);
	control_points2.resize(64);
	control_points.resize(64);
	coefficients.resize(positions.size() * 64);

	FFD::InitControlPoints(control_points1);
	FFD::InitControlPoints(control_points);
	FFD::CreateBernsteinCoefficients(coefficients, positions);
	FFD::ComputeNewControlPointsPositions(control_points2);
	
	{
		Mesh* mesh = new Mesh("CP");
		meshes[mesh->GetMeshID()] = mesh;
		for (int i = 0; i < 64; i++)
		{
			indices_cp.push_back(i);
			normals_cp.push_back(glm::vec3(1, 0, 0));
		}
		mesh->InitFromData(control_points1, normals_cp, indices_cp);
		mesh->SetDrawMode(GL_POINTS);
	}


	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "lab5", "shaders");
	{
		Shader* shader = new Shader("MainShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}


}

void lab5::FrameStart()
{
	FFD::AnimateControlPointPositions(control_points1, control_points2, control_points, frame, 20);
	FFD::ComputeVerticesPosition(control_points, coefficients, positions);
	frame++;
	if (frame >= 19)
		frame = 0;
}

void lab5::Update(float deltaTimeSeconds)
{
	ClearScreen();
	
	{
		auto shader = shaders["MainShader"];

		shader->Use();
		auto camera = GetSceneCamera();
		glm::vec3 eye_position = camera->m_transform->GetLocalPosition();

		glUniform3f(glGetUniformLocation(shader->program, "light_position"), light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(shader->program, "eye_position"), eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), material_shininess);
		glUniform1f(glGetUniformLocation(shader->program, "material_kd"), material_kd);
		glUniform1f(glGetUniformLocation(shader->program, "material_ks"), material_ks);
		
		glUniform3f(glGetUniformLocation(shader->program, "color"), 0.3, 0.2, 1);
		Mesh *mesh_obj = meshes["bunny"];
		mesh_obj->InitFromData(positions, normals, indices);
		RenderMesh(mesh_obj, shaders["MainShader"], glm::vec3(0,0,0), glm::vec3(1, 1, 1));

		glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 0, 0);
		Mesh *mesh_cp= meshes["CP"];
		mesh_cp->InitFromData(control_points, normals_cp, indices_cp);
		glPointSize(7);
		RenderMesh(mesh_cp, shaders["MainShader"], glm::vec3(0, 0, 0),glm::vec3(1,1,1));
	}
	
}



void lab5::FrameEnd()
{
	DrawCoordinateSystem();
}

void lab5::OnInputUpdate(float deltaTime, int mods)
{

};

void lab5::OnKeyPress(int key, int mods)
{
	// add key press event
};

void lab5::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void lab5::OnWindowResize(int width, int height)
{
	// treat window resize event
}

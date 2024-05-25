#include "Tema2.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace vdvac;


Tema2::Tema2()
{
	traj_no_of_generated_points = 10; // number of points on a Bezier curve (how smooth it is)
	// Define control points
	traj_control_p0 = glm::vec3(-4.0, 0, 1.0);
	traj_control_p1 = glm::vec3(0.5, 1.5, 1.0);
	traj_control_p2 = glm::vec3(0.5, 3.0, 1.0);
	traj_control_p3 = glm::vec3(-4.0, 6, 1.0);
}

Tema2::~Tema2()
{

}

void Tema2::Init()
{
	ToggleGroundPlane();
	auto camera = GetSceneCamera();
	//camera->SetPositionAndRotation(glm::vec3(2, 2, 2), glm::quat(glm::vec3(-45 * TO_RADIANS, 45 * TO_RADIANS, 0)));
	camera->Update();

	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "Tema2", "shaders");
	{
		Shader* shader = new Shader("MainShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		shader = new Shader("BezierShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	CreateLineMesh("line");

	//define coeffiecient values for speed curve and generate it
	SetSpeedCurveLinear();
	CreateSpeedCurve("speedCurve");

	SetSpeedCurveEaseIn();
	CreateSpeedCurveEaseInEaseOut("speedCurveEIEO");
}

void Tema2::FrameStart()
{

}

void Tema2::Update(float deltaTimeSeconds)
{
	ClearScreen(glm::vec3(0.121, 0.168, 0.372));

	//Draw bezier curve
	//auto shader = shaders["MainShader"];
	//shader->Use();

	glm::mat4 model_matrix = glm::mat4(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//GetSceneCamera()->SetOrthographic(1, 1, 0.0001, 200);
	RenderBezierCurve(meshes["line"], shaders["BezierShader"], model_matrix, glm::vec3(0, 1, 1));
	//RenderMesh(meshes["speedCurve"], shaders["MainShader"], model_matrix);
	RenderMesh(meshes["speedCurveEIEO"], shaders["MainShader"], model_matrix);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



}

void Tema2::FrameEnd()
{
	DrawCoordinateSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{

};

void Tema2::OnKeyPress(int key, int mods)
{

};

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Tema2::OnWindowResize(int width, int height)
{
	// treat window resize event
}

void Tema2::ClearScreen(const glm::vec3& color)
{
	glm::ivec2 resolution = window->props.resolution;

	// Sets the clear color for the color buffer
	glClearColor(color.x, color.y, color.z, 1);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

// for hw
void Tema2::CreateLineMesh(std::string name) {

	vector<VertexFormat> vertices
	{
		VertexFormat(traj_control_p0, glm::vec3(0, 1, 1)),
		VertexFormat(traj_control_p1, glm::vec3(0, 1, 0))
	};

	vector<unsigned int> indices =
	{
		0, 1
	};

	meshes[name] = new Mesh("simpleLine");
	meshes[name]->InitFromData(vertices, indices);
	meshes[name]->SetDrawMode(GL_LINES);
}

void Tema2::CreateSpeedCurve(std::string name) {

	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	float pass = 1.0 / float(_speed_curve_no_generated_points);

	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	for (int i = 1; i < _speed_curve_no_generated_points; i++) {
		vertices.push_back(VertexFormat(glm::vec3(i * pass, S(i * pass), 0), glm::vec3(1, 1, 1)));
		indices.push_back(i - 1);
		indices.push_back(i);
	}

	meshes[name] = new Mesh("speedCurve");
	meshes[name]->InitFromData(vertices, indices);
	meshes[name]->SetDrawMode(GL_LINES);
}
void Tema2::CreateSpeedCurveEaseInEaseOut(std::string name) {
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	float pass = 1.0 / float(_speed_curve_no_generated_points);

	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	for (int i = 1; i < _speed_curve_no_generated_points; i++) {
		vertices.push_back(VertexFormat(glm::vec3(i * pass, S(i * pass), 0), glm::vec3(1, 1, 1)));
		indices.push_back(i - 1);
		indices.push_back(i);

		//Change to EaseOut
		if (i == int(_speed_curve_no_generated_points / 2)) {
			std::cout << "DAAAAAAAA";
			SetSpeedCurveEaseOut();
		}
	}

	meshes[name] = new Mesh("speedCurveEaseInEaseOut");
	meshes[name]->InitFromData(vertices, indices);
	meshes[name]->SetDrawMode(GL_LINES);
}

void Tema2::RenderBezierCurve(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color) {
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Send uniforms to shaders
	glUniform3f(glGetUniformLocation(shader->program, "control_p0"), traj_control_p0.x, traj_control_p0.y, traj_control_p0.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p1"), traj_control_p1.x, traj_control_p1.y, traj_control_p1.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p2"), traj_control_p2.x, traj_control_p2.y, traj_control_p2.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p3"), traj_control_p3.x, traj_control_p3.y, traj_control_p3.z);

	//send number of lines to aproximate the curve
	glUniform1i(glGetUniformLocation(shader->program, "u_linesCount"), traj_no_of_generated_points);

	// Draw the object instanced
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);

}

glm::vec3 Tema2::Q_Bezier(float u) {
	return traj_control_p0 * float(pow(1 - u, 3)) +
		traj_control_p1 * 3.0f * u * float(pow(1 - u, 2)) +
		traj_control_p2 * 3.0f * float(pow(u, 2)) * (1 - u) +
		traj_control_p3 * float(pow(u, 3));
}

float Tema2::S(float u) {
	return _as3 * pow(u, 3) + _as2 * pow(u, 2) + _as1 * u + _as0;
}
float Tema2::T(float u) {
	return _at3 * pow(u, 3) + _at2 * pow(u, 2) + _at1 * u + _at0;
}

void vdvac::Tema2::SetSpeedCurveLinear() {

	_as3 = 0;
	_as2 = 0;
	_as1 = 1;
	_as0 = 0;

	_at3 = 0;
	_at2 = 0;
	_at1 = 1;
	_at0 = 0;
}

void vdvac::Tema2::SetSpeedCurveEaseIn() {

	_as3 = 0;
	_as2 = 2;
	_as1 = 0;
	_as0 = 0;

	_at3 = 0;
	_at2 = 0;
	_at1 = 1;
	_at0 = 0;
}

void vdvac::Tema2::SetSpeedCurveEaseOut() {

	_as3 = 0;
	_as2 = -2;
	_as1 = 4;
	_as0 = -1;

	_at3 = 0;
	_at2 = 0;
	_at1 = 1;
	_at0 = 0;
}



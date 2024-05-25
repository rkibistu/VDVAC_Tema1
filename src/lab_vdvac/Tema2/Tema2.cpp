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
	traj_control_p3 = glm::vec3(-4.0, 10, 1.0);
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

	//shaders
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

	// general meshes
	{

		CreateLineMesh("line");

		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Sets trajectory curve and speed curve
	// This can bee changed by pressing specific KEYs (see input methods)
	SetModeBezierEIEO();
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

	//Draw curves
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//RenderBezierCurve(meshes["line"], shaders["BezierShader"], model_matrix, glm::vec3(0, 1, 1));
	RenderMesh(meshes["trajCurve"], shaders["MainShader"], model_matrix);
	//GetSceneCamera()->SetOrthographic(1, 1, 0.0001, 200);
	RenderMesh(meshes["speedCurve"], shaders["MainShader"], model_matrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw aniamted object
	if (_play) {
		//normalize animation timer (because speed curve has valuew in itnevral [0,1])
		float ti = _animationTimer / _animationDuration;

		float Su = CalculateSu(ti);
		float u = CalculateFinalU(Su);

		glm::vec3 Q;
		if(_trajMode == TrajMode::Bezier)
			Q = Q_Bezier(u);
		else if(_trajMode == TrajMode::Circle)
			Q = Q_circle(u);
		else {
			std::cout << "[ERROR] WRONG TRAJ MODE! \n";
			return;
		}

		model_matrix = glm::translate(model_matrix, Q);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.3, 0.3, 0.3));
		RenderMesh(meshes["sphere"], shaders["MainShader"], model_matrix);

		_animationTimer += deltaTimeSeconds;
		if (_animationTimer >= _animationDuration)
			_play = false;
	}
	else {
		model_matrix = glm::translate(model_matrix, traj_control_p0);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.3, 0.3, 0.3));
		RenderMesh(meshes["sphere"], shaders["MainShader"], model_matrix);
	}
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
	if (key == GLFW_KEY_SPACE) {
		_play = true;
		_animationTimer = 0;
	}

	if (key == GLFW_KEY_1) {
		std::cout << "Bezier + linear mode\n";
		SetModeBezierLinear();
		_animationTimer = 0;
	}
	if (key == GLFW_KEY_2) {
		std::cout << "Bezier + EIEO mode\n";
		SetModeBezierEIEO();
		_animationTimer = 0;
	}
	if (key == GLFW_KEY_3) {
		std::cout << "circle + EIEO mode\n";
		SetModeCircleEIEO();
		_animationTimer = 0;
	}
	if (key == GLFW_KEY_4) {
		std::cout << "circle + linear mode\n";
		SetModeCircleLinear();
		_animationTimer = 0;
	}
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

void Tema2::CreateSpeedCurveLinear(std::string name) {

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
			SetSpeedCurveEaseOut();
		}
	}

	meshes[name] = new Mesh("speedCurveEaseInEaseOut");
	meshes[name]->InitFromData(vertices, indices);
	meshes[name]->SetDrawMode(GL_LINES);
}

void Tema2::CreateCircleCurve(std::string name) {

	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	float pass = 2 * 3.14159265358979323846 / float(traj_no_of_generated_points);

	vertices.push_back(VertexFormat(Q_circle(0), glm::vec3(1, 1, 1)));
	for (int i = 1; i <= traj_no_of_generated_points; i++) {
		vertices.push_back(VertexFormat(Q_circle(i * pass), glm::vec3(1, 1, 1)));
		indices.push_back(i - 1);
		indices.push_back(i);
	}

	meshes[name] = new Mesh("circle");
	meshes[name]->InitFromData(vertices, indices);
	meshes[name]->SetDrawMode(GL_LINES);
}
void Tema2::CreateBezierCurve(std::string name) {

	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	float pass = 0.01;
	int index = 1;
	vertices.push_back(VertexFormat(Q_Bezier(0), glm::vec3(1, 1, 1)));
	while (index * pass <= 1.0) {
		vertices.push_back(VertexFormat(Q_Bezier(index * pass), glm::vec3(1, 1, 1)));
		indices.push_back(index - 1);
		indices.push_back(index);
		index++;
	}

	meshes[name] = new Mesh("bezier");
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
glm::vec3 Tema2::Q_circle(float u) {

	glm::vec3 offset = glm::vec3(-5, 0, 0);
	glm::vec3 pos = glm::vec3(
		_circleRadius * cos(u),
		_circleRadius * sin(u),
		0.0f);

	return pos + offset;
}

float Tema2::S(float u) {
	return _as3 * pow(u, 3) + _as2 * pow(u, 2) + _as1 * u + _as0;
}
float Tema2::T(float u) {
	return _at3 * pow(u, 3) + _at2 * pow(u, 2) + _at1 * u + _at0;
}

void Tema2::GenerateTableV() {
	// For u = [0,1], generate T and S values
	float pass = 0.01;
	float u = 0;
	TableElementV el;
	bool changed = false;
	while (u <= 1) {

		if (!changed && u >= 0.5) {
			changed = true;
		}

		el.u = u;
		el.Tu = T(u);
		el.Su = S(u);
		_tableV.push_back(el);

		u += pass;
	}
}
void Tema2::GenerateTableV_EIEO() {
	// For u = [0,1], generate T and S values
	SetSpeedCurveEaseIn();

	float pass = 0.01;
	float u = 0;
	TableElementV el;
	bool changed = false;
	while (u <= 1) {

		if (!changed && u >= 0.5) {
			changed = true;
			SetSpeedCurveEaseOut();
		}

		el.u = u;
		el.Tu = T(u);
		el.Su = S(u);
		_tableV.push_back(el);

		u += pass;
	}
}
void Tema2::GenerateTableBezierQ() {
	// For u = [0,1], generate Q values
	float pass = 0.01;
	float u = 0;
	TableElementQ el;
	glm::vec3 p1, p2;
	float dx, dy, dz;
	float distance, totalDistance = 0;

	// write first eleement before while
	el.u = 0;
	el.Qu = 0;
	_tableQ.push_back(el);
	u += pass;
	while (u <= 1) {

		//calcualte distance between current point and last point
		p1 = Q_Bezier(u);
		p2 = Q_Bezier(u - pass);
		dx = abs(p1.x - p2.x);
		dy = abs(p1.y - p2.y);
		dz = abs(p1.z - p2.z);
		distance = pow(dx * dx + dy * dy + dz * dz, 0.5);

		el.u = u;
		el.Qu = totalDistance + distance;
		_tableQ.push_back(el);

		u += pass;
		totalDistance += distance;
	}
}
void Tema2::GenerateTableCircleQ() {
	// For u = [0,1], generate Q values
	float pass = 2 * 3.14159265358979323846 / float(traj_no_of_generated_points);
	float u = 0;
	TableElementQ el;
	glm::vec3 p1, p2;
	float dx, dy, dz;
	float distance, totalDistance = 0;

	// write first eleement before while
	el.u = 0;
	el.Qu = 0;
	_tableQ.push_back(el);
	u += pass;
	for(int i=0;i<traj_no_of_generated_points;i++) {

		//calcualte distance between current point and last point
		p1 = Q_circle(u);
		p2 = Q_circle(u - pass);
		dx = abs(p1.x - p2.x);
		dy = abs(p1.y - p2.y);
		dz = abs(p1.z - p2.z);
		distance = pow(dx * dx + dy * dy + dz * dz, 0.5);

		el.u = u; 
		el.Qu = totalDistance + distance;
		_tableQ.push_back(el);

		u += pass;
		totalDistance += distance;
	}
}
void Tema2::NormalizeTableQ() {
	// Find the minimum and maximum values in the array
	double min_val = _tableQ[0].Qu;
	double max_val = _tableQ[_tableQ.size() - 1].Qu;

	// Normalize each element in the array

	TableElementQ el;
	for (int i = 0; i < _tableQ.size(); i++) {
		el.u = _tableQ[i].u;
		el.Qu = (_tableQ[i].Qu - min_val) / (max_val - min_val);
		_normalizedTableQ.push_back(el);
	}
}
float Tema2::CalculateSu(float ti) {

	int high, low;
	// Find closest value
	int index = BinarySearch(_tableV, 0, _tableV.size() - 1, ti);

	// Choose interval where Ti is found
	if (index == 0) {
		low = 0;
		high = low + 1;
	}
	else if (index == _tableV.size() - 1) {
		high = _tableV.size() - 1;
		low = high - 1;
	}
	else if (ti >= _tableV[index - 1].Tu && ti <= _tableV[index].Tu) {
		low = index - 1;
		high = index;
	}
	else if (ti >= _tableV[index].Tu && ti <= _tableV[index + 1].Tu) {
		low = index;
		high = index + 1;
	}
	else {
		std::cout << "[ERROR] SOEMTHIGN WRONG HERE! SHouldn happend this!\n\n";
	}

	// Calculate Su
	float Su = interpolate(ti, _tableV[low].Tu, _tableV[high].Tu, _tableV[low].Su, _tableV[high].Su);

	return Su;
}
float Tema2::CalculateFinalU(float Su) {

	int high, low;
	// Find closest value
	int index = BinarySearchQ(_normalizedTableQ, 0, _normalizedTableQ.size() - 1, Su);

	// Choose interval where u is found
	if (index == 0) {
		low = 0;
		high = low + 1;
	}
	else if (index == _normalizedTableQ.size() - 1) {
		high = _normalizedTableQ.size() - 1;
		low = high - 1;
	}
	else if (Su >= _normalizedTableQ[index - 1].Qu && Su <= _normalizedTableQ[index].Qu) {
		low = index - 1;
		high = index;
	}
	else if (Su >= _normalizedTableQ[index].Qu && Su <= _normalizedTableQ[index + 1].Qu) {
		low = index;
		high = index + 1;
	}
	else {
		std::cout << "[ERROR] SOEMTHIGN WRONG HERE! SHouldn happend this!\n\n";
	}

	// Calculate Su
	float u = interpolate(Su, _normalizedTableQ[low].Qu, _normalizedTableQ[high].Qu, _normalizedTableQ[low].u, _normalizedTableQ[high].u);

	return u;
}

void Tema2::SetSpeedCurveLinear() {

	_as3 = 0;
	_as2 = 0;
	_as1 = 1;
	_as0 = 0;

	_at3 = 0;
	_at2 = 0;
	_at1 = 1;
	_at0 = 0;
}

void Tema2::SetSpeedCurveEaseIn() {

	_as3 = 0;
	_as2 = 2;
	_as1 = 0;
	_as0 = 0;

	_at3 = 0;
	_at2 = 0;
	_at1 = 1;
	_at0 = 0;
}

void Tema2::SetSpeedCurveEaseOut() {

	_as3 = 0;
	_as2 = -2;
	_as1 = 4;
	_as0 = -1;

	_at3 = 0;
	_at2 = 0;
	_at1 = 1;
	_at0 = 0;
}

int Tema2::BinarySearch(std::vector<TableElementV> arr, int low, int high, float x) {

	// Base case: If the search space becomes empty, the 
	// element is not present in the array 
	if (high >= low) {
		// Calculate the middle index to divide the search 
		// space in half 
		int mid = low + (high - low) / 2;

		// If the middle element is equal to 'x', we have 
		// found the element, return its index 
		if (arr[mid].Tu == x)
			return mid;

		// If the middle element is greater than 'x', search 
		// in the left half of the array 
		if (arr[mid].Tu > x)
			return BinarySearch(arr, low, mid - 1, x);

		// If the middle element is less than 'x', search in 
		// the right half of the array 
		return BinarySearch(arr, mid + 1, high, x);
	}

	return high;
}
int Tema2::BinarySearchQ(std::vector<TableElementQ> arr, int low, int high, float x) {

	// Base case: If the search space becomes empty, the 
	// element is not present in the array 
	if (high >= low) {
		// Calculate the middle index to divide the search 
		// space in half 
		int mid = low + (high - low) / 2;

		// If the middle element is equal to 'x', we have 
		// found the element, return its index 
		if (arr[mid].Qu == x)
			return mid;

		// If the middle element is greater than 'x', search 
		// in the left half of the array 
		if (arr[mid].Qu > x)
			return BinarySearchQ(arr, low, mid - 1, x);

		// If the middle element is less than 'x', search in 
		// the right half of the array 
		return BinarySearchQ(arr, mid + 1, high, x);
	}

	return high;
}

float Tema2::interpolate(float x, float t1, float t2, float s1, float s2) {
	return s1 + (x - t1) * (s2 - s1) / (t2 - t1);
}

void Tema2::ClearOldData() {
	_tableV.clear();
	_tableQ.clear();
	_normalizedTableQ.clear();

	//TODO
	// I should free the memory of old mesh of speedcurve 
	// because CreateSpeedCurve will overwrite the poitner
	// and we will have memory leaks...
	// But there is an error from the framework
	/*if (meshes["speedCurve"])
		delete(meshes["speedCurve"]);*/
}
void Tema2::SetModeBezierEIEO() {
	// Bezier trajectory with EIEO speed curve
	ClearOldData();
	SetSpeedCurveEaseIn();
	CreateBezierCurve("trajCurve");
	CreateSpeedCurveEaseInEaseOut("speedCurve");
	GenerateTableV_EIEO();
	GenerateTableBezierQ();
	NormalizeTableQ();

	_trajMode = TrajMode::Bezier;
}
void Tema2::SetModeBezierLinear() {
	// Bezier trajectory with linear speed curve
	ClearOldData();
	SetSpeedCurveLinear();
	CreateBezierCurve("trajCurve");
	CreateSpeedCurveLinear("speedCurve");
	GenerateTableV();
	GenerateTableBezierQ();
	NormalizeTableQ();

	_trajMode = TrajMode::Bezier;
}
void Tema2::SetModeCircleEIEO(){
	
	ClearOldData();
	SetSpeedCurveEaseIn();
	CreateCircleCurve("trajCurve");
	CreateSpeedCurveEaseInEaseOut("speedCurve");
	GenerateTableV_EIEO();
	GenerateTableCircleQ();
	NormalizeTableQ();

	_trajMode = TrajMode::Circle;
}
void Tema2::SetModeCircleLinear() {

	ClearOldData();
	SetSpeedCurveLinear();
	CreateCircleCurve("trajCurve");
	CreateSpeedCurveLinear("speedCurve");
	GenerateTableV();
	GenerateTableCircleQ();
	NormalizeTableQ();

	_trajMode = TrajMode::Circle;
}

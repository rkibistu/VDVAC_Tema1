#include "lab1.h"
#include "LookupTables.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace vdvac;
float epsilon = 0.0001;

lab1::lab1()
{
}

lab1::~lab1()
{
	//distruge obiecte
	if (volumeData) delete[] volumeData;
	if (tri) delete[] tri;
}

bool lab1::loadRAWFile(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z)
{
	FILE* File = NULL;

	if (!fileLocation.c_str())
		return false;

	fopen_s(&File, fileLocation.c_str(), "rb");

	if (!File)
		return false;

	xsize = x;
	ysize = y;
	zsize = z;
	volumeData = new unsigned char[xsize * ysize * zsize];

	int sliceSize = xsize * ysize;
	for (int slice = 0; slice < zsize; slice++)
		fread(&volumeData[slice * ysize * xsize], sizeof(unsigned char), sliceSize, File);

	fclose(File);
	return true;
}

glm::vec3 lab1::VertexInterp(double isolevel, glm::vec3 p1, glm::vec3 p2, double valp1, double valp2)
{
	float mu;
	glm::vec3 p;

	if (abs(isolevel - valp1) < epsilon)
		return(p1);
	if (abs(isolevel - valp2) < epsilon)
		return(p2);
	if (abs(valp1 - valp2) < epsilon)
		return(p1);
	mu = (isolevel - valp1) / (valp2 - valp1);
	p = p1 + mu * (p2 - p1);

	return p;
}


int lab1::PolygoniseCube(GRIDCELL g, double iso, TRIANGLE* tri) {

	int i, ntri = 0;
	int cubeindex;
	glm::vec3 vertlist[12];  //tabloul de vârfuri care rezultă în urma intersecției 
	//dintre voxel si izosuprafata
	glm::vec3 normlist[12];  //tabloul de normale
	glm::vec3 normal1, normal2, normal3; //se folosesc daca avem cate o normala per vârf
	glm::vec3 normal;        // se foloseste daca am o singura normala per triunghi

	/*
	Determina indicele din tabela de muchii care ne spune
	care vârfuri sunt în interiorul izosuprafetei
	*/

	cubeindex = 0;
	if (g.val[0] < iso) cubeindex |= 1;
	if (g.val[1] < iso) cubeindex |= 2;
	if (g.val[2] < iso) cubeindex |= 4;
	if (g.val[3] < iso) cubeindex |= 8;
	if (g.val[4] < iso) cubeindex |= 16;
	if (g.val[5] < iso) cubeindex |= 32;
	if (g.val[6] < iso) cubeindex |= 64;
	if (g.val[7] < iso) cubeindex |= 128;

	/* voxelul este complet in interiorul/exteriorul izosuprafetei */
	if (edgeTable[cubeindex] == 0)
		return(0);

	/* determina varfurile unde izosuprafata intersecteaza voxelul */
	if (edgeTable[cubeindex] & 1) {
		//TODO4 completeaza si normallist[] prin interpolare intre normalele din varfurile voxelului
		//valabil pentru toate muchiile
		vertlist[0] = VertexInterp(iso, g.p[0], g.p[1], g.val[0], g.val[1]);
		normlist[0] = VertexInterp(iso, g.n[0], g.n[1], g.val[0], g.val[1]);
	}

	if (edgeTable[cubeindex] & 2) {
		vertlist[1] = VertexInterp(iso, g.p[1], g.p[2], g.val[1], g.val[2]);
		normlist[1] = VertexInterp(iso, g.n[1], g.n[2], g.val[1], g.val[2]);

	}
	if (edgeTable[cubeindex] & 4) {
		vertlist[2] = VertexInterp(iso, g.p[2], g.p[3], g.val[2], g.val[3]);
		normlist[2] = VertexInterp(iso, g.n[2], g.n[3], g.val[2], g.val[3]);

	}
	if (edgeTable[cubeindex] & 8) {
		vertlist[3] = VertexInterp(iso, g.p[3], g.p[0], g.val[3], g.val[0]);
		normlist[3] = VertexInterp(iso, g.n[3], g.n[0], g.val[3], g.val[0]);

	}
	if (edgeTable[cubeindex] & 16) {
		vertlist[4] = VertexInterp(iso, g.p[4], g.p[5], g.val[4], g.val[5]);
		normlist[4] = VertexInterp(iso, g.n[4], g.n[5], g.val[4], g.val[5]);

	}
	if (edgeTable[cubeindex] & 32) {
		vertlist[5] = VertexInterp(iso, g.p[5], g.p[6], g.val[5], g.val[6]);
		normlist[5] = VertexInterp(iso, g.n[5], g.n[6], g.val[5], g.val[6]);

	}
	if (edgeTable[cubeindex] & 64) {
		vertlist[6] = VertexInterp(iso, g.p[6], g.p[7], g.val[6], g.val[7]);
		normlist[6] = VertexInterp(iso, g.n[6], g.n[7], g.val[6], g.val[7]);

	}
	if (edgeTable[cubeindex] & 128) {
		vertlist[7] = VertexInterp(iso, g.p[7], g.p[4], g.val[7], g.val[4]);
		normlist[7] = VertexInterp(iso, g.n[7], g.n[4], g.val[7], g.val[4]);

	}
	if (edgeTable[cubeindex] & 256) {
		vertlist[8] = VertexInterp(iso, g.p[0], g.p[4], g.val[0], g.val[4]);
		normlist[8] = VertexInterp(iso, g.n[0], g.n[4], g.val[0], g.val[4]);

	}
	if (edgeTable[cubeindex] & 512) {
		vertlist[9] = VertexInterp(iso, g.p[1], g.p[5], g.val[1], g.val[5]);
		normlist[9] = VertexInterp(iso, g.n[1], g.n[5], g.val[1], g.val[5]);

	}
	if (edgeTable[cubeindex] & 1024) {
		vertlist[10] = VertexInterp(iso, g.p[2], g.p[6], g.val[2], g.val[6]);
		normlist[10] = VertexInterp(iso, g.n[2], g.n[6], g.val[2], g.val[6]);

	}
	if (edgeTable[cubeindex] & 2048) {
		vertlist[11] = VertexInterp(iso, g.p[3], g.p[7], g.val[3], g.val[7]);
		normlist[11] = VertexInterp(iso, g.n[3], g.n[7], g.val[3], g.val[7]);

	}

	/* Creeaza triunghiurile */
	for (i = 0; triTable[cubeindex][i] != -1; i += 3) {
		tri[ntri].p[0] = vertlist[triTable[cubeindex][i]];
		tri[ntri].p[1] = vertlist[triTable[cubeindex][i + 1]];
		tri[ntri].p[2] = vertlist[triTable[cubeindex][i + 2]];

		/*normal = glm::cross(tri[ntri].p[2] - tri[ntri].p[1], tri[ntri].p[0] - tri[ntri].p[1]);
		if (glm::length(normal) > epsilon)
			normal = glm::normalize(normal);
		tri[ntri].n[0] = normal;
		tri[ntri].n[1] = normal;
		tri[ntri].n[2] = normal;*/

		glm::vec3 normal1 = normlist[triTable[cubeindex][i]];
		if (glm::length(normal1) > epsilon)
			normal1 = glm::normalize(normal1);

		glm::vec3 normal2 = normlist[triTable[cubeindex][i + 1]];
		if (glm::length(normal2) > epsilon)
			normal2 = glm::normalize(normal2);

		glm::vec3 normal3 = normlist[triTable[cubeindex][i + 2]];
		if (glm::length(normal3) > epsilon)
			normal3 = glm::normalize(normal3);

		//TODO5 - determina normalele triunghiului normal1,normal2, normal3 
		//din tabloul normallist[] și decomenteaza ce e mai jos
		tri[ntri].n[0] = normal1;
		tri[ntri].n[1] = normal2;
		tri[ntri].n[2] = normal3;

		ntri++;
	}

	return(ntri);


}


void lab1::reconstructSurface(Mesh* mesh)
{


	GRIDCELL grid;
	TRIANGLE triangles[10];

	int ntri = 0;
	int x, y, z;
	int n, l, i;



	for (x = 0; x < xsize - 1; x++)
	{
		for (y = 0; y < ysize - 1; y++)
		{
			for (z = 0; z < zsize - 1; z++)
			{
				//TODO1
				//completeaza grid.p[] pentru toate varfurile voxelului
				//completeaza grid.val[] pentru toate varfurile voxelului

				for (int i = 0; i < 8; i++) {
					grid.p[i] = glm::vec3(x + offsets[i][0], y + offsets[i][1], z + offsets[i][2]);
					grid.val[i] = volumeData[(z + offsets[i][2]) * xsize * ysize + (y + offsets[i][1]) * xsize + (x + offsets[i][0])];
					}
				//TODO3
				//completeaza grid.n[] pentru toate varfurile voxelului
				//normalele se calculeaza prin diferente finite si apoi se normalizeaza
				for (int i = 0; i < 8; i++) {

					//calculam indicii dreapta/stanga astfel incat sa nu accesam memorie nealocata
					int rightX = (x + offsets[i][0] == xsize - 1) ? xsize - 1 : x + offsets[i][0] + 1;
					int rightY = (y + offsets[i][1] == ysize - 1) ? ysize - 1 : y + offsets[i][1] + 1;
					int rightZ = (z + offsets[i][2] == zsize - 1) ? zsize - 1 : z + offsets[i][2] + 1;

					int leftY = (y + offsets[i][1] == 0) ? 0 : y + offsets[i][1] - 1;
					int leftZ = (z + offsets[i][2] == 0) ? 0 : z + offsets[i][2] - 1;
					int leftX = (x + offsets[i][0] == 0) ? 0 : x + offsets[i][0] - 1;

					grid.n[i] = glm::vec3(
						volumeData[(z + offsets[i][2]) * xsize * ysize + (y + offsets[i][1]) * xsize + rightX] - volumeData[(z + offsets[i][2]) * xsize * ysize + (y + offsets[i][1]) * xsize + leftX],
						volumeData[(z + offsets[i][2]) * xsize * ysize + rightY * xsize + (x + offsets[i][0])] - volumeData[(z + offsets[i][2]) * xsize * ysize + leftY * xsize + (x + offsets[i][0])],
						volumeData[rightZ * xsize * ysize + (y + offsets[i][1]) * xsize + (x + offsets[i][0])] - volumeData[leftZ * xsize * ysize + y + offsets[i][1] * xsize + (x + offsets[i][0])]
					);
				}


				n = PolygoniseCube(grid, isolevel, triangles);

				tri = (TRIANGLE*)realloc(tri, (ntri + n) * sizeof(TRIANGLE));
				for (l = 0; l < n; l++)
					tri[ntri + l] = triangles[l];
				ntri += n;
			}
		}
	}

	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	vector<unsigned int> indices;
	for (i = 0; i < ntri; i++)
	{
		positions.push_back(glm::vec3(tri[i].p[0].x, tri[i].p[0].y, tri[i].p[0].z));
		positions.push_back(glm::vec3(tri[i].p[1].x, tri[i].p[1].y, tri[i].p[1].z));
		positions.push_back(glm::vec3(tri[i].p[2].x, tri[i].p[2].y, tri[i].p[2].z));

		normals.push_back(glm::vec3(tri[i].n[0].x, tri[i].n[0].y, tri[i].n[0].z));
		normals.push_back(glm::vec3(tri[i].n[1].x, tri[i].n[1].y, tri[i].n[1].z));
		normals.push_back(glm::vec3(tri[i].n[2].x, tri[i].n[2].y, tri[i].n[2].z));

		indices.push_back(i * 3);
		indices.push_back(i * 3 + 1);
		indices.push_back(i * 3 + 2);

	}

	mesh->InitFromData(positions, normals, indices);

}


void lab1::Init()
{
	tri = NULL;

	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(2, 5, 10), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
	camera->Update();

	{
		loadRAWFile(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES, "bucky.raw"), 32, 32, 32);
		isolevel = 100;
		Mesh* mesh_volume = new Mesh("volume");
		reconstructSurface(mesh_volume);
		meshes[mesh_volume->GetMeshID()] = mesh_volume;
	}

	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "lab1", "shaders");
	{
		Shader* shader = new Shader("MainShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void lab1::FrameStart()
{
}

void lab1::Update(float deltaTimeSeconds)
{
	ClearScreen();

	{
		auto shader = shaders["MainShader"];
		shader->Use();
		RenderMesh(meshes["volume"], shaders["MainShader"], glm::vec3(0, 0, 0), glm::vec3(5.0f / xsize));
	}
}

void lab1::FrameEnd()
{
	DrawCoordinateSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void lab1::OnInputUpdate(float deltaTime, int mods)
{
	//TODO 2 - sa se modifice izovaloarea la apasarea unei taste
	// Atentie! - trebuie sa se reapeleze functia reconstructSurface()

	if (window->KeyHold(GLFW_KEY_SPACE)) {
		if (isolevel < 255)
			isolevel++;
	}
	if (window->KeyHold(GLFW_KEY_X)) {
		if (isolevel > 1)
			isolevel--;
	}
	reconstructSurface(meshes["volume"]);
};

void lab1::OnKeyPress(int key, int mods)
{
	// add key press event
};

void lab1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void lab1::OnWindowResize(int width, int height)
{
	// treat window resize event
}

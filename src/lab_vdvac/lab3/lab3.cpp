#include "lab3.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace vdvac;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

lab3::lab3()
{
}

lab3::~lab3()
{
	//distruge obiecte
	glDeleteTextures(xsize, textures_idX);
	if (volumeData) delete[] volumeData;
	if (textures_idX) delete[] textures_idX;

}

bool lab3::loadRAWFile(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z)
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

bool lab3::load2DTexturesRAW_X(unsigned int* textures_id, unsigned char* volumeData, unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
	float* currentSlice;
	currentSlice = new float[ysize * zsize];

	bool Status = false;
	float val;
	// Incarca datele din volum
	if (volumeData)
	{
		Status = true;
		//se genereaza toate texturile de-a lungul axei x (de dimensiunea xsize)
		glGenTextures(xsize, &textures_id[0]);

		//OX
		//feliile sunt de-a lungul axei x
		for (int slice = 0; slice < xsize; slice++)
		{
			glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
			//filtrare
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (GLEW_EXT_texture_filter_anisotropic) {
				float maxAnisotropy;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			//cand lucram cu texturi cu dimensiuni non multiple de 4 trebuie sa facem cititorul de randuri
			//ce incarca texturile in OpenGL sa lucreze cu memorie aliniata la 1 (default este la 4)
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			//genereaza textura yOz (de-a lungul feliei curente "slice")
			for (int k = 0; k < zsize; k++)
			{
				for (int j = 0; j < ysize; j++)
				{
					currentSlice[k * ysize + j] = ((float)volumeData[k * ysize * xsize + j * xsize + slice]) / 255.0f;
				}
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ysize, zsize, 0, GL_RED, GL_FLOAT, currentSlice);
		}
	}
	delete[]currentSlice;
	return Status;
}


bool lab3::load2DTexturesRAW_Y(unsigned int* textures_id, unsigned char* volumeData, unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
	//TODO1 
	//incarca texturile de-a lungul axei y
	float* currentSlice;
	currentSlice = new float[xsize * zsize];

	bool Status = false;
	float val;
	// Incarca datele din volum
	if (volumeData)
	{
		Status = true;
		//se genereaza toate texturile de-a lungul axei x (de dimensiunea xsize)
		glGenTextures(ysize, &textures_id[0]);

		//OX
		//feliile sunt de-a lungul axei x
		for (int slice = 0; slice < ysize; slice++)
		{
			glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
			//filtrare
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (GLEW_EXT_texture_filter_anisotropic) {
				float maxAnisotropy;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			//cand lucram cu texturi cu dimensiuni non multiple de 4 trebuie sa facem cititorul de randuri
			//ce incarca texturile in OpenGL sa lucreze cu memorie aliniata la 1 (default este la 4)
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			//genereaza textura yOz (de-a lungul feliei curente "slice")
			for (int k = 0; k < zsize; k++)
			{
				for (int i = 0; i < xsize; i++)
				{
					currentSlice[k * xsize + i] = ((float)volumeData[k * ysize * xsize + slice * xsize + i]) / 255.0f;
				}
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ysize, zsize, 0, GL_RED, GL_FLOAT, currentSlice);
		}
	}
	delete[]currentSlice;
	return Status;
}


//returneaza id-urile texturilor 2D
bool lab3::load2DTexturesRAW_Z(unsigned int* textures_id, unsigned char* volumeData, unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
	//TODO1 
	//incarca texturile de-a lungul axei Z
	float* currentSlice;
	currentSlice = new float[xsize * ysize];

	bool Status = false;
	float val;
	// Incarca datele din volum
	if (volumeData)
	{
		Status = true;
		//se genereaza toate texturile de-a lungul axei x (de dimensiunea xsize)
		glGenTextures(zsize, &textures_id[0]);

		//OX
		//feliile sunt de-a lungul axei x
		for (int slice = 0; slice < zsize; slice++)
		{
			glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
			//filtrare
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (GLEW_EXT_texture_filter_anisotropic) {
				float maxAnisotropy;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
			}
			//cand lucram cu texturi cu dimensiuni non multiple de 4 trebuie sa facem cititorul de randuri
			//ce incarca texturile in OpenGL sa lucreze cu memorie aliniata la 1 (default este la 4)
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			//genereaza textura yOz (de-a lungul feliei curente "slice")
			for (int j = 0; j < ysize; j++)
			{
				for (int i = 0; i < xsize; i++)
				{
					currentSlice[j * xsize + i] = ((float)volumeData[slice * ysize * xsize + j * xsize + i]) / 255.0f;
				}
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, xsize, ysize, 0, GL_RED, GL_FLOAT, currentSlice);
		}
	}
	delete[]currentSlice;
	return Status;
}

Mesh* lab3::createPoligX()
{
	//se creeaza un poligon care este paralel cu planul yOz
	//pentru afisarea poligoanelor de-a lungul axei Ox
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0,-1,-1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(0,0)),
		VertexFormat(glm::vec3(0, 1,-1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(1,0)),
		VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(1,1)),
		VertexFormat(glm::vec3(0,-1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(0,1))
	};
	vector<unsigned int> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	meshes["poligX"] = new Mesh("poligX");
	meshes["poligX"]->InitFromData(vertices, indices);
	meshes["poligX"]->UseMaterials(false);
	return meshes["poligX"];
}

Mesh* lab3::createPoligY()
{
	//TODO2 
	//creeaza un poligon care este paralel cu planul xOz
	//pentru afisarea poligoanelor de-a lungul axei Oy
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1,0,-1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(0,0)),
		VertexFormat(glm::vec3(1,0,-1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(1,0)),
		VertexFormat(glm::vec3(1,0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(1,1)),
		VertexFormat(glm::vec3(-1,0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(0,1))
	};
	vector<unsigned int> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	meshes["poligY"] = new Mesh("poligY");
	meshes["poligY"]->InitFromData(vertices, indices);
	meshes["poligY"]->UseMaterials(false);
	return meshes["poligY"];
}



Mesh* lab3::createPoligZ()
{
	//TODO2 
	//creeaza un poligon care este paralel cu planul xOy
	//pentru afisarea poligoanelor de-a lungul axei Oz
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1,-1,0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(0,0)),
		VertexFormat(glm::vec3(-1, 1,0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(0,1)),
		VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(1,1)),
		VertexFormat(glm::vec3(1,-1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),glm::vec2(1,0))
	};
	vector<unsigned int> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	meshes["poligZ"] = new Mesh("poligZ");
	meshes["poligZ"]->InitFromData(vertices, indices);
	meshes["poligZ"]->UseMaterials(false);
	return meshes["poligZ"];
}





void lab3::Init()
{
	ToggleGroundPlane();
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(2, 2, 2), glm::quat(glm::vec3(-30 * TO_RADIANS, 45 * TO_RADIANS, 0)));
	camera->Update();

	loadRAWFile(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES, "engine.raw"), 256, 256, 256);

	textures_idZ = new unsigned int[zsize];
	textures_idX = new unsigned int[xsize];
	textures_idY = new unsigned int[ysize];

	load2DTexturesRAW_Z(textures_idZ, volumeData, xsize, ysize, zsize);
	load2DTexturesRAW_X(textures_idX, volumeData, xsize, ysize, zsize);
	load2DTexturesRAW_Y(textures_idY, volumeData, xsize, ysize, zsize);

	createPoligX();
	createPoligY();
	createPoligZ();

	std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "lab3", "shaders");
	{
		Shader* shader = new Shader("MainShader");
		shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void lab3::FrameStart()
{

}

void lab3::DesenStivaAxaYNegativa(unsigned int* textures_id, unsigned int num_polig)
{
	//TODO3
	//deseneaza poligoanele de-a lungul axei Oy
	//de la poligonul cel mai indepartat de observator (y = -1)
	//pana la poligonul cel mai apropiat de observator (y = 1)
	cout << "axa Y negativa" << endl;
	float yPos = -1.0f;
	float pas = 2.0f / (float)num_polig;

	glm::mat4 model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, yPos, 0));

	// Seteaza mecanism de amestec
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader* shader = shaders["MainShader"];
	shader->Use();

	//se afiseaza poligoanele de-a lungul axei Ox, in ordinea
	//de la cel mai indepartat de observator (x = -1) 
	//pana la cel mai apropiat de observator (x = 1)
	for (int slice = 0; slice < num_polig; slice++)
	{
		unsigned int slice_unitate_texturare = slice;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
		glUniform1i(glGetUniformLocation(shader->program, "textura1"), 0);

		RenderMesh(meshes["poligY"], shader, model_matrix_slice);
		yPos += pas;
		model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, yPos, 0));
	}
	glDisable(GL_BLEND);
}

void lab3::DesenStivaAxaYPozitiva(unsigned int* textures_id, unsigned int num_polig)
{
	//TODO3
	//deseneaza poligoanele de-a lungul axei Oy
	//de la poligonul cel mai indepartat de observator (y = 1)
	//pana la poligonul cel mai apropiat de observator (y = -1)
	cout << "axa Y pozitiva" << endl;
	float yPos = 1.0f;
	float pas = 2.0f / (float)num_polig;

	glm::mat4 model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, yPos, 0));

	// Seteaza mecanism de amestec
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader* shader = shaders["MainShader"];
	shader->Use();

	//se afiseaza poligoanele de-a lungul axei Ox, in ordinea
	//de la cel mai indepartat de observator (x = 1) 
	//pana la cel mai apropiat de observator (x = -1)
	for (int slice = num_polig - 1; slice >= 0; slice--)
	{
		unsigned int slice_unitate_texturare = slice;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glUniform1i(glGetUniformLocation(shader->program, "textura1"), 0);

		RenderMesh(meshes["poligY"], shader, model_matrix_slice);
		yPos -= pas;
		model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, yPos, 0));
	}
	glDisable(GL_BLEND);
}

void lab3::DesenStivaAxaZNegativa(unsigned int* textures_id, unsigned int num_polig)
{
	//TODO3
	//deseneaza poligoanele de-a lungul axei Oz
	//de la poligonul cel mai indepartat de observator (z = -1)
	//pana la poligonul cel mai apropiat de observator (z = 1)
	cout << "axa Z negativa" << endl;
	float zPos = -1.0f;
	float pas = 2.0f / (float)num_polig;

	glm::mat4 model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, 0, zPos));

	// Seteaza mecanism de amestec
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader* shader = shaders["MainShader"];
	shader->Use();

	//se afiseaza poligoanele de-a lungul axei Ox, in ordinea
	//de la cel mai indepartat de observator (x = -1) 
	//pana la cel mai apropiat de observator (x = 1)
	for (int slice = 0; slice < num_polig; slice++)
	{
		unsigned int slice_unitate_texturare = slice;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
		glUniform1i(glGetUniformLocation(shader->program, "textura1"), 0);

		RenderMesh(meshes["poligZ"], shader, model_matrix_slice);
		zPos += pas;
		model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, 0, zPos));
	}
	glDisable(GL_BLEND);
}

void lab3::DesenStivaAxaZPozitiva(unsigned int* textures_id, unsigned int num_polig)
{
	//TODO3
	//deseneaza poligoanele de-a lungul axei Oz
	//de la poligonul cel mai indepartat de observator (z = 1)
	//pana la poligonul cel mai apropiat de observator (z = -1)
	cout << "axa Z pozitiva" << endl;
	float zPos = 1.0f;
	float pas = 2.0f / (float)num_polig;

	glm::mat4 model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, 0, zPos));

	// Seteaza mecanism de amestec
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader* shader = shaders["MainShader"];
	shader->Use();

	//se afiseaza poligoanele de-a lungul axei Ox, in ordinea
	//de la cel mai indepartat de observator (x = 1) 
	//pana la cel mai apropiat de observator (x = -1)
	for (int slice = num_polig - 1; slice >= 0; slice--)
	{
		unsigned int slice_unitate_texturare = slice;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glUniform1i(glGetUniformLocation(shader->program, "textura1"), 0);

		RenderMesh(meshes["poligZ"], shader, model_matrix_slice);
		zPos -= pas;
		model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(0, 0, zPos));
	}
	glDisable(GL_BLEND);
}


void lab3::DesenStivaAxaXNegativa(unsigned int* textures_id, unsigned int num_polig)
{
	cout << "axa X negativa" << endl;
	float xPos = -1.0f;
	float pas = 2.0f / (float)num_polig;

	glm::mat4 model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(xPos, 0, 0));

	// Seteaza mecanism de amestec
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader* shader = shaders["MainShader"];
	shader->Use();

	//se afiseaza poligoanele de-a lungul axei Ox, in ordinea
	//de la cel mai indepartat de observator (x = -1) 
	//pana la cel mai apropiat de observator (x = 1)
	for (int slice = 0; slice < num_polig; slice++)
	{
		unsigned int slice_unitate_texturare = slice;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
		glUniform1i(glGetUniformLocation(shader->program, "textura1"), 0);

		RenderMesh(meshes["poligX"], shader, model_matrix_slice);
		xPos += pas;
		model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(xPos, 0, 0));
	}
	glDisable(GL_BLEND);
}


void lab3::DesenStivaAxaXPozitiva(unsigned int* textures_id, unsigned int num_polig)
{
	cout << "axa X pozitiva" << endl;
	float xPos = 1.0f;
	float pas = 2.0f / (float)num_polig;

	glm::mat4 model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(xPos, 0, 0));

	// Seteaza mecanism de amestec
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	Shader* shader = shaders["MainShader"];
	shader->Use();

	//se afiseaza poligoanele de-a lungul axei Ox, in ordinea
	//de la cel mai indepartat de observator (x = 1) 
	//pana la cel mai apropiat de observator (x = -1)
	for (int slice = num_polig - 1; slice >= 0; slice--)
	{
		unsigned int slice_unitate_texturare = slice;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures_id[slice]);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glUniform1i(glGetUniformLocation(shader->program, "textura1"), 0);

		RenderMesh(meshes["poligX"], shader, model_matrix_slice);
		xPos -= pas;
		model_matrix_slice = glm::translate(glm::mat4(1), glm::vec3(xPos, 0, 0));
	}
	glDisable(GL_BLEND);
}

void lab3::Update(float deltaTimeSeconds)
{
	ClearScreen();
	{
		auto shader = shaders["MainShader"];

		shader->Use();
		int dir = detMostPerpendicularAxis();

		switch (dir)
		{
		case 1: DesenStivaAxaXNegativa(textures_idX, xsize); break;
		case -1: DesenStivaAxaXPozitiva(textures_idX, xsize); break;
		case 2: DesenStivaAxaYNegativa(textures_idY, ysize); break;
		case -2: DesenStivaAxaYPozitiva(textures_idY, ysize); break;
		case 3: DesenStivaAxaZNegativa(textures_idZ, zsize); break;
		case -3: DesenStivaAxaZPozitiva(textures_idZ, zsize); break;
		default:  DesenStivaAxaXNegativa(textures_idX, xsize); break;
		}
	}

}

//determina cea mai perpendiculara axa (X, Y sau Z) pe planul de vizualizare
int lab3::detMostPerpendicularAxis()
{
	auto camera = GetSceneCamera();
	//vectorul de vizualizare
	glm::vec3 view_vec = glm::normalize(camera->m_transform->GetLocalOZVector());
	//TODO4
	//determina produsele scalare dintre vectorul de vizualizare si cele 3 axe principale 
	//determina cel mai mare produs scalar in valoare absoluta
	//daca acest produs e pozitiv, atunci se alege desenarea stivei unei axe negative (return 1,2 sau 3)
	//daca acest produs e negativ, atunci se alege desenarea stivei unei axe pozitive (return -1, -2 sau -3)

	//glm::vec3(1,0,0) ox   produs scalar cu view_vec


	//produs scalar mic --> unghi mare --> vrem produsul scalar cat mai mare
	
	float dotX = glm::dot(glm::vec3(1, 0, 0), view_vec);
	float dotY = glm::dot(glm::vec3(0, 1, 0), view_vec);
	float dotZ = glm::dot(glm::vec3(0, 0, 1), view_vec);

	if (glm::abs(dotX) >= glm::abs(dotY) && glm::abs(dotX) >= glm::abs(dotZ)) {
		return 1 * glm::sign(dotX);
	}
	if (glm::abs(dotY) >= glm::abs(dotX) && glm::abs(dotY) >= glm::abs(dotZ)) {
		return 2 * glm::sign(dotY);
	}
	else {
		return 3 * glm::sign(dotZ);
	}
}

void lab3::FrameEnd()
{
	DrawCoordinateSystem();

}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void lab3::OnInputUpdate(float deltaTime, int mods)
{

};

void lab3::OnKeyPress(int key, int mods)
{
	// add key press event
};

void lab3::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void lab3::OnWindowResize(int width, int height)
{
	// treat window resize event
}

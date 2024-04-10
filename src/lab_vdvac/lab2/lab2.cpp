#include "lab2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace vdvac;


lab2::lab2()
{
}


lab2::~lab2()
{
    delete[] volumeData;
}


bool lab2::loadRAWFile(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z)
{

    FILE* File = NULL;

    if (!fileLocation.c_str())
    {
        cout << fileLocation << "nu exista" << endl;
        return false;
    }

    fopen_s(&File, fileLocation.c_str(), "rb");

    if (!File)
    {
        cout << fileLocation << "nu a putut fi deschis" << endl;
        return false;
    }

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

//se creeaza un cub in care coordonatele varfurilor
//sunt identice culorilor varfurilor
Mesh* lab2::createCube(const char* name)
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

GLuint lab2::createVolumeTexture(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z) {

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

GLuint lab2::createTFTexture(const string& fileLocation) {
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


void lab2::Init()
{
    frameBuffer = new FrameBuffer();
    auto resolution = window->GetResolution();
    frameBuffer->Generate(resolution.x, resolution.y, 3);
    
    stepSize = 0.001f;

    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(1, 0.2, 2), glm::quat(glm::vec3(-30 * TO_RADIANS, 45 * TO_RADIANS, 0)));
    camera->Update();
    
    Mesh* cube = createCube("cube");

    // quad-ul asta  e pt alt lab 
    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,"primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }


    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::VDVAC, "lab2", "shaders");

    {
        Shader* shader = new Shader("BackFaceShader");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader_backface.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader_backface.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("RayCastingShader");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader_raycasting.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader_raycasting.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    volumeTexture = createVolumeTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES, "brain.raw"), 200, 160, 160);
   
    tfTexture = createTFTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::VOLUMES,"tff.dat"));
}


void lab2::FrameStart()
{

}

void lab2::Update(float deltaTimeSeconds)
{
    //in primul pas se deseneaza in framebuffer
    frameBuffer->Bind();
    glm::mat4 model_matrix = glm::rotate(glm::mat4(1), -45.f * TO_RADIANS, glm::vec3(0, 1, 0));
    model_matrix = glm::rotate(model_matrix, 90.f * TO_RADIANS, glm::vec3(1, 0, 0));
    glm::ivec2 resolution = window->props.resolution;

    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, resolution.x, resolution.y);

    {
        //se foloseste shaderul care deseneaza fetele spate ale cubului
        auto shader = shaders["BackFaceShader"];
        shader->Use();

        //se da enable la culling si se elimina fetele fata (deci se vad fetele spate)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        RenderMesh(meshes["cube"], shaders["BackFaceShader"], model_matrix);
        glDisable(GL_CULL_FACE);
    }

    //se deseneaza pe ecran
    FrameBuffer::BindDefault();
    {
        glm::ivec2 resolution = window->props.resolution;
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, resolution.x, resolution.y);

        //se foloseste shaderul de Ray Casting
        auto shader = shaders["RayCastingShader"];
        shader->Use();

        //se trimite textura 2D cu imaginea cubului (cu fetele spate desenate)
        int textureReflexionLoc = shader->GetUniformLocation("ExitPoints");
        glUniform1i(textureReflexionLoc, 0);
        frameBuffer->BindTexture(0, GL_TEXTURE0);

        //se trimite textura 3D cu datele din volum
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_3D, volumeTexture);
        glUniform1i(glGetUniformLocation(shader->program, "VolumeTex"), 1);

        //se trimite textura 1D cu functia de transfer
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_1D, tfTexture);
        glUniform1i(glGetUniformLocation(shader->program, "TransferFunc"), 2);

        //se trimite pasul de esantionare
        glUniform1f(glGetUniformLocation(shader->program, "StepSize"), stepSize);
        meshes["cube"]->UseMaterials(false);

        //se deseneaza cubul (se vor vedea fetele fata ale cubului)
        RenderMesh(meshes["cube"], shaders["RayCastingShader"], model_matrix);
    }
}


void lab2::FrameEnd()
{
}


void lab2::OnInputUpdate(float deltaTime, int mods)
{
    
}


void lab2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void lab2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void lab2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void lab2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void lab2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void lab2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void lab2::OnWindowResize(int width, int height)
{
    frameBuffer->Generate(width, height, 3);
}

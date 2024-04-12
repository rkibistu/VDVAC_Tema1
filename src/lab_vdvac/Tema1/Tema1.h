#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
using namespace std;

namespace vdvac {

	class Tema1 : public gfxc::SimpleScene
	{


	public:
		Tema1();
		~Tema1();

		void Init() override;


	private:

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		bool loadRAWFile(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z);
		Mesh* createCube(const char* name);
		Mesh* createCubePerimeter(const char* name);
		GLuint createVolumeTexture(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z);
		GLuint createTFTexture(const string& fileLocation);
		float CalculateMaxDistanceToCube(glm::vec3 worldPosition);

		void DrawVolume();
		void DrawPlaneWithColors();
		void DrawPlaneWithTextures();

		unsigned char* volumeData;
		unsigned int xsize, ysize, zsize;

		unsigned int _cubeEdgesTexture;

		float _proxyDistance = 2.3f;
		float _proxyEsantionsCount = 256.0;
		float _proxyPass = 0.1f;
		glm::vec3 _viewVec;
		GLuint _volumeTexture;               //textura 3D (cu datele din volum)
		GLuint _tfTexture;                   //textura 1D (cu functia de transfer)
	
		// 1 - volume
		// 2 - plane with colors
		// 3 - plane with textures
		int _mode = 1;
	};
}
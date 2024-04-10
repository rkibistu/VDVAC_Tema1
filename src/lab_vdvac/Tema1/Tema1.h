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

		unsigned char* volumeData;
		unsigned int xsize, ysize, zsize;

		float _proxyDistance;
	};
}
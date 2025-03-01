#pragma once
#include "components/simple_scene.h"
#include "components/transform.h"
using namespace std;

namespace vdvac {
	class lab6 : public gfxc::SimpleScene
	{


	public:
		lab6();
		~lab6();

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

		void bind(int i);

	private:
		std::unordered_map<std::string, Texture2D*> mapTextures;
		int indice;
		int timp;
		string girl;

		glm::vec3 light_position;
		unsigned int material_shininess;
		float material_kd;
		float material_ks;

		
	};
}
#pragma once
#include "components/simple_scene.h"
#include "components/transform.h"
using namespace std;

namespace vdvac {
	class lab5 : public gfxc::SimpleScene
	{


	public:
		lab5();
		~lab5();

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

	private:
		glm::vec3 light_position;

		unsigned int material_shininess;
		float material_kd;
		float material_ks;

		vector<glm::vec3> positions;
		vector<glm::vec3> normals;
		vector<unsigned int> indices;


		vector<glm::vec3> positions_cp;
		vector<glm::vec3> normals_cp;
		vector<unsigned int> indices_cp;


		vector<float> coefficients;
		std::vector<glm::vec3> control_points1, control_points2, control_points;
		glm::vec3 scale;
		int frame;
	};
}
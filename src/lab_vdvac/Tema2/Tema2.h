#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
using namespace std;

namespace vdvac {

	class Tema2 : public gfxc::SimpleScene
	{


	public:
		Tema2();
		~Tema2();

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

		void CreateLineMesh(std::string name);
		void RenderBezierCurve(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

	private:
		// trajectory data
		glm::vec3 traj_control_p0, traj_control_p1, traj_control_p2, traj_control_p3;
		unsigned int traj_no_of_generated_points;
	};
}
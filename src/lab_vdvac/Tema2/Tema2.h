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

		void ClearScreen(const glm::vec3& color = glm::vec3(0,0,0)) override;

		// used to feed geometry shader to create bezier curve for trajectory
		void CreateLineMesh(std::string name);
		// used to generate the speed curve that will we seen in 2D (Z values always 0)
		// Only for curves that have the same value of S(u) for entire duration
		void CreateSpeedCurve(std::string name);
		// Particualr case of creatSpeedCurve for easein eastout (it need to change s(u) in the middle)
		void CreateSpeedCurveEaseInEaseOut(std::string name);
		
		void RenderBezierCurve(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

		//trajectory curve equation
		glm::vec3 Q_Bezier(float u);
		// speed curve equations
		float S(float u);
		float T(float u);

		// Set the coeffiecient for predefined speed curves
		void SetSpeedCurveLinear();
		void SetSpeedCurveEaseIn();
		void SetSpeedCurveEaseOut();

	private:
		// trajectory data
		glm::vec3 traj_control_p0, traj_control_p1, traj_control_p2, traj_control_p3;
		unsigned int traj_no_of_generated_points;

		// S(u) coeffiecients
		float _as3, _as2, _as1, _as0;
		//T(u) coeffiecients
		float _at3, _at2, _at1, _at0;
		// speed curve smoothness
		int _speed_curve_no_generated_points = 50;
	};
}
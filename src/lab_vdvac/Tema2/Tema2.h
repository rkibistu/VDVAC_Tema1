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

		typedef struct TableElementV{
			float u;
			float Tu;
			float Su;
		} TableElementV;
		typedef struct TableElementQ {
			float u;
			float Qu;
		} TableElementQ;

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
		void CreateSpeedCurveBezier(std::string name);
		// Particualr case of creatSpeedCurve for easein eastout (it need to change s(u) in the middle)
		void CreateSpeedCurveEaseInEaseOut(std::string name);
		
		//trajectory curves
		void CreateCircleCurve(std::string name);
		void CreateBezierCurve(std::string name);

		void RenderBezierCurve(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

		//trajectory curve equation
		glm::vec3 Q_Bezier(float u);
		glm::vec3 Q_circle(float u);
		// speed curve equations
		float S(float u);
		float T(float u);

		//table with T(u) and S(u) - works only for cases when S(u) 
		// doesn t change it equation 
		void GenerateTableV();
		//generate table with T9(u) and S(u) - workd for ease in ease out case
		// It changes the S equation at the middle
		void GenerateTableV_EIEO();
		//table with Q(u) - specific to bezier curve
		// you need to change the equation of calcualtion  for p1 and p2 for other curves
		void GenerateTableBezierQ();
		// we need to normalize distance because Su is in interva; [0,1]
		void NormalizeTableQ();

		// Gets ti (animation time [0,1]) and calculates the Su value (tableV)
		float CalculateSu(float ti);
		// Gets Su calcualted with CalculateSu method and calculates the final U (tableQ)
		// This u values will be sued to calculate the current position of the object
		float CalculateFinalU(float su);

		// Set the coeffiecient for predefined speed curves
		void SetSpeedCurveLinear();
		void SetSpeedCurveEaseIn();
		void SetSpeedCurveEaseOut();

		//utils
		// Returns the index of a number very close to x
		// You have to check if x is in the interval below or above this value.
		int BinarySearch(std::vector<TableElementV> arr, int low, int high, float x);
		int BinarySearchQ(std::vector<TableElementQ> arr, int low, int high, float x);

		// Interpolate
		// x is a value in interval [t1,t2]. Find the coresponding value in intravel [s1,s2]
		float interpolate(float x, float t1, float t2, float s1, float s2);

		// DIFFRENT CURVES
		void ClearOldData();
		void SetModeBezierEIEO();
		void SetModeBezierLinear();
		void SetModeCircleEIEO();

	private:
		// trajectory data
		glm::vec3 traj_control_p0, traj_control_p1, traj_control_p2, traj_control_p3;
		float _circleRadius = 1.0;
		unsigned int traj_no_of_generated_points;

		// S(u) coeffiecients
		float _as3, _as2, _as1, _as0;
		//T(u) coeffiecients
		float _at3, _at2, _at1, _at0;
		// speed curve smoothness
		int _speed_curve_no_generated_points = 50;

		// table with valuews from speed curve
		std::vector<TableElementV> _tableV;
		std::vector<TableElementQ> _tableQ;
		std::vector<TableElementQ> _normalizedTableQ;

		//animation
		bool _play = false; // start animation
		float _animationDuration = 5.0f; //seconds
		float _animationTimer; // used to time the current animation
	};
}
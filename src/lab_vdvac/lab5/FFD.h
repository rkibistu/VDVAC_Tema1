#pragma once
#include <math.h>
#include <vector>
#include "components/simple_scene.h"
#include <iostream>


using namespace std;
namespace FFD
{

	float B0(float x) {
		return pow(1 - x, 3);
	}
	float B1(float x) {
		return 3 * x * pow(1 - x, 2);
	}
	float B2(float x) {
		return 3 * x * x * (1 - x);
	}
	float B3(float x) {
		return pow(x, 3);
	}

	float CalculateCoefficientB(int grad, float value) {

		switch (grad)
		{
		case 0:
			return B0(value);
		case 1:
			return B1(value);
		case 2:
			return B2(value);
		case 3:
			return B3(value);
		default:
			std::cout << "ERROR! Bad value for GRAD of B coefficient: " << grad << "\n";
			return 0;
			break;
		}
	}

	float SimpleCalcualte(int grad, float value) {
		return pow(value, grad) * pow(1 - value, 3 - grad);
	}

	//calculeaza coeficientii Bernstein pentru toate punctele obiectului
	void CreateBernsteinCoefficients(std::vector<float>& coefficients, std::vector<glm::vec3> vertices)
	{
		int offset = 0;
		for (int vertex = 0; vertex < vertices.size(); vertex++)
		{
			for (int k = 0; k <= 3; k++)
			{

				for (int j = 0; j <= 3; j++)
				{

					for (int i = 0; i <= 3; i++)
					{
						//TODO2 calculeaza corect coeficientii Bernstein pentru toate varfurile 
						//varful curent este "vertex"
						//fiecare varf are 64 de coeficienti
						//slide 21, fara pij, doar coeficientii B
						coefficients[offset] = CalculateCoefficientB(i, vertices[vertex].x) *
							CalculateCoefficientB(j, vertices[vertex].y) *
							CalculateCoefficientB(k, vertices[vertex].z);
						offset++; //offset merge de la 0 la vertices.size() * 64 - 1
					}
				}
			}
		}

	}


	//seteaza pozitiile default ale punctelor de control in cubul de latura 1
	//la distanta de 1/3 unul fata de celalalt: 0, 1/3, 2/3, 1
	void InitControlPoints(std::vector<glm::vec3>& control_points)
	{
		int i, j, k, offset;
		offset = 0;
		for (k = 0; k <= 3; k++)
		{
			for (j = 0; j <= 3; j++)
			{
				for (i = 0; i <= 3; i++)
				{
					// 000 1/300 2/300 100 si tot asa
					control_points[offset] = glm::vec3(i * 1/3, j * 1/3, k * 1/3); //TODO1 calculeaza corect

					offset++;
				}
			}
		}

	}

	//calculeaza pozitiile varfurilor obiectului 3D conform coeficientilor Bernstein
	void ComputeVerticesPosition(std::vector<glm::vec3> control_points, std::vector<float> coefficients, std::vector<glm::vec3>& vertices)
	{
		int offset = 0;
		for (int vertex = 0; vertex < vertices.size(); vertex++)
		{
			vertices[vertex].x = 0;
			vertices[vertex].y = 0;
			vertices[vertex].z = 0;

			int offset_control_points = 0;
			for (int k = 0; k <= 3; k++)
			{
				for (int j = 0; j <= 3; j++)
				{
					for (int i = 0; i <= 3; i++)
					{ 
						//TODO3 calculeaza corect in functie de coeficienti si puncte de control
						vertices[vertex] += control_points[offset_control_points] * coefficients[offset];
						offset++;  //offset merge de la 0 la vertices.size() * 64 - 1
						offset_control_points++;  //offset_control_points merge pentru fiecare varf de la 0 la 63
					}
				}
			}
		}

	}

	//se alege o functie de modificare random a punctelor de control
	//aici se poate inlocui cu orice alta functie care are sens (astfel incat fetele obiectului sa nu se intersecteze)
	void ComputeNewControlPointsPositions(std::vector<glm::vec3>& control_points)
	{
		int i, j, k, offset;
		offset = 0;
		for (k = 0; k <= 3; k++)
		{
			for (j = 0; j <= 3; j++)
			{
				for (i = 0; i <= 3; i++)
				{
					control_points[offset].x = (float)i / 3.0f + (rand() - (signed)RAND_MAX / 2) / (3.0f * RAND_MAX);
					control_points[offset].y = (float)j / 3.0f + (rand() - (signed)RAND_MAX / 2) / (3.0f * RAND_MAX);
					control_points[offset].z = (float)k / 3.0f + (rand() - (signed)RAND_MAX / 2) / (3.0f * RAND_MAX);
					offset++;
				}
			}
		}
	}


	void AnimateControlPointPositions(std::vector<glm::vec3>& control_points1, std::vector<glm::vec3>& control_points2, std::vector<glm::vec3>& control_points, int frame, int no_frames)
	{

		if (frame == 0)
		{
			for (int offset = 0; offset < 64; offset++)
			{
				control_points1[offset] = control_points2[offset];
			}


			ComputeNewControlPointsPositions(control_points2);
		}

		for (int offset = 0; offset < 64; offset++)
		{
			control_points[offset].x = ((float)frame * control_points2[offset].x + (no_frames - (float)frame) * control_points1[offset].x) / (float)no_frames;
			control_points[offset].y = ((float)frame * control_points2[offset].y + (no_frames - (float)frame) * control_points1[offset].y) / (float)no_frames;
			control_points[offset].z = ((float)frame * control_points2[offset].z + (no_frames - (float)frame) * control_points1[offset].z) / (float)no_frames;
		}
	}

}


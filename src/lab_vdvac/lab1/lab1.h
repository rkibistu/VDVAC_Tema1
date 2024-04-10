#pragma once

#include "components/simple_scene.h"
using namespace std;


namespace vdvac {
	typedef struct {
		glm::vec3 p[8]; //coordonatele vârfurilor
		glm::vec3 n[8]; //normalele
		double val[8];  //valorile scalare din vârfuri

	} GRIDCELL;

	typedef struct {
		glm::vec3 p[3]; //varfurile
		glm::vec3 n[3]; //normale
	} TRIANGLE;


	class lab1 : public gfxc::SimpleScene
	{


	public:
		lab1();
		~lab1();

		void Init() override;


		bool loadRAWFile(const string& fileLocation, unsigned int xsize, unsigned int ysize, unsigned int zsize);
		glm::vec3 VertexInterp(double isolevel, glm::vec3 p1, glm::vec3 p2, double valp1, double valp2);
		int PolygoniseCube(GRIDCELL g, double iso, TRIANGLE* tri);
		void reconstructSurface(Mesh* mesh);

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
		unsigned int xsize, ysize, zsize; // dimensiunile volumului de date
		unsigned char* volumeData; //tablou care con?ine valorile scalare din volum
		double isolevel;  //izovaloarea izosuprafe?ei
		TRIANGLE* tri;  //tabloul în care se stocheaz? suprafa?a triunghiular?  
		int offsets[8][3] = {
			{0,0,0},
			{1,0,0},
			{1,0,1},
			{0,0,1},
			{0,1,0},
			{1,1,0},
			{1,1,1},
			{0,1,1}
		};
	};
}
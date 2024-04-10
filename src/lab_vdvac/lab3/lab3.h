#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
using namespace std;

namespace vdvac {

	class lab3 : public gfxc::SimpleScene
	{


	public:
		lab3();
		~lab3();

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
		bool load2DTexturesRAW_X(unsigned int* textures_id, unsigned char* volumeData, unsigned int xsize, unsigned int ysize, unsigned int zsize);
		bool load2DTexturesRAW_Y(unsigned int* textures_id, unsigned char* volumeData, unsigned int xsize, unsigned int ysize, unsigned int zsize);
		bool load2DTexturesRAW_Z(unsigned int* textures_id, unsigned char* volumeData, unsigned int xsize, unsigned int ysize, unsigned int zsize);
		void DesenStivaAxaXNegativa(unsigned int* textures_id, unsigned int num_polig);
		void DesenStivaAxaXPozitiva(unsigned int* textures_id, unsigned int num_polig);
		void DesenStivaAxaYNegativa(unsigned int* textures_id, unsigned int num_polig);
		void DesenStivaAxaYPozitiva(unsigned int* textures_id, unsigned int num_polig);
		void DesenStivaAxaZNegativa(unsigned int* textures_id, unsigned int num_polig);
		void DesenStivaAxaZPozitiva(unsigned int* textures_id, unsigned int num_polig);
		int detMostPerpendicularAxis();
		Mesh* createPoligX();
		Mesh* createPoligY();
		Mesh* createPoligZ();


		unsigned char* volumeData;
		unsigned int xsize, ysize, zsize;

		unsigned int* textures_idX;
		unsigned int* textures_idY;
		unsigned int* textures_idZ;

		float XVIEW, YVIEW, ZVIEW;
	};
}
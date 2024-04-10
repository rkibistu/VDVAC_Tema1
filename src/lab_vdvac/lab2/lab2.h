#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "core/gpu/frame_buffer.h"

using namespace std;

namespace vdvac
{
    class lab2 : public gfxc::SimpleScene
    {
     public:
        lab2();
        ~lab2();

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

        Mesh* createCube(const char* name);
        bool loadRAWFile(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z);
        GLuint createVolumeTexture(const string& fileLocation, unsigned int x, unsigned int y, unsigned int z);
        GLuint createTFTexture(const string& fileLocation);
   
        unsigned char *volumeData;          //volumul de date
		unsigned int xsize, ysize, zsize;   //dimensiunile volumului
		FrameBuffer *frameBuffer;           
		GLuint volumeTexture;               //textura 3D (cu datele din volum)
		GLuint tfTexture;                   //textura 1D (cu functia de transfer)
		float stepSize;                     //pasul de esantionare
    };
}   // namespace vdvac

#ifndef CPAGEHOLDER_H
#define CPAGEHOLDER_H

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif


#include "CViewPort.h"


class CPageHolder
{
    //methods
    public:
        CPageHolder();
        virtual ~CPageHolder();
        bool InitHolder(Uint8 *pImage, size_t nSize);
        const int GetWidth();
        const int GetHeight();
        void MoveViewPort(int dx, int dy);
        void ScaleView(double k, int direction);
        void DrawViewPortToScreen(SDL_Surface* physicalScreen);
        double GetScaler() {return m_dblScale;}
        void SetScaler(double dblScaler){ ScaleViewImpl(dblScaler, 91); } //the number refers to the direction of zoom, default=centre. See main.cpp
        bool LineFeed();

    protected:
    private:
    SDL_Surface* CreateSurface(int width,int height,const SDL_Surface* display);
    void ScaleViewImpl(double dblNewScaler);

    //properties
    private:
      bool bSDLImageInited;
      SDL_Surface* pLoadedImage;
      CViewPort viewPort;
      double m_dblScale;
};

#endif // CPAGEHOLDER_H

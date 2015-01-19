#include "CPageHolder.h"
#include "defines.h"

#ifdef GCWZERO
#include "SDL_rotozoom.h"
//#include "SDL_stretch_.h"
#include<iostream>
#endif
#ifndef GCWZERO
#include "Sprig/sprig.h"
#endif

CPageHolder::CPageHolder():bSDLImageInited(false),pLoadedImage(NULL)
{
    //ctor
}

CPageHolder::~CPageHolder()
{
    //dtor
    if (pLoadedImage)
      SDL_FreeSurface(pLoadedImage);
}

bool CPageHolder::InitHolder(Uint8* pImage, size_t nSize)
{
    m_dblScale=1;
    SDL_Surface* temp_image_surface(NULL);
    //creating SDL_RW
    SDL_RWops *rw=SDL_RWFromMem(pImage, nSize);
    temp_image_surface = IMG_LoadJPG_RW(rw);
    SDL_FreeRW(rw);
	if (temp_image_surface)
	{
	    pLoadedImage = SDL_DisplayFormat(temp_image_surface);
	    SDL_FreeSurface(temp_image_surface);
	    if (!pLoadedImage)
	    {
	        return false;
	    }

	    SDL_Rect vp;
	    vp.x=0;
	    vp.y=0;
	    vp.w=SCR_WIDTH;
	    vp.h=SCR_HEIGHT;
	    viewPort.SetNew(vp);
	    return true;
	}
	else
	{
	    return false;
	}

	return false;

}
const int CPageHolder::GetWidth()
{
    if (pLoadedImage)
       return pLoadedImage->w;

    return 0;
}

const int CPageHolder::GetHeight()
{
    if (pLoadedImage)
       return pLoadedImage->h;

    return 0;
}

void CPageHolder::MoveViewPort(int dx, int dy)
{
    //calculating new viewport coords;
    int newX=viewPort.GetX()+dx*m_dblScale;
    int newY=viewPort.GetY()+dy*m_dblScale;
    if (newX < 0) newX=0;
    if (newY < 0) newY=0;
    if ((viewPort.GetWidth() + newX) > GetWidth())
    {
        //out of screen - recalc x
        newX=GetWidth()-viewPort.GetWidth();
    }
    if ((viewPort.GetHeight() + newY) > GetHeight())
    {
        //out of screen - recalc y
        newY=GetHeight()-viewPort.GetHeight();
    }

    SDL_Rect newRect;
    newRect.x=newX;
    newRect.y=newY;
    newRect.w=viewPort.GetWidth();
    newRect.h=viewPort.GetHeight();
    viewPort.SetNew(newRect);
}
void CPageHolder::ScaleView( double k , int direction)
{
    double dblNewScaler=m_dblScale+k;
    ScaleViewImpl(dblNewScaler, direction);

}
void CPageHolder::ScaleViewImpl(double dblNewScaler, int pandirection)
{


    int newWidth    = ((double)SCR_WIDTH) *dblNewScaler;
    int newHeight   = ((double)SCR_HEIGHT)*dblNewScaler;
    int oldWidth    = ((double)SCR_WIDTH) *(dblNewScaler+0.1);//STEP VALUE, change sign if STEP*BACK
    int oldHeight   = ((double)SCR_HEIGHT)*(dblNewScaler+0.1);
    int differencex = 0.5*(oldWidth-newWidth);
    int differencey = 0.5*(oldHeight-newHeight);
#ifdef GCWZERO
SDL_Rect newRect;

//ZOOMOUT
if(pandirection == 1) { //up
    newRect.x=(viewPort.GetX()-differencex);
    newRect.y=viewPort.GetY();
    newRect.w=newWidth;
    newRect.h=newHeight;
    } else
if(pandirection == 3) { //left
    newRect.x=viewPort.GetX();
    newRect.y=(viewPort.GetY()-differencey);
    newRect.w=newWidth;
    newRect.h=newHeight;
    } else
if(pandirection == 5) { //down
    newRect.x=(viewPort.GetX()-differencex);
    newRect.y=(viewPort.GetY()-(2*differencey));
    newRect.w=newWidth;
    newRect.h=newHeight;
    } else
if(pandirection == 7) { //right
    newRect.x=(viewPort.GetX()-(2*differencex));
    newRect.y=(viewPort.GetY()-differencey);
    newRect.w=newWidth;
    newRect.h=newHeight;
    } else
if(pandirection == 9) { //centre
    newRect.x=(viewPort.GetX()-differencex);
    newRect.y=(viewPort.GetY()-differencey);
    newRect.w=newWidth;
    newRect.h=newHeight;
    } else

//ZOOMIN
if(pandirection == 11) { //up
    newRect.x=(viewPort.GetX()+differencex);
    newRect.y=viewPort.GetY();
    newRect.w=newWidth;
    newRect.h=newHeight;
} else
if(pandirection == 31) { //left
    newRect.x=viewPort.GetX();
    newRect.y=(viewPort.GetY()+differencey);
    newRect.w=newWidth;
    newRect.h=newHeight;
} else
if(pandirection == 51) { //down
    newRect.x=(viewPort.GetX()+differencex);
    newRect.y=(viewPort.GetY()+(2*differencey));
    newRect.w=newWidth;
    newRect.h=newHeight;
} else
if(pandirection == 71) { //right
    newRect.x=(viewPort.GetX()+(2*differencex));
    newRect.y=(viewPort.GetY()+differencey);
    newRect.w=newWidth;
    newRect.h=newHeight;
} else

if(pandirection == 91) { //centre
    newRect.x=(viewPort.GetX()+differencex);
    newRect.y=(viewPort.GetY()+differencey);
    newRect.w=newWidth;
    newRect.h=newHeight;
}
#else
    SDL_Rect newRect;
    newRect.x=viewPort.GetX();
    newRect.y=viewPort.GetY();
    newRect.w=newWidth;
    newRect.h=newHeight;
#endif
    if(newRect.w >= GetWidth())
    {
        newRect.x=0;
    }
    if(newRect.h >= GetHeight())
    {
        newRect.y=0;
    }

//out of screen - recalc x
    if ((newRect.w + newRect.x) > GetWidth())
    {
        newRect.x=GetWidth()-newRect.w;
    }
    if (newRect.x < 0)
    {
        newRect.x=0;
    }


//out of screen - recalc y
    if ((newRect.h + newRect.y) > GetHeight())
    {
        newRect.y=GetHeight()-newRect.h;
    }
    if (newRect.y < 0)
    {
        newRect.y=0;
    }



//width in screen and height in screen and new scaler 1 or more
    if((newRect.w<=GetWidth() && newRect.h<=GetHeight() )
        && (dblNewScaler >= 1))
    {
        viewPort.SetNew(newRect);
        m_dblScale=dblNewScaler;
    }

}

void CPageHolder::DrawViewPortToScreen(SDL_Surface* physicalScreen)
{
    SDL_Rect cnstRect;
    cnstRect.w=SCR_WIDTH;
    cnstRect.h=SCR_HEIGHT;
    cnstRect.x=0;
    cnstRect.y=0;
    //temporary!!!!!! resizing goes here;
    //creating temporary surface


    double zoomX=(double)cnstRect.w/(double)viewPort.GetRectPtr()->w;
    double zoomY=(double)cnstRect.h/(double)viewPort.GetRectPtr()->h;
    //Blit sourceSurface;
    SDL_Surface* source_surface=CreateSurface(viewPort.GetRectPtr()->w,viewPort.GetRectPtr()->h,physicalScreen);
    SDL_BlitSurface(pLoadedImage, viewPort.GetRectPtr(), source_surface,NULL);

#ifdef GCWZERO
// use one of the following (not both!)

/*    SDL_Surface* temp_surface = CreateSurface(SCR_WIDTH,SCR_HEIGHT,physicalScreen);
      SDL_StretchSurfaceRect(source_surface,NULL,temp_surface,NULL);
*/

// or this
    SDL_Surface* temp_surface;
    if(antialiasing)  temp_surface = zoomSurface(source_surface, zoomX, zoomY,1);//antialiasing
    else              temp_surface = zoomSurface(source_surface, zoomX, zoomY,0);//antialiasing disabled -faster zooming

#else
//gcw0  this crashes with SDL_HWSURFACE enabled
    SDL_Surface* temp_surface=SPG_Scale(source_surface,zoomX,zoomY);
#endif

    SDL_BlitSurface(temp_surface,&cnstRect,physicalScreen,&cnstRect);
    SDL_FreeSurface(temp_surface);
    SDL_FreeSurface(source_surface);
}

SDL_Surface* CPageHolder::CreateSurface(int width,int height,const SDL_Surface* display)
{
  // 'display' is the surface whose format you want to match
  //  if this is really the display format, then use the surface returned from SDL_SetVideoMode

  const SDL_PixelFormat& fmt = *(display->format);

#ifdef GCW0
  return SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_DOUBLEBUF,width,height,
                  fmt.BitsPerPixel,
                  fmt.Rmask,fmt.Gmask,fmt.Bmask,fmt.Amask );
#else
  return SDL_CreateRGBSurface(display->flags,width,height,
                  fmt.BitsPerPixel,
                  fmt.Rmask,fmt.Gmask,fmt.Bmask,fmt.Amask );
#endif
}

bool CPageHolder::LineFeed()
{
    bool bRes(false);

    //get current postion and size.
    int nCurrentY = viewPort.GetY();
    int nHeight   = viewPort.GetHeight();
    int nNewY;
    int nCurrentX = viewPort.GetX();
    int nNewX     = 0;
    int nWidth    = viewPort.GetWidth();

#ifdef GCWZERO
    if ((nCurrentX+nWidth)< GetWidth()){
        //move right before going down
        nNewX = nCurrentX + nWidth*0.80;
        if ( (nNewX + nWidth) > GetWidth() ) nNewX = GetWidth() - viewPort.GetWidth();
        if ( (nNewX + nWidth) > ( 0.90 * GetWidth() ) )//snap to right of page (avoids another annoying tiny horizontal pan)
            nNewX=GetWidth()-viewPort.GetWidth();

        SDL_Rect newRect;
        newRect.x=nNewX;
        newRect.y=nCurrentY;
        newRect.w=nWidth;
        newRect.h=nHeight;
        viewPort.SetNew(newRect);

        bRes = true;
    }   else
#endif
    if((nCurrentY + nHeight) < GetHeight())
    {
        int nNewY = nCurrentY + nHeight*0.90;

        if ( (nNewY + nHeight) > GetHeight())
        {
            //out of screen - recalc y
            nNewY=GetHeight()-viewPort.GetHeight();
        }
        if ( (nNewY + nHeight) > ( 0.90 * GetHeight() ) )//snap to bottom of page (avoids another horizontal pan before pagedown)
            nNewY=GetHeight()-viewPort.GetHeight();


        SDL_Rect newRect;
        newRect.x=nNewX;
        newRect.y=nNewY;
        newRect.w=nWidth;
        newRect.h=nHeight;
        viewPort.SetNew(newRect);

        bRes = true;

    } else
    //bottom right of page
    bRes = false;

     return bRes;
}

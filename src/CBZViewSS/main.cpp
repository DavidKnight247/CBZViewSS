#ifdef __cplusplus
    #include <cstdlib>
    #include <iostream>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>

#endif

#include "defines.h"
#include "CPageHolder.h"
#include "unzip.h"
#include "cbzholder.h"

#ifdef GCWZERO
#include <SDL_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <time.h>
#include <string>
#include <iostream>
int antialiasing;
#endif


void Slock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
  }
}

void Sulock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    SDL_UnlockSurface(screen);
  }
}


void Title()
{
    cout << "ComicViewer for small screens v "<<VERSION_NUMBER<<" by F0lken"<<endl;
}

void Usage(const char* executable)
{
    cout << "Usage:"<<endl;
    cout << executable<<" <filename>"<<endl;
}

int main ( int argc, char** argv )
{
    Title();
    if(argc < MINIMUM_ARGC)
    {
        Usage(argv[0]);
        return 1;
    }

    //options parsing should be here
#ifdef GCWZERO
    int infobar = 1;
    antialiasing = 1;
    const char* filename = argv[1];
    static char* tempcbzname;
    static char* cbzname;
    tempcbzname=strrchr(filename, '/');
    cbzname=tempcbzname+1;
    static char strtest[40];
    strncat(strtest, cbzname, 39);

#else
    const char* filename = argv[1];
#endif

    CBZHolder cbzReader;
    CBHolder* pHolder = (CBHolder*)(&cbzReader);

    if ( !(pHolder->InitFile(filename) && pHolder->GetFirstPage()))
    {
        cerr<< "Unable to open file "<<filename<<endl;
        return 1;
    }

    cout<<"Inited file... Starting SDL Loop..."<<endl;


    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        cerr<<"Unable to init SDL: "<<SDL_GetError()<<endl;
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, DEPTH,
                                           SDL_SWSURFACE);
    if ( !screen )
    {
        cerr<<"Unable to set desired video: "<<SDL_GetError()<<endl;
        return 1;
    }

#ifdef mips
  SDL_ShowCursor(SDL_DISABLE);
#endif

    CPageHolder *holder=new CPageHolder;
    bool bInited = holder->InitHolder(pHolder->GetCurrentPage(), pHolder->GetCurrentPageSize());
    pHolder->DeleteCurrentPage();


    if (!bInited)
    {
        delete holder;
        atexit(SDL_Quit);
    }

    int gameRunning = 1;
    bool bNeedRedraw(true);
    bool bFirstDraw=true;
    holder->SetScaler(1);

    int nDelta=SCROLLSTEP;

    SDL_Event event;

 bool bUp=false;
 bool bDown=false;
 bool bLeft=false;
 bool bRight=false;
 bool bZoom=false;
 bool bUnZoom=false;
 bool bPgUp = false;
 bool bPgDown = false;
 bool bLF = false;
 bool bExit1 = false;
 bool bExit2 = false;
 bool bFineScroll =false;
	while (gameRunning) //main loop
	{
        bool bKeyPressed = bUp || bDown || bLeft || bRight || bZoom || bUnZoom || bPgUp || bPgDown ||
                            bLF || bExit1 || bExit2 || bFineScroll;

#ifdef GCWZERO //A-stick controls
        static SDL_Joystick *m_joy = NULL;
        static int joystickmove = 0;
        m_joy = SDL_JoystickOpen(0);
        if(m_joy) {
            SDL_JoystickUpdate();
                if(SDL_JoystickGetAxis(m_joy,1) < -2000) {
                    bUp = true;
                    joystickmove = 1;
                }
                if(SDL_JoystickGetAxis(m_joy,1) > 2000) {
                    bDown = true;
                    joystickmove = 1;
                }
                if(SDL_JoystickGetAxis(m_joy,0) < -2000) {
                    bLeft = true;
                    joystickmove = 1;
                }
                if(SDL_JoystickGetAxis(m_joy,0) > 2000) {
                    bRight = true;
                    joystickmove = 1;
                }
        }

		if (bFirstDraw)
		{

	        bFirstDraw=false;
		    holder->DrawViewPortToScreen(screen);
            if (infobar) {
                boxRGBA(screen, 0,0, 320,11, 250,250,250,128);
                char aux[54];
                time_t rawtime;
                struct tm* time_;
                time(&rawtime);
                time_   = localtime(&rawtime);
                int hours    = time_->tm_hour;
                int minutes = time_->tm_min;
                sprintf(aux, "%02d:%02d pg%-4d%s", hours, minutes, currentpagenumber, strtest);
                TTF_Init();
                TTF_Font *ttffont = NULL;
                SDL_Color text_color = {0,0,0}; //bleck
                ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
                SDL_Surface *textSurface;
                textSurface = TTF_RenderText_Solid(ttffont, aux, text_color);
                SDL_Rect destination;
                destination.x = 1;
                destination.y = 1;
                destination.w = 319;
                destination.h = 20;
                SDL_BlitSurface(textSurface, NULL, screen, &destination);
                SDL_Flip(screen);
                SDL_FreeSurface(textSurface);
       			TTF_CloseFont (ttffont);

            } else SDL_Flip(screen);
	        SDL_PumpEvents();
		    bNeedRedraw=false;
		}

#endif

if(joystickmove) ; else
		if (bKeyPressed ? SDL_PollEvent(&event) : SDL_WaitEvent(&event))
		{


			switch( event.type ){
                    case SDL_KEYDOWN:

					switch( event.key.keysym.sym ){
                    case KEY_LEFT:
                        //holder->MoveViewPort(nDelta*(-1),0);
                        bLeft=true;
                        break;
                    case KEY_RIGHT:
                        //holder->MoveViewPort(nDelta,0);
                        bRight=true;
                        break;
                    case KEY_UP:
                        //holder->MoveViewPort(0,nDelta*(-1) );
                        bUp=true;
                        break;
                    case KEY_DOWN:
                        //holder->MoveViewPort(0,nDelta);
                        bDown=true;
                        break;
                    case KEY_ZOOM:
                        bZoom=true;
                        break;
                    case KEY_UNZOOM:
                        bUnZoom=true;
                        break;

                    case KEY_PGDOWN:
                          bPgDown = true;
                        break;
                    case KEY_PGUP:
                          bPgUp = true;
                        break;
                    case KEY_FOREXIT1:
                          bExit1 = true;
                        break;
                    case KEY_FOREXIT2:
                          bExit2 = true;
                        break;
                    case KEY_LF:
                        bLF = true;
                        break;
                    case KEY_FINESCROLL:
                        bFineScroll=true;
                        break;
                    default:
                        break;
                }


              break;
                 case SDL_KEYUP:
                    bNeedRedraw=true;
					switch( event.key.keysym.sym ){
                    case KEY_LEFT:
                        bLeft=false;
                    case KEY_RIGHT:
                        bRight=false;
                        break;
                    case KEY_UP:
                        bUp=false;
                        break;
                    case KEY_DOWN:
                        bDown=false;
                        break;
                    case KEY_ZOOM:
                        bZoom=false;
                        break;
                    case KEY_UNZOOM:
                        bUnZoom=false;
                        break;
                    case KEY_PGDOWN:
                        bPgDown = false;
                        break;
                    case KEY_PGUP:
                        bPgUp = false;
                        break;
                    case KEY_FOREXIT1:
                          bExit1 = false;
                        break;
                    case KEY_FOREXIT2:
                          bExit1 = false;
                        break;
                    case KEY_LF:
                        bLF = false;
                        break;
                    case KEY_FINESCROLL:
                        bFineScroll=false;
                        break;
                    default:
                        break;
                }


              break;

	          case SDL_QUIT:
               gameRunning = 0;
		      break;

               default:
               break;
             }
		}
		if (bUp)
		{
		    holder->MoveViewPort(0,nDelta*(-1) );
		    bNeedRedraw=true;
		}
		if (bDown)
		{
		    holder->MoveViewPort(0,nDelta);
		    bNeedRedraw=true;
		}
		if(bLeft)
		{
		    holder->MoveViewPort(nDelta*(-1),0);
		    bNeedRedraw=true;
		}

		if(bRight)
		{
		    holder->MoveViewPort(nDelta,0);
		    bNeedRedraw=true;
		}
		if(bZoom)
		{
#ifdef GCWZERO //zoomOUT to centre or in panning direction
            if     (bUp)             holder->ScaleView(STEP, 1);
            else if(bLeft)		     holder->ScaleView(STEP, 3);
            else if(bDown)		     holder->ScaleView(STEP, 5);
            else if(bRight) 		 holder->ScaleView(STEP, 7);
            else 		             holder->ScaleView(STEP, 9); //centre

#else
		    holder->ScaleView(STEP);
#endif
		    bNeedRedraw=true;
#ifdef GCWZERO
		    bZoom=true;
#else
		    bZoom=false;
#endif
		}

		if(bUnZoom)
		{
#ifdef GCWZERO //zoomIN
            if     (bUp)             holder->ScaleView(STEP*BACK, 11);
            else if(bLeft)		     holder->ScaleView(STEP*BACK, 31);
            else if(bDown)		     holder->ScaleView(STEP*BACK, 51);
            else if(bRight) 		 holder->ScaleView(STEP*BACK, 71);
            else 		             holder->ScaleView(STEP*BACK, 91); //centre
#else
		    holder->ScaleView(STEP*BACK);
#endif
		    bNeedRedraw=true;
#ifdef GCWZERO
		    bUnZoom=true;
#else
		    bUnZoom=false;
#endif
		}

		if(bPgDown)
		{
        cout<<"m_nPage"<<currentpagenumber<<endl;

            if (pHolder->GetNextPage())
            {
              double dblScaler=holder->GetScaler();
              if(holder && bInited)
                delete holder;
              holder = new CPageHolder;
              bInited = holder->InitHolder(pHolder->GetCurrentPage(), pHolder->GetCurrentPageSize());
              pHolder->DeleteCurrentPage();
              holder->SetScaler(dblScaler);
              bNeedRedraw = true;
              bPgDown=false;
            }
		}

		if(bPgUp)
		{
            if (pHolder->GetPrevPage())
            {
              double dblScaler=holder->GetScaler();
              if(holder && bInited)
               delete holder;
              holder = new CPageHolder;
              bInited = holder->InitHolder(pHolder->GetCurrentPage(), pHolder->GetCurrentPageSize());
              pHolder->DeleteCurrentPage();
              holder->SetScaler(dblScaler);
              bNeedRedraw = true;
              bPgUp=false;
            }
		}
#ifdef GCWZERO
        if(bExit2) {//menu screen
/*TODO control variables:

infobar
antialiasing
a-stick on/off?
quit
etc...but for now better keep it as quit
*/
gameRunning=0;
bExit1=false;
        }
        if(bExit1) {//select=infobar toggle
            infobar=!infobar;
            bExit1=false;
        }
#else
		if(bExit1 && bExit2)
		{
		    gameRunning = 0;
		    bExit1=false;
		    bExit2=false;

		}
#endif


		(bFineScroll) ? nDelta = SCROLLSTEP/SCROLLSTEP : nDelta = SCROLLSTEP;

		if ( bLF )
		{
		    //calc new pos.
		    bNeedRedraw = holder->LineFeed();
            if(!bNeedRedraw)//LineFeed has reached end of page so goto next page
                bPgDown=true;
//		    bNeedRedraw = true;
		    bLF=false;

		}
#ifdef GCWZERO
if (joystickmove)
{
    joystickmove=0;
    bUp=false;
    bDown=false;
    bLeft=false;
    bRight=false;
}
#endif

		//drawing goes here
		if (bNeedRedraw)
		{
#ifndef GCWZERO
		    Slock(screen);
#endif
		    holder->DrawViewPortToScreen(screen);
#ifdef GCWZERO
        if (infobar) {
                boxRGBA(screen, 0,0, 320,11, 250,250,250,128);
                char aux[54]; //aux=time in display format "hr:mn"
                time_t rawtime;
                struct tm* time_;
                time(&rawtime);
                time_   = localtime(&rawtime);
                int hours  = time_->tm_hour;
                int minutes = time_->tm_min;
                sprintf(aux, "%02d:%02d pg%-4d%s", hours, minutes, currentpagenumber, strtest);
                TTF_Init();
                TTF_Font *ttffont = NULL;
                SDL_Color text_color = {0,0,0}; //bleck
                ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
                SDL_Surface *textSurface;
                textSurface = TTF_RenderText_Solid(ttffont, aux, text_color);
                SDL_Rect destination;
                destination.x = 1;
                destination.y = 1;
                destination.w = 319;
                destination.h = 20;
                SDL_BlitSurface(textSurface, NULL, screen, &destination);
                SDL_Flip(screen);
                SDL_FreeSurface(textSurface);
       			TTF_CloseFont (ttffont);
		} else  SDL_Flip(screen);
#else
            SDL_Flip(screen);
#endif
		    if(bFirstDraw)
		    {
		        bFirstDraw=false;
		        SDL_Flip(screen);
		        SDL_Flip(screen);
		        SDL_PumpEvents();
		        bKeyPressed=true;
		    }
#ifndef GCWZERO
    	    Sulock(screen);
#endif
		    bNeedRedraw=false;
		}

    }

    delete holder;

    // all is well ;)
    atexit(SDL_Quit);
    cout<<"Exited cleanly"<<endl;
    return 0;
}


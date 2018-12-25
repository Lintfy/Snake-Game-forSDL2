
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

void putDot(SDL_Surface *surface,int x,int y,Uint32 pixel)
{
    int bpp=surface->format->BytesPerPixel;
    Uint8 *p=(Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp){
    case 1:
        *p=pixel;
        break;
    case 2:
        *(Uint16 *)p=pixel;
    	break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
            p[0]=(pixel>>16) & 0xff;
            p[1]=(pixel>>8) & 0xff;
            p[2]=pixel & 0xff;
        } else {
            p[0]=pixel & 0xff;
            p[1]=(pixel>>8) & 0xff;
            p[2]=(pixel>>16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p=pixel;
        break;
    }
}

void rectangle(SDL_Surface *surface,int x,int y,int w,int h,Uint32 pixel){
	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			putDot(surface,x+i,y+j,pixel);
		}
	}
}

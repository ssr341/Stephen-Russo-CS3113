// linker error, blurred sprites, what does update do, timeLeftOver error
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ClassDemoApp.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	ClassDemoApp app;

	while (!app.ProcessEvents()){
		app.UpdateAndRender();
	}

	return 0;
}
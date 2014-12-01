#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <string>
#include <vector>

class Instructions{
public:
	void Init();
	GLuint LoadTexture(const char *image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);

	int ProcessEvents(SDL_Event* event, bool& done);
	void Render();
	int Update();
private:
	unsigned int screenText;
};
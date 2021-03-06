#include "ModuleWindow.h"
#include "Application.h"
#include <SDL.h>
#include <SDL_image.h>

#define ICON "Game/icon.png"

ModuleWindow::ModuleWindow() {
}

ModuleWindow::~ModuleWindow() {
}

bool ModuleWindow::Init() {
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		// Create window
		int width = SCREEN_WIDTH_INTRO * SCREEN_SIZE;
		int height = SCREEN_HEIGHT_INTRO * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

		if (FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

		if (window == nullptr) {
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else {
			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			// Set icon
			SDL_Surface *iconSurface = IMG_Load(ICON);
			SDL_SetWindowIcon(window, iconSurface);
			SDL_FreeSurface(iconSurface);
		}
	}

	return ret;
}

bool ModuleWindow::CleanUp() {
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		SDL_FreeSurface(screenSurface);
	}

	// Quit SDL subsystems
	SDL_Quit();
	return true;
}

SDL_Window& ModuleWindow::GetWindow() const {
	return *window;
}

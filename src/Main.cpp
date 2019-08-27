#include <iostream>
#include <string.h>
#include <chrono>
#include <vector>

#include "Utility.hpp"
#include "SpriteRenderer.hpp"
#include "TextRenderer.hpp"

/* Wrapper because *technically* the glad function isn't expecting a _cdecl function */
// This is for the Windows port
#if 0
static void* GetGLProcAddress(const char* path) {
	return SDL_GL_GetProcAddress(path);
}
#endif

struct FrameStatistics {
	size_t numFramesInSecond;
	float average;
	float accum;

	void Update(float delta);
};

struct UiState {
	bool showFps = false;
	bool showCacheTexture = false;
	bool showMessage = false;
	bool showTransparency = false;
	bool showControls = false;
};

void FrameStatistics::Update(float delta) {
	accum += delta;
	numFramesInSecond++;

	while (accum > 1.0f) {
		average = numFramesInSecond / accum;
		accum = fmodf(accum, 1.0f / numFramesInSecond);
		numFramesInSecond = 0;
	}
}

int main(int argc, char** argv) {
	int result;

	result = SDL_Init(SDL_INIT_EVERYTHING);

	if (result < 0) {
		std::cout << "Cannot initialize SDL.\n";
		exit(EXIT_FAILURE);
	}
	 
	result = TTF_Init();

	if (result < 0) {
		std::cout << "Cannot initialize SDL TTF.\n";
		exit(EXIT_FAILURE);
	}

	RenderContextDesc rcDesc = {};
	RenderContext renderContext;
	bool running;

	rcDesc.width  = 512;
	rcDesc.height = 512;
	rcDesc.title = "Text Renderer Example";

	running = CreateRenderContext(rcDesc, renderContext);

	// Now that an OpenGL context was created, tell GLAD to load our OpenGL ES functions
	//gladLoadGLES2Loader(GetGLProcAddress);

	glClearColor(.23f, .23f, .23f, 1.0f);

	// Load our resources
	const size_t MaxSprites = 1024 * 8;
	std::vector<uint8_t> fb;

	Utility::LoadFile("assets/font/Hack-Regular.ttf", fb);
	SpriteRenderer spriteRenderer(renderContext, MaxSprites);
	TextRenderer textRenderer(spriteRenderer, 20, fb.data(), fb.size());


	// Set up our matrices
	Math::Matrix4x4f pm, vm, mvp;
	Math::Identity(pm);
	Math::Identity(vm);
	Math::Identity(mvp);

	Math::BuildOrthoMatrix(pm, (float)rcDesc.width, (float)rcDesc.height, 1.0f, -100.0f);
	
	GLint u_mvp = glGetUniformLocation(spriteRenderer.program, "u_mvp");

	// Buffer to hold a copy of our render calls
	std::vector<RenderCall> calls(MaxSprites);
	size_t numCalls = 0;
	int x = 0;

	auto startTime = std::chrono::high_resolution_clock::now();
	float deltaTime = 0;

	// EXAMPLE: Warm up the text renderer
	/*for (size_t i = 'A'; i < 'z'; i++) {
		textRenderer.AddCharacter(i);
	}*/
	
	UiState uiState;
	FrameStatistics frameStats = {};

	const char* GreetingMessage =
		"Welcome to this OpenGL ES text\n"
		"rendering example. Uses SDL2\n"
		"and SDL2 TTF.";
	const char* ControlsMessage =
		"       *CONTROLS*\n"
		"Show Greeting: 'M'\n"
		"Show Frames Per Second: 'F'\n"
		"Show Text Cache: 'T'\n"
		"Show Controls: 'C'\n"
		"Show Transparency: 'A'\n";

	char fpsBuffer[256] = { 0 };

	// Default state
	uiState.showMessage = true;
	uiState.showCacheTexture = true;
	uiState.showFps = true;
	uiState.showTransparency = true;
	uiState.showControls = true;

	while (running) {
		SDL_Event event = {};
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_m:
					uiState.showMessage = !uiState.showMessage;
					break;
				case SDLK_t:
					uiState.showCacheTexture = !uiState.showCacheTexture;
					break;
				case SDLK_f:
					uiState.showFps = !uiState.showFps;
					break;
				case SDLK_c:
					uiState.showControls = !uiState.showControls;
					break;
				case SDLK_a:
					uiState.showTransparency = !uiState.showTransparency;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		frameStats.Update(deltaTime);

		mvp = pm * vm;
		glUseProgram(spriteRenderer.program);
		glUniformMatrix4fv(u_mvp, 1, GL_FALSE, (const float*)& mvp);

		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		float avgFps = frameStats.average;
		sprintf(fpsBuffer, "%.2f", avgFps);

		// Draw our strings with the text renderer
		if (uiState.showFps) {
			textRenderer.WriteString(
				Math::Vector2f(120, 220),
				Math::Vector3f(1, 1, 0),
				fpsBuffer,
				strlen(fpsBuffer)
			);
		}

		if (uiState.showCacheTexture) {
			Math::Vector4f src, dst;

			src.x = 0;
			src.y = 0;
			src.z = 1;
			src.w = 1;

			dst.x = 128;
			dst.y = -150;
			dst.z = 128;
			dst.w = 128;

			spriteRenderer.PushSprite(
				textRenderer.cacheTexture,
				src,
				dst,
				Math::Vector3f(1, 1, 1)
			);
		}

		if (uiState.showMessage) {
			textRenderer.WriteString(
				Math::Vector2f(-175, -40),
				Math::Vector3f(1, 1, 1),
				GreetingMessage,
				strlen(GreetingMessage)
			);
		}

		if (uiState.showControls) {
			textRenderer.WriteString(
				Math::Vector2f(-220, 125),
				Math::Vector3f(.5, .75, 0),
				ControlsMessage,
				strlen(ControlsMessage)
			);
		}

		if (uiState.showTransparency) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else {
			glDisable(GL_BLEND);
		}

		// Build our command list based off of our previous commands
		spriteRenderer.BuildCommandList(calls.data(), numCalls);

		// Make sure the texture is updated
		textRenderer.UpdateTexture();

		SubmitRenderCalls(renderContext, calls.data(), numCalls);

		SDL_GL_SwapWindow(renderContext.window);

		auto endTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(endTime - startTime).count();
		startTime = endTime;
	}
	
	DestroyRenderContext(renderContext);

	TTF_Quit();
	SDL_Quit();

	return 0;
}
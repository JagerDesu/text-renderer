#pragma once

//#include "gles.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "Math.hpp"

struct TextureDesc {
	size_t width;
	size_t height;
};

struct Texture {
	TextureDesc desc;
	GLuint textureHandle;
};

using TextureHandle = Texture; // Hack

struct SpriteVertex {
	Math::Vector3f a_position;
	Math::Vector3f a_normal;
	Math::Vector3f a_color0;
	Math::Vector2f a_texcoord0;
};

struct RenderContextDesc {
	size_t width;
	size_t height;
	const char* title;
};

struct RenderContext {
	RenderContextDesc desc;
	SDL_GLContext context;
	SDL_Window* window;
};

struct RenderCall {
	GLuint texture;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint program;
	uint16_t indexBase;
	uint16_t numVertices; // Number of vertices to draw
};

bool CreateRenderContext(const RenderContextDesc& desc, RenderContext& renderContext);

void DestroyRenderContext(RenderContext& context);
GLuint CreateGraphicsBuffer(GLenum type, GLenum usage, size_t size, const void* initial);
GLuint CompileShader(RenderContext& renderContext, GLenum shaderType, const void* buffer, size_t bufferSize);
GLuint CreateGraphicsProgram(RenderContext& context, GLuint vertexShader, GLuint fragmentShader);
TextureHandle LoadTexture(const void* buffer, size_t size);
TextureHandle CreateGraphicsTexture(TextureDesc& desc, const void* initial);
void SubmitRenderCalls(RenderContext& context, const RenderCall* renderCalls, size_t numRenderCalls);
#pragma once

#include "RenderContext.hpp"
#include <SDL2/SDL_ttf.h>

struct SpriteRenderer {
	SpriteRenderer(RenderContext& context, const uint16_t MaxSprites);
	~SpriteRenderer();

	void PushSprite(TextureHandle textureHandle, const Math::Vector4f& src, const Math::Vector4f& dst, const Math::Vector3f& color);
	void BuildCommandList(RenderCall* out, size_t& outCount);

	SpriteVertex* vertices;
	uint16_t* indices;

	RenderContext& context;
	RenderCall* renderCalls;
	uint16_t numRenderCalls;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint program;
	uint16_t vbCursor;
	uint16_t ibCursor;
	uint16_t rcCursor;
	uint16_t vbCapacity;
	uint16_t ibCapacity;
};
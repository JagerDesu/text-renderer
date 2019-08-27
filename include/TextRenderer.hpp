#pragma once

#include <cstdint>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include "SpriteRenderer.hpp"

struct TextRenderer {
	TextRenderer(SpriteRenderer& spriteRenderer, size_t fontSize, const void* fontBuffer, size_t size);
	~TextRenderer();
	void AddCharacter(uint32_t c);
	void WriteString(Math::Vector2f position, Math::Vector3f color, const char* message, size_t length);
	void UpdateTexture();

	SpriteRenderer& spriteRenderer;

	TextureHandle cacheTexture;
	SDL_Surface* cacheSurface;
	uint8_t* charScratch; // Scratchpad for flipping characters, is cache surface size to avoid being too small :/

	struct Clip {
		uint16_t x = 0, y = 0, w = 0, h = 0;
	};

	std::unordered_map<uint32_t, Clip> clips;

	size_t fontSize;
	TTF_Font* font;
	uint16_t xOffset;
	uint16_t yOffset;
	uint16_t yMax;

	// Change padding here to prevent bleeding
	static const uint16_t PaddingX = 0;
	static const uint16_t PaddingY = 0;
};

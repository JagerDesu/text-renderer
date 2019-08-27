#include "TextRenderer.hpp"
#include <iostream>

TextRenderer::TextRenderer(SpriteRenderer& spriteRenderer, size_t fontSize, const void* fontBuffer, size_t size) :
	spriteRenderer(spriteRenderer),
	cacheSurface(nullptr),
	cacheTexture({}),
	fontSize(fontSize),
	xOffset(0),
	yOffset(0),
	yMax(0)
{
	cacheSurface = SDL_CreateRGBSurfaceWithFormat(0, 1024, 1024, 32, SDL_PIXELFORMAT_RGBA8888);

	TextureDesc td = {};
	td.width = cacheSurface->w;
	td.height = cacheSurface->h;
	cacheTexture = CreateGraphicsTexture(td, nullptr);
	SDL_RWops* ops = SDL_RWFromConstMem(fontBuffer, size);
	font = TTF_OpenFontRW(ops, SDL_TRUE, fontSize);
	charScratch = new uint8_t[(size_t)cacheSurface->pitch * cacheSurface->h];
}

TextRenderer::~TextRenderer() {
	TTF_CloseFont(font);
	SDL_FreeSurface(cacheSurface);
	delete[] charScratch;
}

void TextRenderer::AddCharacter(uint32_t c) {
	Clip clip;

	SDL_Surface* s = TTF_RenderGlyph_Blended(font, c, SDL_Color{ 0xFF, 0, 0, 0xFF });

	const size_t Pitch = s->pitch;
	const uint8_t* Src = (uint8_t*)s->pixels;

	// Use our pre-allocated buffer as our scratchpad,
	// SDL TTF already does an allocation every time we
	// want to render a Glyph (I think) >.>
	uint8_t* dst = charScratch;

	// Flip the image
	for (size_t i = 0; i < s->h; i++) {
		const uint8_t* SrcLine = Src + (Pitch * (s->h - i - 1));
		memcpy(dst + (Pitch * i), SrcLine, Pitch);
	}

	// Copy over our results 7
	memcpy(s->pixels, dst, Pitch * s->h);

	clip.x = xOffset;
	clip.y = yOffset;
	clip.w = s->w;
	clip.h = s->h;

	xOffset += (s->w + PaddingX);

	// Determine the baseline to move down
	if (s->h > yMax) {
		yMax = s->h;
	}

	// Move right on the texture until we reach the end,
	// then move down
	if (xOffset >= cacheSurface->w) {
		xOffset = 0;
		yOffset += (yMax + PaddingY);
	}

	SDL_Rect dstRect = { clip.x, clip.y, clip.w, clip.h };
	SDL_BlitSurface(s, nullptr, cacheSurface, &dstRect);

	clips[c] = clip;
	SDL_FreeSurface(s);
}

void TextRenderer::WriteString(Math::Vector2f position, Math::Vector3f color, const char* message, size_t length) {
	int x = 0;
	int y = 0;
	for (size_t i = 0; i < length; i++) {
		uint32_t c = (uint32_t)message[i];
		const auto& it = clips.find(c);

		if (c == '\n') {
			y -= yMax;
			x = 0;
			continue;
		}

		if (it == clips.end()) {
			AddCharacter(c);
		}
		auto& clip = clips[c];

		Math::Vector4f src;
		Math::Vector4f dst;

		// Calculate the texture coordinates for the graphics backend
		src.x = (float)clip.x / cacheSurface->w;
		src.y = (float)clip.y / cacheSurface->h;
		src.z = (float)(clip.x + clip.w) / cacheSurface->w;
		src.w = (float)(clip.y + clip.h) / cacheSurface->h;

		// Calculate the destination to render to the screen
		dst.x = position.x + x;
		dst.y = position.y + y;
		dst.z = clip.w;
		dst.w = clip.h;

		spriteRenderer.PushSprite(cacheTexture, src, dst, color);

		x += clip.w + PaddingX;
	}
}

void TextRenderer::UpdateTexture() {
	glBindTexture(GL_TEXTURE_2D, cacheTexture.textureHandle);

	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		(GLsizei)cacheTexture.desc.width,
		(GLsizei)cacheTexture.desc.height,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		cacheSurface->pixels
	);

	glBindTexture(GL_TEXTURE_2D, 0);
}
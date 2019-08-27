
#include "RenderContext.hpp"

#include <vector>
#include <iostream>

bool CreateRenderContext(const RenderContextDesc& desc, RenderContext& renderContext) {
	SDL_Window* window;
	SDL_GLContext context;

	SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
	SDL_SetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER, "none");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow(
		desc.title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(int)desc.width,
		(int)desc.height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	context = SDL_GL_CreateContext(window);

	auto s = SDL_GetError();

	SDL_GL_MakeCurrent(window, context);

	renderContext.desc = desc;
	renderContext.window = window;
	renderContext.context = context;
	return (window && context);
}

void DestroyRenderContext(RenderContext& context) {
	SDL_GL_MakeCurrent(context.window, nullptr);
	SDL_GL_DeleteContext(context.context);
	SDL_DestroyWindow(context.window);
}

GLuint CreateGraphicsBuffer(GLenum type, GLenum usage, size_t size, const void* initial) {
	GLuint bufferHandle = 0;

	glGenBuffers(1, &bufferHandle);
	glBindBuffer(type, bufferHandle);
	glBufferData(type, size, initial, usage);

	return bufferHandle;
}

GLuint CompileShader(RenderContext& renderContext, GLenum shaderType, const void* buffer, size_t bufferSize) {
	(void)renderContext;
	GLuint shaderHandle = 0;
	GLint isCompiled = GL_FALSE;
	const GLchar* source[] = { (const GLchar*)buffer };

	const GLsizei size = (GLsizei)bufferSize;
	shaderHandle = glCreateShader(shaderType);
	glShaderSource(shaderHandle, 1, source, &size);
	glCompileShader(shaderHandle);

	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(shaderHandle);

		if (shaderType == GL_VERTEX_SHADER)
			std::cout << "[VERTEX SHADER]\n";
		else if (shaderType == GL_FRAGMENT_SHADER)
			std::cout << "[FRAGMENT SHADER]\n";
		std::cout << "Cannot compile shader: " << infoLog.data() << "\n";
		return 0;
	}

	return shaderHandle;
}

GLuint CreateGraphicsProgram(RenderContext& context, GLuint vertexShader, GLuint fragmentShader) {
	(void)context;
	GLuint programHandle = glCreateProgram();
	GLint isLinkSuccessful = 0;

	glAttachShader(programHandle, vertexShader);
	glAttachShader(programHandle, fragmentShader);
	glLinkProgram(programHandle);
	glGetProgramiv(programHandle, GL_LINK_STATUS, &isLinkSuccessful);

	if (isLinkSuccessful == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programHandle, maxLength, &maxLength, &infoLog[0]);
		std::cout << "Cannot link program: " << infoLog.data() << "\n";
		glDeleteProgram(programHandle);
		return 0;
	}

	return programHandle;
}

TextureHandle LoadTexture(const void* buffer, size_t size) {
	auto rwops = SDL_RWFromConstMem(buffer, (int)size);
	TextureHandle textureHandle = {};
	SDL_Surface* rawSurface = SDL_LoadBMP_RW(rwops, SDL_TRUE);

	if (!rawSurface) {
		std::cout << "Could not read SDL_Surface: " << SDL_GetError() << "\n";
		return textureHandle;
	}

	SDL_Surface* optimizedSurface = nullptr;

	optimizedSurface = SDL_CreateRGBSurface(
		0,
		rawSurface->w,
		rawSurface->h,
		32,
		0xFF000000,
		0x00FF0000,
		0x0000FF00,
		0x000000FF
	);

	if (!optimizedSurface) {
		std::cout << "Could not allocate RGBA surface: " << SDL_GetError() << "\n";
		if (rawSurface)
			SDL_FreeSurface(rawSurface);
		return textureHandle;
	}

	SDL_BlitSurface(rawSurface, nullptr, optimizedSurface, nullptr);



	TextureDesc td = {};

	td.width = (size_t)optimizedSurface->w;
	td.height = (size_t)optimizedSurface->h;
	textureHandle = CreateGraphicsTexture(td, optimizedSurface->pixels);

	if (rawSurface)
		SDL_FreeSurface(rawSurface);
	if (optimizedSurface)
		SDL_FreeSurface(optimizedSurface);

	return textureHandle;
}

TextureHandle CreateGraphicsTexture(TextureDesc& desc, const void* initial) {
	TextureHandle textureHandle;
	textureHandle.desc = desc;
	glGenTextures(1, &textureHandle.textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle.textureHandle);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		desc.width,
		desc.height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		initial
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureHandle;
}


void SubmitRenderCalls(RenderContext& context, const RenderCall* renderCalls, size_t numRenderCalls) {
	for (size_t i = 0; i < numRenderCalls; i++) {
		auto& rc = renderCalls[i];

		GLuint a_position = glGetAttribLocation(rc.program, "a_position");
		GLuint a_normal = glGetAttribLocation(rc.program, "a_normal");
		GLuint a_color0 = glGetAttribLocation(rc.program, "a_color0");
		GLuint a_texcoord0 = glGetAttribLocation(rc.program, "a_texcoord0");

		glEnableVertexAttribArray(a_position);
		glEnableVertexAttribArray(a_normal);
		glEnableVertexAttribArray(a_color0);
		glEnableVertexAttribArray(a_texcoord0);

		size_t offset = 0;
		glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offset);
		offset += sizeof(Math::Vector3f);
		glVertexAttribPointer(a_normal, 3, GL_FLOAT, GL_TRUE, sizeof(SpriteVertex), (void*)offset);
		offset += sizeof(Math::Vector3f);
		glVertexAttribPointer(a_color0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offset);
		offset += sizeof(Math::Vector3f);
		glVertexAttribPointer(a_texcoord0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offset);
		offset += sizeof(Math::Vector2f);

		glBindBuffer(GL_ARRAY_BUFFER, rc.vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc.indexBuffer);
		glBindTexture(GL_TEXTURE_2D, rc.texture);
		glUseProgram(rc.program);
		glDrawElements(GL_TRIANGLES, rc.numVertices, GL_UNSIGNED_SHORT, (const void*)(size_t)rc.indexBase);
		glDisableVertexAttribArray(a_texcoord0);
		glDisableVertexAttribArray(a_color0);
		glDisableVertexAttribArray(a_normal);
		glDisableVertexAttribArray(a_position);
	}
}

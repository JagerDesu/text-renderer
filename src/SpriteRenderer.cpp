#include "SpriteRenderer.hpp"
#include "Utility.hpp"

SpriteRenderer::SpriteRenderer(RenderContext& context, const uint16_t MaxSprites) :
	context(context),
	vbCapacity(MaxSprites * 4),
	ibCapacity(MaxSprites * 6),
	vertices(nullptr),
	indices(nullptr),
	vbCursor(0),
	ibCursor(0),
	rcCursor(0),
	vertexBuffer(0),
	indexBuffer(0),
	program(0),
	numRenderCalls(0),
	renderCalls(nullptr)
{
	vertices = new SpriteVertex[vbCapacity];
	indices = new uint16_t[ibCapacity];

	vertexBuffer = CreateGraphicsBuffer(
		GL_ARRAY_BUFFER,
		GL_STREAM_DRAW,
		vbCapacity * sizeof(SpriteVertex),
		nullptr
	);

	indexBuffer = CreateGraphicsBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		GL_STREAM_DRAW,
		ibCapacity * sizeof(uint16_t),
		nullptr
	);

	numRenderCalls = MaxSprites;
	renderCalls = new RenderCall[numRenderCalls];

	std::vector<uint8_t> vs, fs;

	Utility::LoadFile("assets/shaders/sprite/sprite-v.glsl", vs);
	Utility::LoadFile("assets/shaders/sprite/sprite-f.glsl", fs);

	GLuint vsh, fsh;
	vsh = CompileShader(context, GL_VERTEX_SHADER, vs.data(), vs.size());
	fsh = CompileShader(context, GL_FRAGMENT_SHADER, fs.data(), fs.size());

	program = CreateGraphicsProgram(context, vsh, fsh);

	glDeleteShader(vsh);
	glDeleteShader(fsh);
}

SpriteRenderer::~SpriteRenderer() {
	delete[] vertices;
	delete[] indices;
	delete[] renderCalls;
}

void SpriteRenderer::PushSprite(TextureHandle textureHandle, const Math::Vector4f& src, const Math::Vector4f& dst, const Math::Vector3f& color) {
	auto& rc = renderCalls[rcCursor++];

	const uint16_t NumVertices = 4;
	const uint16_t NumIndices = 6;

	SpriteVertex nv[NumVertices] = {
		{ Math::Vector3f(0,     0,     0), Math::Vector3f(0, 0, 0), color, Math::Vector2f(src.x, src.y) },
		{ Math::Vector3f(0,     dst.w, 0), Math::Vector3f(0, 0, 0), color, Math::Vector2f(src.x, src.w) },
		{ Math::Vector3f(dst.z, dst.w, 0), Math::Vector3f(0, 0, 0), color, Math::Vector2f(src.z, src.w) },
		{ Math::Vector3f(dst.z, 0,     0), Math::Vector3f(0, 0, 0), color, Math::Vector2f(src.z, src.y) }
	};
	uint16_t ni[NumIndices] = { 0, 1, 2, 2, 0, 3 };

	for (size_t i = 0; i < NumVertices; i++) {
		nv[i].a_position = nv[i].a_position + Math::Vector3f(dst.x, dst.y, 0);
	}

	for (size_t i = 0; i < NumIndices; i++) {
		ni[i] += vbCursor;
	}

	memcpy(&vertices[vbCursor], nv, sizeof(nv));
	memcpy(&indices[ibCursor], ni, sizeof(ni));

	rc.texture = textureHandle.textureHandle;
	rc.vertexBuffer = vertexBuffer;
	rc.indexBuffer = indexBuffer;
	rc.program = program;
	rc.indexBase = ibCursor * sizeof(uint16_t);
	rc.numVertices = NumIndices;

	vbCursor += NumVertices;
	ibCursor += NumIndices;
}

void SpriteRenderer::BuildCommandList(RenderCall* out, size_t& outCount) {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vbCursor * sizeof(SpriteVertex), vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ibCursor * sizeof(uint16_t), indices);
	memcpy(out, renderCalls, rcCursor * sizeof(RenderCall));
	outCount = rcCursor;

	rcCursor = 0;
	vbCursor = 0;
	ibCursor = 0;
}

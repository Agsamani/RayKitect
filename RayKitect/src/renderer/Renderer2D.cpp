#include <rktpch.h>
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Program.h"
#include "RenderCommand.h"
#include "Texture.h"



struct QuadVertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
};

struct Renderer2DData {
	std::shared_ptr<VertexBuffer> QuadVB;
	std::shared_ptr<VertexArray> QuadVA;
	std::shared_ptr<Program> QuadShader;
	std::shared_ptr<Program> TextShader;

	std::shared_ptr<Texture2D> QuadWhiteTexture;

	glm::vec4 QuadVertexPositions[4];

	glm::mat4 CameraMatrix;

};

static Renderer2DData s_Data;


void Renderer2D::Init()
{
	RenderCommand::Init();

	s_Data.QuadVertexPositions[0] = glm::vec4(-0.5, -0.5, 0.0, 1.0);
	s_Data.QuadVertexPositions[1] = glm::vec4(0.5, -0.5, 0.0, 1.0);
	s_Data.QuadVertexPositions[2] = glm::vec4(0.5,  0.5, 0.0, 1.0);
	s_Data.QuadVertexPositions[3] = glm::vec4(-0.5,  0.5, 0.0, 1.0);

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};


	s_Data.QuadVB = std::make_shared<VertexBuffer>(4 * sizeof(QuadVertex));
	s_Data.QuadVA = std::make_shared<VertexArray>();

	BufferLayout layout = {
		{ShaderDataType::Float3, "position"},
		{ShaderDataType::Float4, "color"},
		{ShaderDataType::Float2, "texcoord"}
	};
	s_Data.QuadVB->SetLayout(layout);

	std::shared_ptr<IndexBuffer> IBO = std::make_shared<IndexBuffer>(indices, 6);

	s_Data.QuadVA->SetIndexBuffer(IBO);
	s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

	s_Data.QuadShader = std::make_unique<Program>();
	s_Data.QuadShader->AttachShader(Shader("assets/shaders/engine/Renderer2DQuad.vert", ShaderType::Vertex));
	s_Data.QuadShader->AttachShader(Shader("assets/shaders/engine/Renderer2DQuad.frag", ShaderType::Fragment));
	s_Data.QuadShader->LinkProgram();
	s_Data.QuadShader->UseProgram();

	s_Data.TextShader = std::make_unique<Program>();
	s_Data.TextShader->AttachShader(Shader("assets/shaders/engine/Renderer2DQuad.vert", ShaderType::Vertex));
	s_Data.TextShader->AttachShader(Shader("assets/shaders/engine/Renderer2DText.frag", ShaderType::Fragment));
	s_Data.TextShader->LinkProgram();
	s_Data.TextShader->UseProgram();

	s_Data.QuadWhiteTexture = std::make_shared<Texture2D>(1, 1);
	glm::u8vec4 colorWhite = glm::u8vec4(255);
	s_Data.QuadWhiteTexture->SetData(&colorWhite, 4);
	s_Data.QuadWhiteTexture->BindImage(0);
}

void Renderer2D::Shutdown()
{
		
}

	

void Renderer2D::BeginScene(const OrthographicCamera& ortho)
{
	s_Data.CameraMatrix = ortho.GetViewProjection();
}

void Renderer2D::EndScene()
{

}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	glm::vec2 TexCoords[4] = { {0.0, 0.0},
						{1.0, 0.0},
						{1.0, 1.0},
						{0.0, 1.0} };
	DrawQuad(transform, s_Data.QuadWhiteTexture, TexCoords, color);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2* texCoords, const glm::vec4& tintColor)
{
	QuadVertex Vertices[4];

	for (int i = 0; i < 4; i++)
	{
		Vertices[i].Position = glm::vec3((transform * s_Data.QuadVertexPositions[i]));
		Vertices[i].Color = tintColor;
		Vertices[i].TexCoord = texCoords[i];
	}

	s_Data.QuadVB->SetData(Vertices, sizeof(Vertices));


	s_Data.QuadShader->UseProgram();
	s_Data.QuadShader->UploadUniformInt("u_Texture", 0);
	texture->Bind();
	s_Data.QuadShader->UploadUniformMat4("u_Camera", s_Data.CameraMatrix);

	RenderCommand::DrawIndexed(s_Data.QuadVA, 6);

}
	

#pragma once

#include "Formats.h"
#include "core/Base.h"

struct FramebufferTextureSpec {
	FramebufferTextureSpec() = default;
	FramebufferTextureSpec(TextureDataFormat dataFormat, TextureFilterFormat filterFormat)
		:DataFormat(dataFormat), FilterFormat(filterFormat) {}

	TextureDataFormat DataFormat;
	TextureFilterFormat FilterFormat = TextureFilterFormat::Linear;
};

struct FramebufferAttachmentSpec {
	FramebufferAttachmentSpec() = default;
	FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> attachments)
		:Attachments(attachments) {}

	std::vector<FramebufferTextureSpec> Attachments;
};

struct FramebufferSpec {
	uint32_t Width = 0, Height = 0;
	FramebufferAttachmentSpec Attachments;
	uint32_t Samples = 1;
};

class Framebuffer
{
public:
	Framebuffer(const FramebufferSpec& spec);
	~Framebuffer();

	void Invalidate();

	void Bind() const;
	void Unbind() const;

	void Resize(uint32_t width, uint32_t height);
 	void ReadPixels(uint32_t attachmentIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* pixels);

	void ClearAttachment(uint32_t attachmentIndex, int value) const;

	uint32_t GetColorAttachmentRendererID(int index);
	void BindAttachmentTexture(int index, int slot);

	const FramebufferSpec& GetSpecification() const { return m_Spec; }

private:
	uint32_t m_RendererID;
	FramebufferSpec m_Spec;

	std::vector<FramebufferTextureSpec> m_ColorAttachmentSpecs;
	FramebufferTextureSpec m_DepthAttachmentSpec = { TextureDataFormat::None, TextureFilterFormat::None };

	std::vector<uint32_t> m_ColorAttachmentIDs;
	uint32_t m_DepthAttachmentID;
};


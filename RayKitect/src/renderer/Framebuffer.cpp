#include "rktpch.h"
#include "Framebuffer.h"

#include <glad/glad.h>

static uint32_t s_MaxFramebufferSize = 8192;

namespace Utils {
	static bool IsDepthFormat(TextureDataFormat format)
	{
		switch (format)
		{
		case TextureDataFormat::DEPTH24STENCIL8:  return true;
		}

		return false;
	}

	static GLenum GetInternalFormat(TextureDataFormat DataFormat)
	{
		switch (DataFormat) {
		case TextureDataFormat::RGBA8: return GL_RGBA8;
		case TextureDataFormat::RED_INTEGER: return GL_R32I;
		case TextureDataFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
		}

		RKT_CORE_ASSERT(false, "Unknow texture data format for frame buffer");
	}

	static GLenum GetFormat(TextureDataFormat DataFormat)
	{
		switch (DataFormat) {
		case TextureDataFormat::RGBA8: return GL_RGBA;
		case TextureDataFormat::RED_INTEGER: return GL_RED_INTEGER;
		}

		RKT_CORE_ASSERT(false, "Unknow texture data format for frame buffer");
	}

	static GLenum GetFilter(TextureFilterFormat FilterFormat)
	{
		switch (FilterFormat) {
		case TextureFilterFormat::Linear: return GL_LINEAR;
		case TextureFilterFormat::Nearest: return GL_NEAREST;
		}

		RKT_CORE_ASSERT(false, "Unknow texture filter format for frame buffer");
	}

	static void AttachColorTexture(uint32_t id, int samples, FramebufferTextureSpec spec, uint32_t width, uint32_t height, int index) {
		bool multisample = samples > 1;

		GLenum internalFormat = GetInternalFormat(spec.DataFormat);
		GLenum format = GetFormat(spec.DataFormat);

		if (multisample)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{   //TODO : support mire than GL_UNSIGNED_BYTE and maybe use texStorage2D
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			GLenum filter = GetFilter(spec.FilterFormat);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id, 0);
	}

	static void AttachDepthTexture(uint32_t id, int samples, FramebufferTextureSpec spec, uint32_t width, uint32_t height) {
		bool multisample = samples > 1;

		GLenum internalFormat = GetInternalFormat(spec.DataFormat);

		if (multisample)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{   
			glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);

			GLenum filter = GetFilter(spec.FilterFormat);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT , multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id, 0);
	}


}

Framebuffer::Framebuffer(const FramebufferSpec& spec)
	:m_Spec(spec)
{
	for (auto attachmentSpec : m_Spec.Attachments.Attachments) {
		if (!Utils::IsDepthFormat(attachmentSpec.DataFormat)) {
			m_ColorAttachmentSpecs.push_back(attachmentSpec);
		}
		else {
			m_DepthAttachmentSpec = attachmentSpec;
		}
	}

	Invalidate();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
	glDeleteTextures(1, &m_DepthAttachmentID);
}

void Framebuffer::Invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
		glDeleteTextures(1, &m_DepthAttachmentID);

		m_ColorAttachmentIDs.clear();
		m_DepthAttachmentID = 0;
	}

	glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	bool multisample = m_Spec.Samples > 1;

	if (m_ColorAttachmentSpecs.size())
	{
		m_ColorAttachmentIDs.resize(m_ColorAttachmentSpecs.size());
		glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
			
		for (int i = 0; i < m_ColorAttachmentIDs.size(); i++) {
			glBindTexture(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_ColorAttachmentIDs[i]);
			Utils::AttachColorTexture(m_ColorAttachmentIDs[i], m_Spec.Samples, m_ColorAttachmentSpecs[i], m_Spec.Width, m_Spec.Height, i);
		}
	}

	if (m_DepthAttachmentSpec.DataFormat != TextureDataFormat::None) {
		glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 1, &m_DepthAttachmentID);
		glBindTexture(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_DepthAttachmentID);
		Utils::AttachDepthTexture(m_DepthAttachmentID, m_Spec.Samples, m_DepthAttachmentSpec, m_Spec.Width, m_Spec.Height);
	}

	if (m_ColorAttachmentIDs.size())
	{
		int maxColorAttchments;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttchments);
		RKT_CORE_ASSERT(m_ColorAttachmentIDs.size() <= maxColorAttchments, "Maximum color attachments size exceeded");
		std::vector<GLenum> buffers;
		for (int i = 0; i < maxColorAttchments; i++) {
			buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(m_ColorAttachmentIDs.size(), buffers.data());
	}
	else if (m_ColorAttachmentIDs.empty())
	{
		// Only depth-pass
		glDrawBuffer(GL_NONE);
	}

	RKT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Spec.Width, m_Spec.Height);
}

void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
	{
		RKT_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
		return;
	}
	m_Spec.Width = width;
	m_Spec.Height = height;

	Invalidate();
}

void Framebuffer::ReadPixels(uint32_t attachmentIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* pixels)
{
	RKT_CORE_ASSERT(attachmentIndex < m_ColorAttachmentIDs.size(), "Index must be less than attachment count");

	// TODO : make this proper
	GLenum format = GL_RED;
	GLenum glType = GL_INT;

	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	glReadPixels(x, y, width, height, format, glType, pixels);
}

void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int value) const
{
	RKT_CORE_ASSERT(attachmentIndex < m_ColorAttachmentIDs.size(), "Index must be less than attachment count");

	auto& spec = m_ColorAttachmentSpecs[attachmentIndex];
	glClearTexImage(m_ColorAttachmentIDs[attachmentIndex], 0, Utils::GetInternalFormat(spec.DataFormat), GL_INT, &value);
}

uint32_t Framebuffer::GetColorAttachmentRendererID(int index)
{
	RKT_CORE_ASSERT(index < m_ColorAttachmentIDs.size(), "Index must be less than attachment count");
	return m_ColorAttachmentIDs[index];
}

void Framebuffer::BindAttachmentTexture(int index, int slot)
{
	glBindTextureUnit(slot, GetColorAttachmentRendererID(index));
}

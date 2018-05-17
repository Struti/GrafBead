#include "TextureLoader.h"

#include <algorithm>

#include "stb_image.h"

static GLuint getDataFormat(int channels)
{
	if (channels == 1)
		return GL_RED;
	else if (channels == 2)
		return GL_RG;
	else if (channels == 3)
		return GL_RGB;
	else if (channels == 4)
		return GL_RGBA;
	return 0;
}

ti::TextureLoader::TextureLoader()
{

}


ti::TextureLoader::~TextureLoader()
{

}

ti::Texture ti::TextureLoader::loadTexture(const std::string& relativePath)
{
	return loadTexture(relativePath, true);
}

ti::Texture ti::TextureLoader::loadTexture(const std::string& relativePath, int channels)
{
	return loadTexture(relativePath, channels, true);
}


ti::Texture ti::TextureLoader::loadTexture(const std::string & relativePath, bool repeat)
{
	return loadTexture(relativePath, 3, repeat);
}

ti::Texture ti::TextureLoader::loadTexture(const std::string& relativePath, int channels, bool repeat)
{
	Texture tex;
	if (channels > 4 || channels < 1) return tex;
	unsigned char *imageData = stbi_load(relativePath.c_str(), &tex.width, &tex.height, &tex.channels, channels);
	if (imageData == nullptr)
	{
		throw std::runtime_error("Could not read texture: " + relativePath);
	}

	glGenTextures(1, &tex.texture_id);
	GLint dataFormat = getDataFormat(channels);

	GLuint mode = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;

	glBindTexture(GL_TEXTURE_2D, tex.texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, tex.width, tex.height, 0, dataFormat, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture_cache_.push_back(tex);

	stbi_image_free(imageData);

	tex.type = Texture::SIMPLE;

	return tex;
}

ti::Texture ti::TextureLoader::loadCubemap(const std::string& px, const std::string& nx, const std::string& py, const std::string& ny, const std::string& pz, const std::string& nz)
{
	Texture tex;

	std::string faces[] = { px, nx, py, ny, pz, nz };

	int width, height, channels;
	unsigned char *imageData[6];
	bool failed = false;
	for (int i = 0; i < 6; i++)
	{
		imageData[i] = stbi_load(faces[i].c_str(), &width, &height, &channels, 3);
		failed |= imageData[i] == nullptr;
	}

	if (failed)
	{
		printf("Failed to load sky texture\n");
		for (int i = 0; i < 6; i++)
		{
			stbi_image_free(imageData[i]);
		}
		return tex;
	}

	glGenTextures(1, &tex.texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex.texture_id);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData[i]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	tex.width = width;
	tex.height = height;
	tex.channels = channels;
	tex.type = Texture::CUBEMAP;

	texture_cache_.push_back(tex);

	for (int i = 0; i < 6; i++)
	{
		stbi_image_free(imageData[i]);
	}

	return tex;
}

void ti::TextureLoader::freeTexture(ti::Texture & texture)
{
	auto removeIterator = std::remove_if(texture_cache_.begin(), texture_cache_.end(), 
		[&](const Texture& tex) { return tex.texture_id == texture.texture_id; });
	texture_cache_.erase(removeIterator, texture_cache_.end());
	glDeleteTextures(1, &texture.texture_id);
	texture.texture_id = -1;
}

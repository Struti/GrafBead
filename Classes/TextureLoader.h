#pragma once

#include "GLincludes.h"

#include <string>
#include <vector>

namespace ti
{
	struct Texture
	{
		static const int SIMPLE = 1;
		static const int CUBEMAP = 2;

		int type = SIMPLE;
		int width = -1;
		int height = -1;
		GLuint texture_id = -1;
		int channels = 0;
	};

	class TextureLoader
	{
	public:
		TextureLoader();
		~TextureLoader();

		ti::Texture loadTexture(const std::string& path);
		ti::Texture loadTexture(const std::string& path, int channels);
		ti::Texture loadTexture(const std::string& path, bool repeat);
		ti::Texture loadTexture(const std::string& path, int channels, bool repeat);
		ti::Texture loadCubemap(const std::string& px, const std::string& nx, const std::string& py, const std::string& ny, const std::string& pz, const std::string& nz);
		void freeTexture(ti::Texture& texture);

	private:
		std::vector<Texture> texture_cache_;
	};
}
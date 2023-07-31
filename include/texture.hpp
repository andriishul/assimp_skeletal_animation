#pragma once
#include <string>

struct texture
{
	unsigned ID;
	std::string type, path;
};
texture load_texture2D(std::string const& filename);

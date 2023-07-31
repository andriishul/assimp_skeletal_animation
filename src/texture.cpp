#include "texture.hpp"
#include <glad/glad.h>
#include "opencv2/opencv.hpp"
#include <cassert>
texture load_texture2D(std::string const& filename)
{

	std::string name = filename.substr(0, filename.find_last_of("/"));
	GLenum format = GL_RG;
	cv::Mat image = cv::imread(filename);
	GLuint texture_id;
	system("cls");
	if (image.empty())
	{
		fprintf(stderr, "Image not found %s\n", filename.c_str());
		abort();
	}
	auto numChannels = image.channels();
	if (numChannels == 3)
		format = GL_BGR;
	if (numChannels == 4)
		format = GL_BGRA;


	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, format == GL_BGR ? GL_RGB : format == GL_BGRA ? GL_BGRA : GL_RG, image.cols, image.rows, 0, format, GL_UNSIGNED_BYTE, image.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	image.release();
	
	system("cls");
	assert(glGetError() == GL_NO_ERROR);
	return texture{ texture_id,"",name };
}

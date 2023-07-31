#include "shader/Shader.hpp"
#include <shader/gl/glShader.h>
Shader* make_shader(const char* vsf, const char* fsf)
{
	return new glShader{vsf,fsf};
}

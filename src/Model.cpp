#include <AnimatedModel.hpp>
#include <ModelLoaderHelpers.h>
int GetAnimationCount(std::string const& filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);
	if (scene->HasAnimations())
		return scene->mNumAnimations;
	else return -1;
}

Model* LoadModelFromFile(std::string const& filename)
{
	if (GetAnimationCount(filename) > 0)
		return new AnimatedModel{ filename };
	else return 0;
}

#ifndef MATERIALS_LIBARY_H
#define MATERIALS_LIBARY_H

#include <glm\glm.hpp>

namespace PolyReductor
{
	namespace Renderer
	{
		struct Material
		{
			glm::vec3 ambient, diffuse, specular;
			float shiness;
		};

		enum MaterialType
		{
			EMERALD,
			JADE,
			OBSIBIAN,
			PEARL,
			RUBY,
			TORQOISE,
			BRASS,
			BRONZE,
			CHROME,
			COPPER,
			GOLD,
			SILVER,
			BLACK_PLASTIC,
			CYAN_PLASTIC,
			GREEN_PLASTIC,
			RED_PLASTIC,
			WHITE_PLASTIC,
			YELLOW_PLASTIC,
			BLACK_RUBBER,
			CYAN_RUBBER,
			GREEN_RUBBER,
			RED_RUBBER,
			WHITE_RUBBER,
			YELLOW_RUBBER,
			NUMBER_OF_MATERIALS_TYPE
		};

		Material getMaterial(MaterialType type);


	}
}

#endif // !MATERIALS_LIBARY_H

#include <sky/Camera.h>
#include <sky/Util.h>

namespace sky
{
	namespace cam
	{
		sf::Vector2f center;

		void setCenter(sf::Vector2f target)
		{
			center = target;
		}

		sf::Vector2f getCenter()
		{
			return center;
		}

		void lerpCenter(sf::Vector2f target, float step, float deltaTime)
		{
			center = util::vec::lerp(center, target, step * deltaTime);
		}
	}
}
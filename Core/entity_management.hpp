#include <cstdint>
#include <queue>

using namespace std;
using Entity = uint32_t;


class EntityManager {
public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			entities.push(entity);
		}
	}

	void AddEntity(Entity entity) {
		entities.push(entity);
	}

	void RemoveEntity(Entity entity) {
		
		//entities.
		
	}

private:
	const Entity MAX_ENTITIES = 3000;
	queue<Entity> entities{};


}
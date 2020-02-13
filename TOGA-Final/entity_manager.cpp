#include "entity_manager.h"

EntityManager::EntityManager() {
	nextId = 0;
}

Entity EntityManager::create() {
	Entity id;

	if (!freeEntities.empty()) {
		// If there are ids not being used then use (and remove) the last one
		id = freeEntities.back();
		freeEntities.pop_back();
	}
	else {
		// Otherwise increment the index and set the generation to 1 (0 is destroyed entity)
		id.index = nextId++;
		id.generation = 1;
		generations.push_back(1);
	}

	return id;
}

void EntityManager::destroy(Entity entity) {
	// Increment the generation at the id's index and add to the list of ids that can be reused
	generations[entity.index]++;
	freeEntities.push_back(entity);
}

Entity EntityManager::get(unsigned index) {
	unsigned generation = 0;
	if (index < generations.size()) {
		generation = generations[index];
	}
	// If the entity isn't in the list then return with generation = 0 (destroyed)
	return Entity{ index, 0 };
}

bool EntityManager::isAlive(Entity id) {
	if (id.index < generations.size()) {
		return id.generation > 0 && id.generation == generations[id.index];
	}
	return false;
}

size_t EntityManager::getSize() {
	return generations.size();
}

void EntityManager::clear() {
	generations.clear();
	freeEntities.clear();
	nextId = 0;
}
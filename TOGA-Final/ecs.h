#pragma once

#include "entity_manager.h"
#include "component_manager.h"
#include "system_manager.h"

#include "components.h"

// Static class that acts as a central location to access ECS related functions
class ECS {
public:
	static EntityManager* entityManager;
	static ComponentManager* componentManager;
	static SystemManager* systemManager;

	static Entity createEntity() {
		return entityManager->create();
	}

	static void destroyEntity(Entity entity) {
		if (entityManager->isAlive(entity)) {
			entityManager->destroy(entity);
			componentManager->onEntityDestroyed(entity);
			systemManager->onEntityDestroyed(entity);
		}
	}

	template<typename Component>
	static void registerComponent() {
		componentManager->registerComponent<Component>();
	}

	// Maybe move these into ComponentManager
	template<typename Component>
	static void addComponent(Entity entity, Component component) {
		// Adds the component to the vector of components
		componentManager->addComponent<Component>(entity, component);
		// Sets the bit at the index of the component's unique id to true
		entityManager->getSignature(entity).set(componentManager->getComponentId<Component>(), true);
		// Update systems to add entities if needed
		systemManager->onEntitySignatureChanged(entity, entityManager->getSignature(entity));
	}

	template<typename Component>
	static void removeComponent(Entity entity) {
		componentManager->removeComponent<Component>(entity);
		entityManager->getSignature(entity).set(componentManager->getComponentId<Component>(), false);
		systemManager->onEntitySignatureChanged(entity, entityManager->getSignature(entity));
	}

	template<typename Component>
	static Component& getComponent(Entity entity) {
		return componentManager->getComponent<Component>(entity);
	}

	template<typename Component>
	static int getComponentId() {
		return componentManager->getComponentId<Component>();
	}

	template<typename Component>
	static bool hasComponent(Entity entity) {
		return componentManager->hasComponent<Component>(entity);
	}

	template<typename System>
	static System* registerSystem() {
		return systemManager->registerSystem<System>();
	}
};

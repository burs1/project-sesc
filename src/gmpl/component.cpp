#include "gmpl/component.h"
#include "logger/logger.h"

namespace eng::gmpl {

Component::Component() : entity(entity_) {}

Component::~Component() {
    OnDestroy();
    log::Info("Component with id " + std::to_string(id_) + " destroyed");
}

auto Component::Init(Entity* entity, int id) -> void {
    entity_ = entity;
    id_ = id;

    log::Info("New component created with id " + std::to_string(id_));

    OnCreate();
}

}

#include "gmpl/components/behaviour.h"

namespace eng::gmpl {

class Rotator : public Behaviour {
 public:
  // Sets rotation speed.
  void SetRotationSpeed(math::Vec3&);

 private:
  void OnUpdate() override;

  math::Vec3 rotsp_ = math::Vec3(0.0f, 0.5f, 0.0f);
};

}

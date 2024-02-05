#include "math/vec3.h"
#include "math/matrix4x4.h"
#include "gmpl/components/transform.h"
#include "logger/logger.h"

namespace eng::gmpl {

auto Transform::SetPosition(math::Vec3 new_pos) -> void {
  pos_ = new_pos;
  is_transfmat_updated_ = false;
}


auto Transform::SetRotation(math::Vec3 new_rot) -> void {
  rot_ = new_rot;
  is_rotmat_updated_ = false;
  is_transfmat_updated_ = false;
}


auto Transform::SetScale(math::Vec3 new_scale) -> void {
  scale_ = new_scale;
  is_transfmat_updated_ = false;
}


auto Transform::GetPosition() const -> math::Vec3 {
  return pos_;
}


auto Transform::GetRotation() const -> math::Vec3 {
  return rot_;
}


auto Transform::GetScale() const -> math::Vec3 {
  return scale_;
}


auto Transform::GetTransformMatrix() -> math::Matrix4x4 {
  UpdateTransfmat();
  return transfmat_;
}


auto Transform::Translate(math::Vec3 translation) -> void {
  pos_ += translation;
  is_transfmat_updated_ = false;
}


auto Transform::Rotate(math::Vec3 rotation) -> void {
  rot_ += rotation;
  is_rotmat_updated_ = false;
  is_transfmat_updated_ = false;
}


auto Transform::Rescale(math::Vec3 scaling) -> void {
  scale_ += scaling;
  is_transfmat_updated_ = false;
}


auto Transform::Forward() -> math::Vec3 {
  UpdateRotmat();
  return rotmat_.Forward();
}


auto Transform::Right() -> math::Vec3 {
  UpdateRotmat();
  return rotmat_.Right();
}


auto Transform::Up() -> math::Vec3 {
  UpdateRotmat();
  return rotmat_.Up();
}


auto Transform::UpdateRotmat() -> void {
  if (is_rotmat_updated_) { return; }
  rotmat_.SetRotation(rot_);
  is_rotmat_updated_ = true;
}


auto Transform::UpdateTransfmat() -> void {
  if (is_transfmat_updated_) { return; }
  transfmat_.SetTransform(pos_, rot_, scale_);
  is_transfmat_updated_ = true;
}


}


#include "cinn/lang/compute.h"

#include "cinn/common/common.h"
#include "cinn/poly/dim.h"
#include "cinn/poly/domain.h"
#include "cinn/poly/stage.h"

namespace cinn {
namespace lang {

ir::Tensor Compute(const std::vector<int> &dims,
                   std::function<Expr(Expr)> fn,
                   const std::string &name,
                   int reduce_axis) {
  return Compute(
      dims,
      [fn](const std::vector<Expr> &axis) -> Expr {
        CHECK_EQ(axis.size(), 1);
        return fn(axis[0]);
      },
      name,
      reduce_axis);
}

ir::Tensor Compute(const std::vector<int> &dims,
                   std::function<Expr(Expr, Expr)> fn,
                   const std::string &name,
                   int reduce_axis) {
  return Compute(
      dims,
      [fn](const std::vector<Expr> &axis) -> Expr {
        CHECK_EQ(axis.size(), 2);
        return fn(axis[0], axis[1]);
      },
      name,
      reduce_axis);
}

ir::Tensor Compute(const std::vector<int> &dims,
                   std::function<Expr(Expr, Expr, Expr)> fn,
                   const std::string &name,
                   int reduce_axis) {
  return Compute(
      dims,
      [fn](const std::vector<Expr> &axis) -> Expr {
        CHECK_EQ(axis.size(), 3);
        return fn(axis[0], axis[1], axis[2]);
      },
      name,
      reduce_axis);
}

ir::Tensor Compute(const std::vector<int> &dims,
                   std::function<Expr(Expr, Expr, Expr, Expr)> fn,
                   const std::string &name,
                   int reduce_axis) {
  return Compute(
      dims,
      [fn](const std::vector<Expr> &axis) -> Expr {
        CHECK_EQ(axis.size(), 4);
        return fn(axis[0], axis[1], axis[2], axis[3]);
      },
      name,
      reduce_axis);
}

ir::Tensor Compute(const std::vector<int> &dims,
                   std::function<Expr(Expr, Expr, Expr, Expr, Expr)> fn,
                   const std::string &name,
                   int reduce_axis) {
  return Compute(
      dims,
      [fn](const std::vector<Expr> &axis) -> Expr {
        CHECK_EQ(axis.size(), 5);
        return fn(axis[0], axis[1], axis[2], axis[3], axis[4]);
      },
      name,
      reduce_axis);
}

ir::Tensor Compute(const std::vector<int> &dims,
                   std::function<Expr(const std::vector<Expr> &)> fn,
                   const std::string &name,
                   int reduce_axis) {
  if (reduce_axis != -1) {
    CHECK_GE(reduce_axis, 0);
    CHECK_LT(reduce_axis, dims.size());
  }

  auto axis = common::GenDefaultAxis(dims.size());
  std::vector<Expr> _axis;
  for (auto &x : axis) _axis.push_back(x);
  Expr expr = fn(_axis);

  std::vector<Expr> shape;
  std::vector<Expr> domain;
  for (int i = 0; i < dims.size(); i++) {
    domain.emplace_back(dims[i]);
    // Skip the reduce level.
    if (i != reduce_axis) shape.emplace_back(dims[i]);
  }

  auto unique_name = name.empty() ? Context::Global().NewName("tensor") : name;

  auto op     = ir::ComputeOp::Make(unique_name, "" /*tag*/, {}, fn, domain);
  auto tensor = ir::_Tensor_::Make(unique_name, shape, op);
  if (reduce_axis >= 0) tensor->set_reduce_axis(reduce_axis);
  tensor->axis   = axis;
  tensor->domain = domain;
  return tensor;
}

}  // namespace lang
}  // namespace cinn

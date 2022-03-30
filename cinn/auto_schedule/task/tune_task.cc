// Copyright (c) 2022 CINN Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cinn/auto_schedule/task/tune_task.h"

#include <glog/logging.h>

#include <vector>

#include "cinn/hlir/framework/node.h"
#include "cinn/ir/ir_base.h"
#include "cinn/ir/ir_schedule.h"

namespace cinn {
namespace auto_schedule {

void TuneTask::SetGraphCompiler(hlir::framework::GraphCompiler* compiler) { graph_compiler_ = compiler; }

void TuneTask::TaskGraphToModuleExpr() {
  CHECK(graph_compiler_ != nullptr) << "graph_compiler_ must be set before processing graph";
  std::vector<ir::Expr> exprs = graph_compiler_->FusedGraphToExpr(task_graph_);
  tune_context_.module        = ir::ModuleExpr(exprs);
}

}  // namespace auto_schedule
}  // namespace cinn
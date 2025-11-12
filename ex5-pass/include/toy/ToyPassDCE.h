#pragma once

#include "toy/ToyOps.h"
#include <memory>
#include <mlir/Dialect/Func/IR/FuncOps.h>
#include <mlir/Pass/Pass.h>
#include <mlir/Pass/PassRegistry.h>

namespace toy {

std::unique_ptr<mlir::Pass> createDCEPass();

#define GEN_PASS_DECL
#include "toy/ToyPassDCE.h.inc"

#define GEN_PASS_REGISTRATION
#include "toy/ToyPassDCE.h.inc"

} // namespace toy

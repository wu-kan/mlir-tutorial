#pragma once

#include "toy/ToyOps.h"
#include <memory>
#include <mlir/Dialect/Func/IR/FuncOps.h>
#include <mlir/Pass/Pass.h>
#include <mlir/Pass/PassRegistry.h>

namespace toy {

#define GEN_PASS_DECL
#include "toy/ToyPassCVT.h.inc"

std::unique_ptr<mlir::Pass>
createConvertToyToArithPass(toy::ConvertToyToArithOptions options = {});

#define GEN_PASS_REGISTRATION
#include "toy/ToyPassCVT.h.inc"

} // namespace toy

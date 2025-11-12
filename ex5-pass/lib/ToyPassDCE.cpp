#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/raw_ostream.h>

#define GEN_PASS_DEF_DCE
#include "toy/ToyPassDCE.h"

struct DCEPass : toy::impl::DCEBase<DCEPass> {
  void visitAll(llvm::DenseSet<mlir::Operation *> &visited,
                mlir::Operation *op) {
    if (visited.contains(op))
      return;
    visited.insert(op);
    for (auto operand : op->getOperands())
      if (auto def = operand.getDefiningOp())
        this->visitAll(visited, def);
  }
  void runOnOperation() final {
    llvm::DenseSet<mlir::Operation *> visited;
    this->getOperation()->walk(
        [&](mlir::func::ReturnOp op) { this->visitAll(visited, op); });
    llvm::SmallVector<mlir::Operation *> opToRemove;
    this->getOperation().walk([&](mlir::Operation *op) {
      if (op == this->getOperation())
        return;
      if (!visited.contains(op))
        opToRemove.push_back(op);
    });
    for (auto v : llvm::reverse(opToRemove)) {
      v->erase();
    }
  }
};

std::unique_ptr<mlir::Pass> toy::createDCEPass() {
  return std::make_unique<DCEPass>();
}
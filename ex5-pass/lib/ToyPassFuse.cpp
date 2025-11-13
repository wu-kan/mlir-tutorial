#include <iterator>
#include <mlir/IR/PatternMatch.h>
#include <mlir/Support/LogicalResult.h>
#include <mlir/Transforms/GreedyPatternRewriteDriver.h>

#define GEN_PASS_DEF_FUSE
#include "toy/ToyPassFuse.h"

struct FusePat : mlir::OpRewritePattern<toy::AddOp> {
  using mlir::OpRewritePattern<toy::AddOp>::OpRewritePattern;
  mlir::LogicalResult
  matchAndRewrite(toy::AddOp op, mlir::PatternRewriter &rewriter) const final {
    if (std::distance(op->getUsers().begin(), op->getUsers().end()) != 1)
      return mlir::failure();
    auto op1 = mlir::dyn_cast_if_present<toy::AddOp>(*(op->getUsers().begin()));
    if (op1 == nullptr)
      return mlir::failure();
    auto inputs = llvm::to_vector(op->getOperands()),
         inputs1 = llvm::to_vector(op1->getOperands());
    for (auto &oop : inputs1) {
      if (oop != op)
        inputs.push_back(oop);
    }
    auto result = rewriter.create<toy::AddOp>(
        op->getLoc(), inputs.front().getType(), mlir::ValueRange(inputs));
    rewriter.replaceOp(op1, mlir::ValueRange(result));
    // op->erase();
    return mlir::success();
  }
};

struct FusePass : toy::impl::FuseBase<FusePass> {
  void runOnOperation() final {
    mlir::RewritePatternSet patterns(&this->getContext());
    patterns.add<FusePat>(&this->getContext());
    if (mlir::failed(mlir::applyPatternsGreedily(this->getOperation(),
                                                 std::move(patterns))))
      this->signalPassFailure();
  }
};

std::unique_ptr<mlir::Pass> toy::createFusePass() {
  return std::make_unique<FusePass>();
}

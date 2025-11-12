#include "toy/ToyDialect.h"
#include "toy/ToyOps.h"
#include <llvm/ADT/SmallVector.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/IR/BuiltinDialect.h>
#include <mlir/IR/PatternMatch.h>
#include <mlir/Support/LogicalResult.h>
#include <mlir/Transforms/DialectConversion.h>
#include <mlir/Transforms/GreedyPatternRewriteDriver.h>

#define GEN_PASS_DEF_CONVERTTOYTOARITH
#include "toy/ToyPassCVT.h"

struct AddOpPat : mlir::OpRewritePattern<toy::AddOp> {
  using mlir::OpRewritePattern<toy::AddOp>::OpRewritePattern;
  mlir::LogicalResult
  matchAndRewrite(toy::AddOp op, mlir::PatternRewriter &rewriter) const final {
    auto inputs = llvm::to_vector(op.getInputs());
    auto result = inputs[0];
    for (size_t i = 1; i < inputs.size(); i++) {
      result =
          rewriter.create<mlir::arith::AddIOp>(op->getLoc(), result, inputs[i]);
    }
    rewriter.replaceOp(op, mlir::ValueRange(result));
    return mlir::success();
  }
};

struct SubOpPat : mlir::OpRewritePattern<toy::SubOp> {
  using mlir::OpRewritePattern<toy::SubOp>::OpRewritePattern;
  mlir::LogicalResult
  matchAndRewrite(toy::SubOp op, mlir::PatternRewriter &rewriter) const final {
    rewriter.replaceOpWithNewOp<mlir::arith::SubIOp>(op, op.getLhs(),
                                                     op.getRhs());
    return mlir::success();
  }
};

struct ConstantOpPat : mlir::OpRewritePattern<toy::ConstantOp> {
  using mlir::OpRewritePattern<toy::ConstantOp>::OpRewritePattern;
  mlir::LogicalResult
  matchAndRewrite(toy::ConstantOp op,
                  mlir::PatternRewriter &rewriter) const final {
    rewriter.replaceOpWithNewOp<mlir::arith::ConstantOp>(op, op.getValueAttr());
    return mlir::success();
  }
};

struct ConvertToyToArithPass
    : toy::impl::ConvertToyToArithBase<ConvertToyToArithPass> {
  using toy::impl::ConvertToyToArithBase<
      ConvertToyToArithPass>::ConvertToyToArithBase;
  void getDependentDialects(mlir::DialectRegistry &registry) const final {
    registry.insert<mlir::arith::ArithDialect>();
  }
  void runOnOperation() final {
    mlir::ConversionTarget target(getContext());
    target.addLegalDialect<mlir::arith::ArithDialect>();
    mlir::RewritePatternSet patterns(&getContext());
    patterns.add<AddOpPat, SubOpPat, ConstantOpPat>(&getContext());
    if (mlir::failed(applyPartialConversion(this->getOperation(), target,
                                            std::move(patterns))))
      this->signalPassFailure();
  }
};

std::unique_ptr<mlir::Pass>
toy::createConvertToyToArithPass(toy::ConvertToyToArithOptions options) {
  return std::make_unique<ConvertToyToArithPass>(options);
}
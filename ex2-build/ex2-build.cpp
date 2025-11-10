#include <mlir/Dialect/Func/IR/FuncOps.h>
#include <mlir/Dialect/Tensor/IR/Tensor.h>
#include <mlir/IR/BuiltinOps.h>
#include <mlir/IR/BuiltinTypes.h>
#include <mlir/IR/MLIRContext.h>

int main(int argc, char **argv) {
  mlir::MLIRContext ctx;
  ctx.loadDialect<mlir::func::FuncDialect>();
  ctx.loadDialect<mlir::tensor::TensorDialect>();
  mlir::OpBuilder builder(&ctx);
  auto mod = builder.create<mlir::ModuleOp>(builder.getUnknownLoc());
  builder.setInsertionPointToStart(mod.getBody());

  llvm::SmallVector<int64_t, 2> shapex = {mlir::ShapedType::kDynamic, 3},
                                shapey = {2, 3},
                                shapez = {mlir::ShapedType::kDynamic, 3};
  auto f32 = builder.getF32Type();
  auto tensorTx = mlir::RankedTensorType::get(shapex, f32);
  auto tensorTy = mlir::RankedTensorType::get(shapey, f32);
  auto tensorTz = mlir::RankedTensorType::get(shapez, f32);

  auto funcType = builder.getFunctionType({tensorTx, tensorTy}, {tensorTz});
  auto func = builder.create<mlir::func::FuncOp>(builder.getUnknownLoc(),
                                                 "my_concat", funcType);

  auto entry = func.addEntryBlock();

  builder.setInsertionPointToEnd(entry);

  auto args = func.getArguments();

  auto concat = builder.create<mlir::tensor::ConcatOp>(
      builder.getUnknownLoc(), (int64_t)(0),
      mlir::ValueRange({args[0], args[1]}));

  builder.create<mlir::func::ReturnOp>(builder.getUnknownLoc(),
                                       mlir::ValueRange({concat}));
  mod->dump();
}
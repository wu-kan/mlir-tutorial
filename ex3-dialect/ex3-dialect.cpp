#include "toy/ToyDialect.h"
#include "toy/ToyOps.h"
#include <mlir/Dialect/Func/IR/FuncOps.h>
#include <mlir/Dialect/Tensor/IR/Tensor.h>
#include <mlir/IR/BuiltinOps.h>
#include <mlir/IR/BuiltinTypes.h>
#include <mlir/IR/MLIRContext.h>
#include <mlir/Pass/Pass.h>
#include <mlir/Pass/PassManager.h>
#include <mlir/Transforms/Passes.h>

int main(int argc, char **argv) {
  mlir::MLIRContext ctx;
  ctx.loadDialect<mlir::func::FuncDialect>();
  ctx.loadDialect<toy::ToyDialect>();
  mlir::OpBuilder builder(&ctx);
  auto mod = builder.create<mlir::ModuleOp>(builder.getUnknownLoc());
  do {
    builder.setInsertionPointToStart(mod.getBody());

    auto i32 = builder.getI32Type();
    auto funcType = builder.getFunctionType({i32, i32}, {i32});
    auto func = builder.create<mlir::func::FuncOp>(builder.getUnknownLoc(),
                                                   "test", funcType);
    auto entry = func.addEntryBlock();

    builder.setInsertionPointToEnd(entry);

    auto args = func.getArguments();

    auto command0 = builder.create<toy::SubOp>(
        builder.getUnknownLoc(), i32, mlir::ValueRange({args[0], args[1]}));

    auto command1 = builder.create<toy::AddOp>(
        builder.getUnknownLoc(), i32,
        mlir::ValueRange({command0, args[0], args[1]}));

    auto command2 = builder.create<toy::ConstantOp>(
        builder.getUnknownLoc(), i32, mlir::IntegerAttr::get(i32, 7));

    [[maybe_unused]] auto command3 = builder.create<toy::AddOp>(
        builder.getUnknownLoc(), i32,
        mlir::ValueRange({command0, command1, command2}));

    builder.create<mlir::func::ReturnOp>(builder.getUnknownLoc(),
                                         mlir::ValueRange({command1}));
  } while (0);
  mod->dump();

  do {
    mlir::PassManager pm(&ctx);
    pm.addNestedPass<mlir::func::FuncOp>(mlir::createCanonicalizerPass());
    assert(llvm::succeeded(pm.run(mod)));
  } while (0);

  mod->dump();
}
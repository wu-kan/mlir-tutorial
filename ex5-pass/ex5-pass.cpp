#include "toy/ToyDialect.h"
#include "toy/ToyOps.h"
#include "toy/ToyPassFuse.h"
#include <mlir/Dialect/Func/IR/FuncOps.h>
#include <mlir/IR/MLIRContext.h>
#include <mlir/Parser/Parser.h>
#include <mlir/Pass/Pass.h>
#include <mlir/Pass/PassManager.h>
int main(int argc, char **argv) {
  mlir::MLIRContext ctx;
  ctx.loadDialect<mlir::func::FuncDialect>();
  ctx.loadDialect<toy::ToyDialect>();
  mlir::OpBuilder builder(&ctx);
  llvm::StringRef s = "                                          \n\
module {                                                         \n\
  func.func @test(%arg0: i32, %arg1: i32) -> i32 {               \n\
    %0 = \"toy.sub\"(%arg0, %arg1) : (i32, i32) -> i32           \n\
    %1 = \"toy.add\"(%0, %arg0, %arg1) : (i32, i32, i32) -> i32  \n\
    %2 = \"toy.const\"() <{value = 7 : i32}> : () -> i32         \n\
    %3 = \"toy.add\"(%0, %1, %2) : (i32, i32, i32) -> i32        \n\
    %4 = \"toy.add\"(%arg0, %arg1) : (i32, i32) -> i32           \n\
    %5 = \"toy.add\"(%4, %arg1) : (i32, i32) -> i32              \n\
    %6 = \"toy.add\"(%5, %arg1) : (i32, i32) -> i32              \n\
    return %6 : i32                                              \n\
  }                                                              \n\
}                                                                \n\
";
  auto mod = mlir::parseSourceString<mlir::ModuleOp>(s, &ctx);
  mod->dump();

  do {
    mlir::PassManager pm(&ctx);
    pm.addNestedPass<mlir::func::FuncOp>(toy::createFusePass());
    assert(mlir::succeeded(pm.run(mod.get())));
  } while (0);
  mod->dump();
}
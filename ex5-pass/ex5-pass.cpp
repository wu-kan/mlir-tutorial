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
  llvm::StringRef s = "\
module {\
  func.func @test(%arg0: i32, %arg1: i32) -> i32 {\
    %0 = \"toy.sub\"(%arg0, %arg1) : (i32, i32) -> i32\
    %1 = \"toy.add\"(%0, %arg0, %arg1) : (i32, i32, i32) -> i32\
    %2 = \"toy.const\"() <{value = 7 : i32}> : () -> i32\
    %3 = \"toy.add\"(%0, %1, %2) : (i32, i32, i32) -> i32\
    %4 = \"toy.add\"(%arg0, %arg1) : (i32, i32) -> i32\
    %5 = \"toy.add\"(%4, %arg1) : (i32, i32) -> i32\
    %6 = \"toy.add\"(%5, %arg1) : (i32, i32) -> i32\
    return %6 : i32\
  }\
}\
";
  auto mod = mlir::parseSourceString<mlir::ModuleOp>(s, &ctx);

  do {
    mlir::PassManager pm(&ctx);
    pm.addNestedPass<mlir::func::FuncOp>(toy::createFusePass());
    assert(mlir::succeeded(pm.run(mod.get())));
  } while (0);
  mod->dump();
}
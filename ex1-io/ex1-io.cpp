#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/Dialect/Func/IR/FuncOps.h>
#include <mlir/IR/BuiltinOps.h>
#include <mlir/IR/MLIRContext.h>
#include <mlir/Parser/Parser.h>

int main(int argc, char **argv) {
  mlir::MLIRContext ctx;
  ctx.loadDialect<mlir::arith::ArithDialect>();
  ctx.loadDialect<mlir::func::FuncDialect>();
  auto src = mlir::parseSourceFile<mlir::ModuleOp>(argv[1], &ctx);
  src->dump();
}
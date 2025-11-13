#ifndef TOY_PASSES_TD
#define TOY_PASSES_TD

include "mlir/Pass/PassBase.td"

def Fuse : Pass<"toy-fuse", "mlir::func::FuncOp"> {
  let summary = "fuse";
  let constructor = "toy::createFusePass()";
}

#endif

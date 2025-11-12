#ifndef TOY_PASSES_TD
#define TOY_PASSES_TD

include "mlir/Pass/PassBase.td"

def DCE : Pass<"toy-dce", "mlir::func::FuncOp"> {
  let summary = "dce";
  let constructor = "toy::createDCEPass()";
}

#endif

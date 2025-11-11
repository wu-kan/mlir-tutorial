#include "toy/ToyDialect.h"
#include "toy/ToyOps.h"

#include "toy/ToyDialect.cpp.inc"
#define GET_OP_CLASSES
#include "toy/Toy.cpp.inc"

void toy::ToyDialect::initialize() {
  toy::ToyDialect::addOperations<
#define GET_OP_LIST
#include "toy/Toy.cpp.inc"
      >();
}
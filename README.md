# mlir-tutorial

本仓库为我学习 [KEKE046/mlir-tutorial](https://github.com/KEKE046/mlir-tutorial) 时的 playground，相较原实验保留了我认为核心的部分，使用最少的代码重新实现。同时对一些功能进行修改。

代码结构做了一些调整：编写了 [github workflow](.github/workflows/test.yml)，可以使用系统自带的 mlir 进行开发，无需手动编译；ex5、ex6 只保留 pass 的实现，链接 ex3 的 lib。

## ex2-build

原实验演示了如何生成 arith 方言的 ir。由于 arith 方言比较简单，操作的类型都是标量，与 ex3 中将要使用的 toy 方言几乎一样，这个实验我尝试使用 tensor 方言操作张量，略微复杂一些但是更能表现 mlir 的类型系统。

## ex3-dialect

本节定义了一个方言 toy，几乎与 arith 方言相同，但是 `toy.add` 算子支持超过两个的输入，这使其可以执行类似于 fusion 的优化。

## ex5-pass

原实验为死代码消除，此处改为 fuse 连续的 `toy.add` 算子。相较于死代码消除，我认为这个算子包含了图匹配的过程，更具代表性。

## ex6-pattern

本实验将 toy 转换回 arith，演示了 mlir 里多方言转换的过程。和 ex5 相反，这里将超过两个输入的 `toy.add` 展开成多个只支持两个输入的 `arith.addi`。

# Solver
Simple expression solver. Use it as a library or as a command line tool.

## Build
1. Clone the repository
2. From the cloned repository, create a build directory: `mkdir bld/`
3. From the build directory: `cmake ..`
4. Build the project: `make`
5. Optional: `sudo make install` (on Linux)


# Using as a command line tool

### Example:
```
~user$ solve "3+2"
5, 0x5, %0101
~user$
```

# Using as a library
Look at the `solver.cpp` or `tests/test_expsolver.cpp` files they contain enough information to get going.

```cpp
  double tmp;
  if (ExpSolver::Solve(&tmp, "3+2")) {
    printf("Result: %f\n", tmp);
  }
```

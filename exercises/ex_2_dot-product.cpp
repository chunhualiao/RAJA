//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-19, Lawrence Livermore National Security, LLC.
//
// Produced at the Lawrence Livermore National Laboratory
//
// LLNL-CODE-689114
//
// All rights reserved.
//
// This file is part of RAJA.
//
// For details about use and distribution, please read RAJA/LICENSE.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "memoryManager.hpp"

#include "RAJA/RAJA.hpp"

/*
 *  Vector Dot Product Example
 *
 *  Computes dot = (a,b), where a, b are vectors of 
 *  doubles and dot is a scalar double. It illustrates how RAJA
 *  supports a portable parallel reduction opertion in a way that 
 *  the code looks like it does in a sequential implementation.
 *
 *  RAJA features shown:
 *    - `forall` loop iteration template method
 *    -  Index range segment
 *    -  Execution policies
 *    -  Reduction types
 *
 * If CUDA is enabled, CUDA unified memory is used.
 */


//
//  Function to check dot product result.
//
void checkResult(double compdot, double refdot);

int main(int RAJA_UNUSED_ARG(argc), char **RAJA_UNUSED_ARG(argv[]))
{

  std::cout << "\n\nRAJA vector dot product example...\n";

//
// Define vector length
//
  const int N = 1000000;

//
// Allocate and initialize vector data
//
  int *a = memoryManager::allocate<int>(N);
  int *b = memoryManager::allocate<int>(N);

  for (int i = 0; i < N; ++i) {
    a[i] = 1.0;
    b[i] = 1.0;
  }

//----------------------------------------------------------------------------//

//
// C-style dot product operation.
//
  std::cout << "\n Running C-version of dot product...\n";

  double dot = 0.0;

  for (int i = 0; i < N; ++i) {
    dot += a[i] * b[i];
  }

  std::cout << "\t (a, b) = " << dot << std::endl;

  double dot_ref = dot;

//----------------------------------------------------------------------------//

  std::cout << "\n Running RAJA sequential dot product...\n";

  //TODO: RAJA variant using RAJA::seq_reduce
  
  std::cout << "\t (a, b) = " << dot << std::endl;

  checkResult(dot, dot_ref);


//----------------------------------------------------------------------------//

#if defined(RAJA_ENABLE_OPENMP)
  std::cout << "\n Running RAJA OpenMP dot product...\n";

  //TODO: RAJA variant using RAJA::omp_reduce

  std::cout << "\t (a, b) = " << dot << std::endl;

  checkResult(dot, dot_ref);
#endif


//----------------------------------------------------------------------------//

  memoryManager::deallocate(a);
  memoryManager::deallocate(b);

  std::cout << "\n DONE!...\n";

  return 0;
}

//
//  Function to check computed dot product and report P/F.
//
void checkResult(double compdot, double refdot)
{
  if ( compdot == refdot ) {
    std::cout << "\n\t result -- PASS\n";
  } else {
    std::cout << "\n\t result -- FAIL\n";
  }
}


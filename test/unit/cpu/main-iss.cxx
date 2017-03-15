/*
 * Copyright (c) 2016, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * For release details and restrictions, please see raja/README-license.txt
 */

//
// Main program illustrating simple RAJA index set creation
// and execution and methods.
//

#include <time.h>
#include <cstdlib>

#include <iostream>
#include <string>

#include "RAJA/RAJA.hxx"
#include "RAJA/internal/defines.hxx"

using namespace RAJA;
using namespace std;

//#include "buildIndexSet.hxx"


struct Dumper {

  template<typename T>
  RAJA_INLINE
  void operator()(T const &foo) const {
    std::cout << "MEOWMEOW: " << foo << std::endl;
  }

};

///////////////////////////////////////////////////////////////////////////
//
// Main Program.
//
///////////////////////////////////////////////////////////////////////////

int main(int RAJA_UNUSED_ARG(argc), char** RAJA_UNUSED_ARG(argv))
{
  /*
    //Commenting out because POD is no longer a valid type for indices

  RAJA::BasicIndexSet<int, double> vts;

  vts.push_back(1.4444);

  vts.push_back(8);
  vts.push_back(3.5);
  //vts.push_back("foo");

  vts.push_front(0);

  vts.dumpSegments();

  std::cout<<"before begin()"<<std::endl;
  std::cout<<"begin="<<*(vts.begin())<<std::endl;
  std::cout<<"after begin()"<<std::endl;
  std::cout<<"end="<<*(vts.end())<<std::endl;
  std::cout<<"after end()"<<std::endl;
  std::cout<<"size="<<(vts.size())<<std::endl;
  std::cout<<"after size()"<<std::endl;


  RAJA::BasicIndexSet<int, double> vts2(vts);

  std::cout<<"vts2:"<<std::endl;
  vts2.dumpSegments();

  printf("vts.getNumTypes()=      %d\n", (int)vts.getNumTypes());
  printf("vts.getNumSegments()=   %d\n", (int)vts.getNumSegments());
  printf("vts.isValidSegmentType(int)=   %d\n", (int)vts.isValidSegmentType(3));
  printf("vts.isValidSegmentType(char*)=   %d\n", (int)vts.isValidSegmentType("meow"));


  RAJA::BasicIndexSet<int, double, std::string> vts3;
  RAJA::BasicIndexSet<int, double, std::string> vts4;

  // we can do this beause vts3 is a super-set of vts2
  vts2.push_into(vts3, PUSH_BACK);
  std::cout<<"vts3:"<<std::endl;
  vts3.dumpSegments();
  vts2.push_into(vts4, PUSH_FRONT);
  std::cout<<"vts4:"<<std::endl;
  vts4.dumpSegments();
  */

/*
  vts3.push_front(std::string("meow"));
  vts3.push_back(std::string("woof"));
  vts3.dumpSegments();

  vts3.segmentCall(0, Dumper());
  vts3.segmentCall(1, Dumper());
  vts3.segmentCall(2, Dumper());
*/

  //RAJA::IndexSet is0;
  RAJA::BasicIndexSet<RAJA::RangeSegment, RAJA::ListSegment> is0;

  std::vector<int> segvec{3,2,1};
  is0.push_back(RAJA::ListSegment(segvec));

  is0.push_back(RAJA::RangeSegment(0,5));
  is0.push_back(RAJA::RangeSegment(-5,0));

  cout << "\n\nIndexSet( master ) " << endl;
  is0.print(cout);


  RAJA::BasicIndexSet<RAJA::RangeSegment, RAJA::ListSegment> is1;
  int num_segments = is0.getNumSegments();

  for (int i = 0; i < num_segments; ++i) {
    is0.segment_push_into(i, is1, PUSH_BACK, PUSH_COPY);
  }


  //for (int i = num_segments-1; i >= 0; --i) {
  //  is0.segment_push_into(i, is1, PUSH_FRONT, PUSH_COPY);
 // }

  RAJA::RangeSegment r_segment(0,5);
  if (!is0.isValidSegmentType(r_segment)) {
    std::cout<<"RangeSegment isValidSegmentType should have returned true"<<std::endl;
  }
  RAJA::RangeStrideSegment rs_segment(0, 4, 2);
  if (is0.isValidSegmentType(rs_segment)) {
    std::cout<<"RangeStrideSegment isValidSegmentType should have returned false"<<std::endl;
  }

  cout << "\n\nIndexSet( copy ) " << endl;
  is1.print(cout);


  if (is1 == is0) { } //std::cout<<"first copy correct"<<std::endl; }
  else { std::cout<<"Copy not correct"<<std::endl; }

  if (is1 != is0) { std::cout<<"!= operator is not correct"<<std::endl; }

  int view_size = num_segments - 1;
  std::cout<<"creating a view of size "<<view_size<<std::endl;

  //RAJA::BasicIndexSet<RAJA::RangeSegment, RAJA::ListSegment>* iset_view
  //  = is1.createView<RAJA::RangeSegment, RAJA::ListSegment>(0, view_size);
  RAJA::BasicIndexSet<RAJA::RangeSegment, RAJA::ListSegment>* iset_view
    = is1.createView(0, view_size);

  cout << "\n\nIndexSet( view ) " << endl;
  iset_view->print(cout);

  //is0.dumpSegments();

  using omp_seq_pol = RAJA::IndexSet::ExecPolicy<RAJA::omp_parallel_for_segit, RAJA::seq_exec>;
  using seq_seq_pol = RAJA::IndexSet::ExecPolicy<RAJA::seq_segit, RAJA::seq_exec>;

  RAJA::forall<seq_seq_pol>(is0, [=](int i){printf("body(%d)\n",i);});

//  //RAJA::forall<omp_seq_pol>(is0, [=](int i){printf("body(%d)\n",i);});


  cout << "\n DONE!!! " << endl;

  return 0;
}

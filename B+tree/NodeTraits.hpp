#include "B+Node.hpp"

template <typename Node>
struct NodeTraits{
  static const int  flag_type = 0;
};

template <typename T, int S>
struct NodeTraits< BPNode <T, S> >{
  static const int  flag_type = B_PLUS_NODE_FLAGXX;
};
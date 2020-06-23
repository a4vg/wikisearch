#include "NodeTraits.hpp"
#include "disk_manager.hpp"
#include <memory>

typedef typename std::shared_ptr<DiskManager> diskManager;

struct Data {
  long root_id{1};
  long total{0};
};


template<typename  Node, int Flag>
struct TreeHelper{
  typedef Node  node_t;
  typedef typename node_t::value_t value_t;
  
  static void  insert (node_t** head, const value_t& val){
    std::cout << "el tipo de dato no es compatible" << std::endl;
  }

  static void  print (node_t** head){
  }

  static void search (node_t ** head, const value_t& val){

  }

};

//B+Tree
template<typename T, int S>
struct TreeHelper<BPNode<T, S>,B_PLUS_NODE_FLAGXX>{
    typedef BPNode<T, S>  node_t;
    typedef typename node_t::value_t value_t;

    enum state { OVERFLOW, NORMAL}; 
    
    static void insert (const value_t& val, Data &header, diskManager &dm){
      std::cout << "Inserting " << val << std::endl;
    }

    static void print (Data &header, diskManager &dm){
      std::cout << "\nPrint B+ Tree\n";
    }

    static void search (const value_t &val, Data &header, diskManager &dm){
      std::cout << "Searching in B+tree\n";
    }

};
#include "disk_manager.hpp"
#include "Tree.hpp"
#include "iostream"
#include <memory>


int main (){
  typedef BPNode<int, 4> bpnode;
  typedef Tree<bpnode, true> bptree;

  std::cout<<"-----------------------------------------------------\n";

  std::shared_ptr<DiskManager> pmi = std::make_shared<DiskManager>("btree.index", true);
  bptree tree3(pmi);

  for (int i = 20; i <=150; i+=10){
    tree3.insert(i);
    tree3.print();
  }

  for (int i = 10; i <=100; i+=5){
    std::cout << i << "\n";
    tree3.search(i);
  }
  
}

// g++ --std=c++11 -I../include b+tree_example.cpp

#include "disk_manager.hpp"
#include "Tree.hpp"
#include "iostream"
#include <memory>
#include "string.h"
#include <vector>


int main (){
  typedef BPNode<Cadena, 4> bpnode;
  typedef Tree<bpnode, true> bptree;

  std::cout<<"-----------------------------------------------------\n";

  std::shared_ptr<DiskManager> pmi = std::make_shared<DiskManager>("btree.index", true);
  bptree tree3(pmi);

  std::cout <<"INSERTION\n\n";
  std::vector <std::string> array = {"False","None","True","and","as","assert","break","class","continue",
  "def","del","elif","else","except","finally","for","from","global","if","import","in",
  "is","lambda","nonlocal","not","or","pass","raise","return","try","while","with","yield"};

  for (int i = 0; i < array.size() ; i+=1){
    char * w = (char*)array[i].c_str();
    Cadena word(w);
    tree3.insert(word, i + 1);
    tree3.print();
  }

  std::vector <std::string> array2 = {"hola", "adios", "return", "and", "true", "True"};

  std::cout <<"SEARCHING\n\n";
  for (int i = 0; i < 6; i+=1){
    char * w = (char*)array2[i].c_str();
    Cadena word(w);
    std::cout << word.get() << "\n";
    tree3.search(word);
  }
  
}
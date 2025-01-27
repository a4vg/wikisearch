#include "TreeHelper.hpp"
#include "disk_manager.hpp"
#include <memory>

template <typename Node, bool is_disk>
class Tree {

};

template <typename Node>
class Tree <Node, true> {
  private:
    typedef typename std::shared_ptr<DiskManager> diskManager;

    Data data;

    diskManager dm;
  
  public:
  typedef Node node_t;
  typedef typename node_t::value_t value_t;

  node_t * root;

  
  Tree (void) : root(nullptr){}
  Tree (diskManager _dm) {
    dm = _dm;
    if(dm->is_empty()){
      node_t root (data.root_id);
      dm->write_record (data.root_id, root);
      data.total++;
      dm->write_record(0, data);
    } else {
      dm->retrieve_record (0, data);
    }
  }

  ~Tree(void){}

  int insert(const value_t& val, long idpage){
    return add<NodeTraits<node_t>::flag_type>(val, idpage, data, dm);
  }

  void print(){
    TreeHelper <node_t, NodeTraits<node_t>::flag_type>::print (data, dm);
  }

  int search (const value_t& val){
    return TreeHelper <node_t, NodeTraits<node_t>::flag_type>::search (val, data, dm);
  }


  template <int Flag> 
  int add(const value_t&, long, Data&, diskManager&);

};

template<typename Node> template <int Flag>
int Tree<Node, true>::add(
  const typename Tree<Node, true>::value_t & val, long idpage,
  Data &data, diskManager &dm){
  return TreeHelper<Tree<Node, true>::node_t,Flag>::insert(val, idpage, data, dm);
}


template <typename Node>
class Tree <Node, false> {
  
  public:
  typedef Node node_t;
  typedef typename node_t::value_t value_t;

  node_t * root;

  
  Tree (void) : root(nullptr){}

  ~Tree(void){}

  void insert(const value_t& val){
    add<NodeTraits<node_t>::flag_type>(&root,val);
  }

  void print(){
    TreeHelper <node_t, NodeTraits<node_t>::flag_type>::print (&root);
  }

  int search (const value_t& val){
    return TreeHelper <node_t, NodeTraits<node_t>::flag_type>::search (&root, val);
  }

  template <int Flag> 
  void add(node_t**, const value_t&);

};

template<typename Node> template <int Flag>
void Tree<Node, false>::add(
  typename Tree<Node, false>::node_t ** root, 
  const typename Tree<Node, false>::value_t & val){
  TreeHelper<Tree<Node, false>::node_t,Flag>::insert(root,val);
}

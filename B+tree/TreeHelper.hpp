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

    static node_t readNode (long disk_id, diskManager &dm){
      node_t new_node(-1);
      dm->retrieve_record(disk_id, new_node);
      return new_node;
    }

    static void writeNode (long disk_id, node_t node, diskManager &dm){
      dm->write_record(disk_id, node);
    }

    static void splitNode (node_t &parent_node, const int pos, Data& header, diskManager &dm){
        node_t ptr_node = readNode (parent_node.children[pos], dm);

        node_t left_node (parent_node.children[pos], ptr_node.is_leaf);

        header.total++;
        dm->write_record(0, header);
        node_t right_node (header.total, ptr_node.is_leaf);

        int iter_child;
        int iter_keys = 0;
        for (iter_child = 0; iter_keys < S / 2; iter_child++) {
            left_node.children[iter_child] = ptr_node.children[iter_keys];
            left_node.keys[iter_child] = ptr_node.keys[iter_keys];
            left_node.size++;
            iter_keys++;
        }
        left_node.children[iter_child] = ptr_node.children[iter_keys];

        iter_child = 0;

        if (ptr_node.is_leaf) {
            right_node.children[iter_child] = ptr_node.children[iter_keys];
            right_node.keys[iter_child] = ptr_node.keys[iter_keys];
            right_node.size++;
            iter_child++;

            left_node.next_node = right_node.disk_id;
            left_node.prev_node = ptr_node.prev_node;
            right_node.prev_node = left_node.disk_id;
            right_node.next_node = ptr_node.next_node;

            if (right_node.next_node != -1){
                node_t temp = readNode(right_node.next_node, dm);
                temp.prev_node = right_node.disk_id;
                writeNode(temp.disk_id, temp, dm);
            }

        }

        for(int i = parent_node.size; i > pos; i--){
          parent_node.keys[i] = parent_node.keys[i - 1];
          parent_node.children[i + 1] = parent_node.children[i];
        }
        parent_node.keys[pos] = ptr_node.keys[iter_keys];
        parent_node.children[pos + 1] = parent_node.children [pos];
        parent_node.size += 1;


        iter_keys++;

        for (; iter_keys < S + 1; iter_child++) {
            right_node.children[iter_child] = ptr_node.children[iter_keys];
            right_node.keys[iter_child] = ptr_node.keys[iter_keys];
            right_node.size++;

            iter_keys++;
        }
        right_node.children[iter_child] = ptr_node.children[iter_keys];
        parent_node.children[pos] = left_node.disk_id;
        parent_node.children[pos + 1] = right_node.disk_id;
        parent_node.is_leaf = false;


        writeNode(parent_node.disk_id, parent_node, dm);
        writeNode(left_node.disk_id, left_node, dm);
        writeNode(right_node.disk_id, right_node, dm);

    }

    static void splitRoot(Data& header, diskManager dm) {
        node_t ptr_node = readNode(header.root_id, dm);

        header.total++;
        dm->write_record(0, header);
        node_t left_node (header.total, ptr_node.is_leaf);

        header.total++;
        dm->write_record(0, header);
        node_t right_node (header.total, ptr_node.is_leaf);

        int iter_child; //for child
        int iter_keys = 0;//for keys
        int pos = 0;

        for (iter_child = 0; iter_keys < S / 2; iter_child++) { //copy to left node
            left_node.children[iter_child] = ptr_node.children[iter_keys];
            left_node.keys[iter_child] = ptr_node.keys[iter_keys];
            left_node.size++;
            iter_keys++;
        }
        left_node.children[iter_child] = ptr_node.children[iter_keys];

        iter_child = 0;

        if (ptr_node.is_leaf){ //if is the first split of the root node
            right_node.children[iter_child] = ptr_node.children[iter_keys];
            right_node.keys[iter_child] = ptr_node.keys[iter_keys]; //left base split
            right_node.size++;
            iter_child++;
        }

        iter_keys++;

        for (; iter_keys < S + 1; iter_child++) {  //copy to right node
            right_node.children[iter_child] = ptr_node.children[iter_keys];
            right_node.keys[iter_child] = ptr_node.keys[iter_keys];
            right_node.size++;
            iter_keys++;
        }

        right_node.children[iter_child] = ptr_node.children[iter_keys];

        if (ptr_node.is_leaf){ //link the children nodes if is the first split
            left_node.next_node = right_node.disk_id;
            right_node.prev_node = left_node.disk_id;
        }


        //update children array
        ptr_node.children[pos] = left_node.disk_id;

        ptr_node.keys[0] = ptr_node.keys[S / 2];
        ptr_node.children[pos + 1] = right_node.disk_id;
        ptr_node.size = 1;
        ptr_node.is_leaf = false;

        writeNode(ptr_node.disk_id, ptr_node, dm);
        writeNode(left_node.disk_id, left_node, dm);
        writeNode(right_node.disk_id, right_node, dm);
      
    }

    

    static int insert (node_t &node, const value_t& val, long idpage, Data &header, diskManager &dm){
        int pos = 0;
        while (pos < node.size && node.keys[pos] < val)
            pos++;
        
        if (node.is_leaf){
            for(int i = node.size; i > pos; i--){
                node.keys[i] = node.keys[i - 1];
                node.children[i] = node.children[i - 1];
            }
            node.keys[pos] = val;
            node.children[pos] = idpage;
            node.size += 1;

            writeNode (node.disk_id, node, dm);
        } else {
            long page_id = node.children[pos];
            node_t child = readNode(page_id, dm);
            int state = insert(child, val, idpage, header, dm);
            if (state == OVERFLOW){
                splitNode(node, pos, header, dm);
            }
        }
        
        return node.isOverflow() ? OVERFLOW : NORMAL;
    }

    static void insert (const value_t& val, long idpage, Data &header, diskManager &dm){
      std::cout << "Inserting " << val << std::endl;
      node_t root = readNode (header.root_id, dm);
      int state = insert(root, val, idpage, header, dm);
      if (state == OVERFLOW){
        splitRoot (header, dm);
      }
    }

    static void print (Data &header, diskManager &dm){
      std::cout << "\nPrint B+ Tree\n";
      node_t temp = readNode(header.root_id, dm);
      while (!temp.is_leaf)
        temp = readNode(temp.children[0], dm);
      
      while (temp.next_node > 0){
        std::cout << "Node: ";
        for (int i = 0; i < temp.size; i++)
          std::cout << temp.keys[i] << ":" << temp.children[i] << " - ";
        temp = readNode(temp.next_node, dm);
      }
      std::cout << "Node: ";
      for (int i = 0; i < temp.size; i++)
        std::cout << temp.keys[i] << ":" << temp.children[i] << " - ";
      std::cout<<"\n";
    }

    static int search (node_t &ptr, const T &val, diskManager &dm){
      int pos = 0;
      while (pos < ptr.size && ptr.keys[pos] <= val)
        pos++;

      if (!ptr.is_leaf){
        long page_id = ptr.children [pos];
        node_t child = readNode (page_id, dm);
        return search (child, val, dm);
      } else {
        if (ptr.keys [pos - 1] != val){
          return -1;
        }else {
          return ptr.children [pos - 1];
        }
      }
    }

    static void search (const value_t &val, Data &header, diskManager &dm){
      std::cout << "Searching in B+tree\n";
      node_t root = readNode(header.root_id, dm);
      int res = search (root, val, dm);
      if (res == -1)
          std::cout << "Not found\n";
      else
          std::cout << "Found: " << res << "\n";
    }

};
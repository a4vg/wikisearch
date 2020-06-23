#include <iostream>
#include <vector>

#define B_PLUS_NODE_FLAGXX 3

template <typename T, int S>
class BPNode {
    public:
    typedef T value_t;
    typedef BPNode <T, S> node_t;
    
    std::size_t order;
    std::size_t size;
    bool is_leaf;
    long disk_id;
    long children [S + 2];
    T keys [S + 1];
    long next_node = -1;
    long prev_node = -1;

    BPNode(long page, bool is_leaf):order(S), size(0),disk_id(page), is_leaf(is_leaf) {
        for (int i = 0; i < S + 2; i++)
            children[i] = 0;
    }

    BPNode(long page):order(S), size(0),disk_id(page), is_leaf(true) {
        for (int i = 0; i < S + 2; i++)
            children[i] = 0;
    }

    bool isOverflow(){
        return size > order;
    }

    ~BPNode(void){}

};

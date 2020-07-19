#include <iostream>
#include <vector>
#include "string.h"

#define B_PLUS_NODE_FLAGXX 3

class Cadena
{    char cad[40];
   public:
     Cadena(char *);
     Cadena();
     char * get(void);
     void operator + (Cadena);
     Cadena &operator= (const Cadena&);
     bool operator ==(const Cadena&);
     bool operator !=(const Cadena&);
     bool operator <(const Cadena&);
     bool operator <=(const Cadena&);
     friend std::ostream& operator<<(std::ostream&, const Cadena&);
};

Cadena::Cadena(char *c)
{    strcpy(cad,c);
}

Cadena::Cadena()
{    strcpy(cad,"");
}

char *Cadena::get(void)
{    return cad; 
}

Cadena &Cadena::operator=(const Cadena &c) {
    if(this != &c) {
        strncpy(cad, c.cad, sizeof(cad));
    }
    return *this;
}

bool Cadena::operator== (const Cadena &c){
    if (strncmp (cad, c.cad, sizeof(cad)) == 0)
        return true;
    return false;
}

bool Cadena::operator!= (const Cadena &c){
    if (strncmp (cad, c.cad, sizeof(cad)) != 0)
        return true;
    return false;
}

bool Cadena::operator< (const Cadena &c){
    if (strncmp (cad, c.cad, sizeof(cad)) < 0)
        return true;
    return false;
}

bool Cadena::operator<= (const Cadena &c){
    if (strncmp (cad, c.cad, sizeof(cad)) <= 0)
        return true;
    return false;
}

std::ostream& operator<<(std::ostream& os, const Cadena& c)
{
    os << c.cad;
    return os;
}

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

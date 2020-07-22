#ifndef BPNODECPP
#define BPNODECPP

#include "B+Node.hpp"

Cadena::Cadena(char *c)
{
    strncpy(cad, c, (sizeof cad) - 1);
    cad[39] = '\0';
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

#endif
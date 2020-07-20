#pragma once
#include <cstdlib>
#include<fstream>
#include<iostream>
#include<string>

class DiskManager : protected std::fstream{

  std::string filePath;
  bool empty;

  public:

    DiskManager(){ };

      DiskManager(std::string fp, bool reset = false):std::fstream(fp.data(),std::ios::in | std::ios::out | std::ios::binary){
          filePath = fp;
          empty=false;
        if(!good() || reset){ 
          empty=true;
          open(filePath.data(),std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc );
        }
      }

      ~DiskManager(){ close();} 

      template<typename Record>
      void write_record(const long &n, Record &reg){
        clear();
        seekp(n*sizeof(Record),std::ios::beg);
        write(reinterpret_cast<const char*>(&reg),sizeof(reg));
      }

      template<typename Record>
      long write_record_toending(Record &reg){
        clear();
        seekp(0,std::ios::end);
        long pos=tellp();
        write(reinterpret_cast<const char*>(&reg),sizeof(reg));
        pos/=sizeof(reg);
        return pos;
      }

      template<typename Record>
      bool retrieve_record(const long &n, Record &reg){
        clear();
        seekg(n*sizeof(Record),std::ios::beg);
        read(reinterpret_cast<char *>(&reg),sizeof(reg));
        return gcount() > 0; 
      }

      inline bool is_empty(){ return empty;}

      int get_size(){
        clear ();
        seekg (0, std::ios::end);
        return tellg ();
      }
    };

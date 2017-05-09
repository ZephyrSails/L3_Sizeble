// by: Zhiping

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <tuple>
#include <unistd.h>
#include <typeinfo>

namespace L3 {

  void debug(std::string str);
  // const std::vector< std::string > sys

  // class Item {
  //   std::string name;
  //   int64_t value;
  // };

  const std::vector< std::string > ARGS = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };
  // const std::vector< std::string > ARGS = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };

  enum INS { VAR, N, LABEL, ELSE };

  class Instance {
  public:
    std::set<std::string> GEN;
    std::set<std::string> KILL;
    std::set<std::string> IN;
    std::set<std::string> OUT;
    std::string name;
    int64_t type = L3::INS::ELSE;
    std::vector<L3::Instance *> instances;

    virtual std::string toString() = 0;

    bool equal(L3::Instance * ins);
  };


  class Var: public Instance {
  public:
    // Var();
    Var(std::string name);
    std::string toString();
  };

  class Op: public Instance {
  public:
    Op(std::vector<std::string> & v);
    std::string toString();
  };

  class Br: public Instance {
  public:
    Br(std::vector<std::string> & v);
    std::string toString();
  };

  class Store: public Instance {
  public:
    Store(std::vector<std::string> & v);
    std::string toString();
  };

  class Load: public Instance {
  public:
    Load(std::vector<std::string> & v);
    std::string toString();
  };

  class Call: public Instance {
  public:
    Call(std::vector<std::string> & v);
    std::string toString();
  };

  class Return: public Instance {
  public:
    Return(std::vector<std::string> & v);
    std::string toString();
  };



  struct Function {
    std::string name;
    std::vector< L3::Instance * > arguments;
    std::vector< L3::Instance * > instructions;
  };

  struct Program {
    std::vector< L3::Function * > functions;
  };
}

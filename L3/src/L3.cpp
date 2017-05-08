// By Zhiping

#include <L3.h>

namespace L3 {
  // std::string Instance::toString() {
  //   std::cout << "instance \n";
  //   return "";
  // }

  bool Instance::equal(L3::Instance * ins) {
    return typeid(this) == typeid(ins) && this->type == ins->type;
  }

  Var::Var(std::string name) {
    if (name[0] == ':') {
      this->type = L3::INS::LABEL;
    } else {
      try { // number
        std::stoll(name);
        this->type = L3::INS::N;
      } catch (const std::exception& e) {
        this->type = L3::INS::VAR;
      }
    }
    this->name = name;
  }

  std::string Var::toString() {
    return this->name;
  }

  // std::string Number::toString() {
  //   return "";
  // }
  //
  // std::string Label::toString() {
  //   return "";
  // }

  Br::Br(std::vector<std::string> & v) {
    this->instances.push_back(new L3::Var(v[0]));
    this->instances.push_back(new L3::Var(v[1]));
    this->instances.push_back(new L3::Var(v[2]));
  }

  std::string Br::toString() {
    return "";
  }

  Store::Store(std::vector<std::string> & v) {
    this->instances.push_back(new L3::Var(v[0]));
    this->instances.push_back(new L3::Var(v[1]));
  }

  std::string Store::toString() {
    return "";
  }

  Op::Op(std::vector<std::string> & v) {
    this->name = v[2];

    this->instances.push_back(new L3::Var(v[1]));
    this->instances.push_back(new L3::Var(v[3]));
  }

  std::string Op::toString() {
    return "";
  }

  Call::Call(std::vector<std::string> & v) {
    this->name = v[0];
    for (int k = 1; k < v.size(); k++) {
      this->instances.push_back(new L3::Var(v[k]));
    }
  }

  std::string Call::toString() {
    return "";
  }

  Load::Load(std::vector<std::string> & v) {
    this->instances.push_back(new L3::Var(v[2]));
  }

  std::string Load::toString() {
    return "";
  }

  Return::Return(std::vector<std::string> & v) {
    if (v.size() == 1) {
      this->name = v[0];
    }
  }

  std::string Return::toString() {
    std::cout << "returning \n";
    std::string res = "\n\t\t";
    if (this->name != "") {
      res += "(rax <- " + this->name + ")\n\t\t";
    }
    res += "(return)";
    return res;
  }
}

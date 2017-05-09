// By Zhiping

#include <L3.h>

namespace L3 {
  // std::string Instance::toString() {
  //   std::cout << "instance \n";
  //   return "";
  // }

  bool Instance::equal(L3::Instance * ins) {
    // std::cout << "typeid: " << (typeid(this).name()) << " " << typeid(ins).name() << "\n";
    // std::cout << "equal: " << (typeid(this) == typeid(ins)) << "\n";
    // std::cout << "type: " << typeid(*this).name() << " " << typeid(*ins).name() << "\n";
    // std::cout << "equal: " << (this->type == ins->type) << "\n";
    // std::cout << "equal: " << ((typeid(this) == typeid(ins)) && (this->type == ins->type)) << "\n";
    return (typeid(*this) == typeid(*ins));
    // return (typeid(this) == typeid(ins)) && (this->type == ins->type);
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
    return "";
    // if (this->type == L3::INS::LABEL) {
    //   std::string res = this->name;
    //   res.erase(0, 1);
    //   return res;
    // } else {
    //   return this->name
    // }
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
    if (v.size() == 3) {
      this->instances.push_back(new L3::Var(v[1]));
      this->instances.push_back(new L3::Var(v[2]));
    }
  }

  std::string Br::toString() {
    std::string res = "\n\t\t";
    if (this->instances.size() == 3) {
      // TODO
      res += "";
    } else {
      res += "(goto " + this->instances[0]->name + ")";
    }
    return res;
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
    // std::string res = "\n\t\t";
    //
    // if (false) { // is cmp
    //
    // } else { // op
    //   res += "(" +
    // }

    return "";
  }

  Call::Call(std::vector<std::string> & v) {
    this->name = v[1];
    for (int k = 2; k < v.size(); k++) {
      this->instances.push_back(new L3::Var(v[k]));
    }
  }

  std::string Call::toString() {
    std::string res = "\n\t\t";
    for (int k = 0; k < this->instances.size(); k++) {
      if (k < 6) {
        res += "\n\t\t(" + L3::ARGS[k] + " <- " + this->instances[k]->name + ")";
      } else {
        res += "\n\t\t((mem rsp -" + std::to_string(8 * k - 32) + ") <- " + this->instances[k]->name + ")";
      }
    }
    std::string fName = this->name;
    if (fName[0] == ':') {
      fName.erase(0, 1);
    }
    res += "\n\t\t((mem rsp -8) <- :ret_" + fName + ")";
    res += "\n\t\t(call " + this->name + " " + std::to_string(this->instances.size()) + ")";
    res += "\n\t\t:ret_" + fName;
    return res;
  }

  Load::Load(std::vector<std::string> & v) {
    this->instances.push_back(new L3::Var(v[2]));
  }

  std::string Load::toString() {
    std::string res = "\n\t\t";

    return "";
  }

  Return::Return(std::vector<std::string> & v) {
    if (v.size() == 1) {
      this->name = v[0];
    }
  }

  std::string Return::toString() {
    std::string res = "\n\t\t";
    if (this->name != "") {
      res += "(rax <- " + this->name + ")\n\t\t";
    }
    res += "(return)";
    return res;
  }
}

// By Zhiping

#include <L3.h>

namespace L3 {
  // std::string Instance::toString() {
  //   std::cout << "instance \n";
  //   return "";
  // }

  void debug(std::string str) {
    if (true) {
      std::cout << str << "\n";
    }
  }

  bool Instance::equal(L3::Instance * tile) {
    // std::cout << "typeid: " << (typeid(this).name()) << " " << typeid(ins).name() << "\n";
    // std::cout << "equal: " << (typeid(this) == typeid(ins)) << "\n";
    // std::cout << "type: " << typeid(*this).name() << " " << typeid(*ins).name() << "\n";
    // std::cout << "equal: " << (this->type == ins->type) << "\n";
    // std::cout << "equal: " << ((typeid(this) == typeid(ins)) && (this->type == ins->type)) << "\n";
    if (typeid(*this) == typeid(*tile)) {
      if (tile->type == L3::INS::LABEL) {
        return (tile->type == this->type);
      }
      return true;
    }
    return false;
    // return (typeid(*this) == typeid(*tile) );
    // return (typeid(this) == typeid(ins)) && (this->type == ins->type);
  }

  void Instance::printTree(int d = 0) {
    for (int k = 0; k < d; k++) {
      std::cout << "  ";
    }
    std::cout << typeid(*this).name() << ": " << this->name << "\n";
    for (auto sub : this->instances) {
      sub->printTree(d + 1);
    }
  }

  void printFunc(L3::Function & func) {
    for (auto ins : func.instructions) {
      ins->printTree();
    }
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
    std::string res;
    if (this->instances.size() == 1 && typeid(*this->instances[0]) == typeid(L3::Op)) {
      if (this->name == this->instances[0]->instances[0]->name) {
        res += "\n\t\t(" + this->name + " " + this->instances[0]->name + "= " + this->instances[0]->instances[1]->name + ")";
      } else if (this->name == this->instances[0]->instances[1]->name) {
        res += "\n\t\t(" + this->name + " " + this->instances[0]->name + "= " + this->instances[0]->instances[0]->name + ")";
      } else {
        res += "\n\t\t(" + this->name + " <- " + this->instances[0]->instances[0]->name + ")";
        res += "\n\t\t(" + this->name + " " + this->instances[0]->name + "= " + this->instances[0]->instances[1]->name + ")";
      }
      return res;
    }
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
      L3::Instance * cmp = this->instances[0]->instances[0];
      if (cmp->name == ">=" || cmp->name == ">" || cmp->name == "=" || cmp->name == "<" || cmp->name == "<=") {
        if (cmp->name == ">=" || cmp->name == ">") {
          if (cmp->name == ">=") {
            cmp->name = "<=";
          } else {
            cmp->name = "<";
          }
          res += "\n\t\t(cjump " + cmp->instances[1]->name + " " + cmp->name + " " + cmp->instances[0]->name + " " + this->instances[1]->name + " " + this->instances[2]->name + ")";
        } else {
          res += "\n\t\t(cjump " + cmp->instances[0]->name + " " + cmp->name + " " + cmp->instances[1]->name + " " + this->instances[1]->name + " " + this->instances[2]->name + ")";
        }
      } else {
        res += this->instances[0]->toString();
        res += "\n\t\t(cjump 0 < " + this->instances[0]->name + " " + this->instances[1]->name + " " + this->instances[2]->name + ")";
      }

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
    std::string res;
    res += "\n\t\t((mem " + this->instances[0]->name + " 0) <- " + this->instances[1]->name + ")";
    return res;
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

    bool isLibFunc = (this->name == "print" || this->name == "array-error" || this->name == "allocate");
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
      fName += "_" + std::to_string(rand());
      // CALLCOUNT++;
    }
    if (!isLibFunc) {
      res += "\n\t\t((mem rsp -8) <- :ret_" + fName + ")";
    }
    res += "\n\t\t(call " + this->name + " " + std::to_string(this->instances.size()) + ")";
    if (!isLibFunc) {
      res += "\n\t\t:ret_" + fName;
    }
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

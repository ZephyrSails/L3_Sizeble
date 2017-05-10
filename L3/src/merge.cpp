// By Zhiping

#include <merge.h>

namespace L3 {

  bool canMerge(L3::Instance * ins, L3::Instance * prev) {
    return (prev->type == L3::INS::VAR && ins->type == L3::INS::VAR
            && typeid(*ins) == typeid(L3::Var)
            && ins->name == prev->name
            && ins->OUT.count(prev->name) == 0);
  }

  bool merge(L3::Instance * ins, L3::Instance * prev) {
    if (ins->instances.size() == 0) { // leaf?
      if (canMerge(ins, prev)) { // ins == prev and prev.
        if (typeid(*prev->instances[0]) == typeid(L3::Var)) {
          *ins = *prev->instances[0];
        } else {
          *ins = *prev;
        }
        return true;
      }
      return false;
    } else {
      bool mergeFlag = false;
      for (auto sub : ins->instances) {
        mergeFlag |= merge(sub, prev);
      }
      return mergeFlag;
    }
  }

  void mergeFunc(L3::Function * f) {
    std::cout << "\nbefore merge size: " << f->instructions.size() << "\n";
    int n = f->instructions.size();
    int k = 0;
    while (k < n-1) {
      if (merge(f->instructions[k+1], f->instructions[k])) {
        f->instructions.erase(f->instructions.begin() + k);
        int n = f->instructions.size();
      } else {
        k++;
      }
    }
    std::cout << "after merge size: " << f->instructions.size() << "\n\n";
  }
}

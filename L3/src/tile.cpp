// By: Zhiping

#include <tile.h>

namespace L3 {

  L3::Tile::Tile() {
    std::vector< std::string > v;
    // return t || return
    v = {"VAR"};
    this->tiles[TNAME::RETURN] = new L3::Return(v);
    // var <- s
    this->tiles[TNAME::ASSIGN] = new L3::Var("V1");
    this->tiles[TNAME::ASSIGN]->instances.push_back(new L3::Var("S"));
    // var <- call callee ( args )
    this->tiles[TNAME::ASSIGN_CALL] = new L3::Var("V1");
    v = {"call", ":L1"};
    this->tiles[TNAME::ASSIGN_CALL]->instances.push_back(new L3::Call(v));
    // call callee ( args )
    v = {"call", ":L1"};
    this->tiles[TNAME::CALL] = new L3::Call(v);
    // br var label label
    v = {"V1", ":L1", ":L2"};
    // TODO
    this->tiles[TNAME::C_BR] = new L3::Br(v);

    // label
    this->tiles[TNAME::LABEL] = new L3::Var(":L1");
    // br label
    v = {"V1"};
    this->tiles[TNAME::BR] = new L3::Br(v);
    // store var <- s
    v = {"V1", "V2"};
    this->tiles[TNAME::STORE] = new L3::Store(v);
    // var <- load var
    v = {"V1", "load", "V2"};
    this->tiles[TNAME::LOAD] = new L3::Var("V1");
    this->tiles[TNAME::LOAD]->instances.push_back(new L3::Load(v));
    // var <- t cmp t | var <- t op t
    v = {"V1", "V2", "OP", "V3"};
    this->tiles[TNAME::ASSIGN_OP] = new L3::Var("V1");
    this->tiles[TNAME::ASSIGN_OP]->instances.push_back(new L3::Op(v));
  }

  std::string L3::Tile::Translate(int tIndex, L3::Instance * ins) {
    std::string res = "";
    // return "";
    L3::Instance * tile = this->tiles[tIndex];
    switch (tIndex) {
      case TNAME::RETURN:
                return ins->toString();

      case TNAME::ASSIGN:
                return "\n\t\t(" + ins->name + " <- " + ins->instances[0]->name + ")";

      case TNAME::ASSIGN_CALL:
                res += ins->instances[0]->toString();
                res += "\n\t\t(" + ins->name + " <- rdx)";
                return res;

      case TNAME::CALL:
                return ins->toString();

      case TNAME::C_BR:
                return "";

      case TNAME::LABEL:
                return "\n\t\t" + ins->name;

      case TNAME::BR:
                return ins->toString();

      case TNAME::STORE:
                res += "\n\t\t((mem " + ins->instances[0]->name + " 0) <- " + ins->instances[1]->name + ")";
                return res;

      case TNAME::LOAD:
                res += "\n\t\t(" + ins->name + " <- (mem " + ins->instances[0]->instances[0]->name + " 0))";
                return res;

      case TNAME::ASSIGN_OP:
                if (false) {

                } else {
                  res += "\n\t\t(" + ins->name + " <- " + ins->instances[0]->instances[0]->name + ")";
                  res += "\n\t\t(" + ins->name + " " + ins->instances[0]->name + "= " + ins->instances[0]->instances[1]->name + ")";
                }
                return res;
      default:
                return "";
    }
  }

  bool L3::Tile::MatchTile(L3::Instance * ins, L3::Instance * til, std::vector< L3::Instance * > & leaf) {
    if (!(ins->equal(til)) || (til->instances.size() > 0 && ins->instances.size() != til->instances.size())) {
      // std::cout << "failed\n";
      return false;
    }
    // std::cout << "successed\n";

    for (int k = 0; k < til->instances.size(); k++) {
      // std::cout << "(til->name == til->instances[k]->name) " << (til->name == til->instances[k]->name) << "\n";
      // std::cout << "(til->name == til->instances[k]->name && ins->name != ins->instances[k]->name)" << ((til->name == til->instances[k]->name) && (ins->name != ins->instances[k]->name)) << "\n";
      if (((til->name == til->instances[k]->name) && (ins->name != ins->instances[k]->name)) ||
            !L3::Tile::MatchTile(ins->instances[k], til->instances[k], leaf)) {
        // std::cout << "ins->instances[k]->name " << ins->instances[k]->name << "\n";
        // std::cout << "til->instances[k]->name " << til->instances[k]->name << "\n";
        return false;
      }
    }
    return true;
  }

  void L3::Tile::Match(L3::Instance * ins, std::vector< std::tuple< int, L3::Instance * > > & stack) {

    for (int k = 0; k < this->tiles.size(); k++) {
      // std::cout << "k = " << k << "\n";
      // std::cout << "k = " << k << " trying to Match " << TMAP[k] << "\n";
      std::vector< L3::Instance * > leaf;
      if (this->MatchTile(ins, this->tiles[k], leaf)) {
        std::cout << "k = " << k << " Matched " << TMAP[k] << "\n";
        stack.push_back(std::make_tuple(k, ins));
        for (auto l : leaf) {
          this->Match(l, stack);
        }
        return;
      }
    }
    std::cout << "stack->size(): " << stack.size() << "\n";
    if (stack.size() == 0) {
      std::cout << "didn't matched anything!!!!!\n";
    }
  }


}

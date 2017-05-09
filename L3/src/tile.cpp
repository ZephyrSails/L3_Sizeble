// By: Zhiping

#include <tile.h>

namespace L3 {

  L3::Tile::Tile() {
    std::vector< std::string > v;
    this->tiles[TNAME::RETURN] = new L3::Return(v);
  }

  std::string L3::Tile::Translate(int tIndex, L3::Instance * ins) {
    std::string res = "";
    // return "";
    L3::Instance * tile = this->tiles[tIndex];
    switch (tIndex) {
      case TNAME::RETURN:
                res = ins->toString();
                return res;

      default:
                return "";
                break;
    }
  }

  bool L3::Tile::MatchTile(L3::Instance * ins, L3::Instance * til, std::vector< L3::Instance * > & leaf) {
    std::cout << "match tile\n";
    if (!(ins->equal(til)) || (til->instances.size() > 0 &&
            ins->instances.size() != til->instances.size())) {
      std::cout << "failed\n";
      return false;
    }
    std::cout << "successed\n";

    for (int k = 0; k < til->instances.size(); k++) {
      if ((til->name == til->instances[k]->name && ins->name != ins->instances[k]->name) ||
            !L3::Tile::MatchTile(ins->instances[k], til->instances[k], leaf)) {
        return false;
      }
    }
    return true;
  }

  void L3::Tile::Match(L3::Instance * ins, std::vector< std::tuple< int, L3::Instance * > > & stack) {

    for (int k = 0; k < this->tiles.size(); k++) {
      std::cout << "k = " << k << "\n";
      std::vector< L3::Instance * > leaf;
      if (this->MatchTile(ins, this->tiles[k], leaf)) {
        std::cout << "k = " << k << "\n" << "Matched\n";
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

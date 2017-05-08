// By: Zhiping

#include <tile.h>

namespace L3 {
  bool L3::Tile::MatchTile(L3::Instance * ins, L3::Instance * til, std::vector< L3::Instance * > * leaf) {
    if (!(ins->equal(til)) || (til->instances.size() > 0 &&
            ins->instances.size() != til->instances.size())) {
      return false;
    }

    for (int k = 0; k < til->instances.size(); k++) {
      if ((til->name == til->instances[k]->name && ins->name != ins->instances[k]->name) ||
            !L3::Tile::MatchTile(ins->instances[k], til->instances[k], leaf)) {
        return false;
      }
    }
    return true;
  }

  void L3::Tile::Match(L3::Instance * ins, std::vector< int > * stack) {

    for (int k = 0; k < this->tiles.size(); k++) {
      std::vector< L3::Instance * > leaf;
      if (this->MatchTile(ins, this->tiles[k], &leaf)) {
        stack->push_back(k);
        for (auto l : leaf) {
          this->Match(l, stack);
        }
      }
    }
  }

}

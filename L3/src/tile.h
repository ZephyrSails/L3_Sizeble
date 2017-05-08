// By: Zhiping

#include <L3.h>

namespace L3 {
  class Tile {
    std::vector< L3::Instance * > tiles;

    bool MatchTile(L3::Instance * ins, L3::Instance * til, std::vector< L3::Instance * > * leaf);
    void Match(L3::Instance * ins, std::vector< int > * stack);
  };
}

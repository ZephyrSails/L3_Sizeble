// By: Zhiping

#include <L3.h>

namespace L3 {
  class Tile {
    enum TNAME {
      RETURN
    };
  public:

    std::map< int, L3::Instance * > tiles;

    bool MatchTile(L3::Instance * ins, L3::Instance * til, std::vector< L3::Instance * > & leaf);
    void Match(L3::Instance * ins, std::vector< std::tuple< int, L3::Instance * > > & stack);

    std::string Translate(int tIndex, L3::Instance * ins);
    Tile();
  };
}

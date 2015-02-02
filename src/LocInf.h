#ifndef LOCINF_H
#define LOCINF_H

enum TileEnum {
  Ti_Player,
  Ti_Blind,
  Ti_Halluc,
  Ti_Creature,
  Ti_Unlit,
  Ti_Thing,
  Ti_Environ, // Ti_Floor, Ti_Wall, Ti_MineralSeam,
  Ti_Empty,
  Ti_Firebolt,
  Ti_Fireball,
  Ti_Breath,
  Ti_ThrowItem
};


const COLORREF colorNone = RGB(1, 0, 0);

struct LocInf {
  unsigned char c;
  TileEnum tile;
  COLORREF color;
  int creatureIndex;
  LocInf() { c = ' '; tile = Ti_Empty; color = colorNone; creatureIndex = 0; }
  explicit LocInf(unsigned char c_, TileEnum tile_) :c(c_), tile(tile_), color(colorNone), creatureIndex(0) {} // RGB(1, 0, 0)) {}
  explicit LocInf(unsigned char c_, TileEnum tile_, COLORREF color_, int creatureIndex_) :c(c_), tile(tile_), color(color_), creatureIndex(creatureIndex_) {}

  void baseClear() {
    tile = Ti_Empty;
    color = colorNone;
    creatureIndex = 0; 
  }
};

#endif // LOCINF_H
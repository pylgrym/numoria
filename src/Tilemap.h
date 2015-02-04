#pragma once

#include <vector>
#include <map>

#include "LocInf.h"

class Assoc {
public:
  CString key;
  CPoint tilepos;
  Assoc(CString key_, int x, int y):key(key_), tilepos(x,y){}
  Assoc(){}
};

class Tilemap
{
public:
  Tilemap();
  ~Tilemap();

  // EDIT MODE:
  std::vector<Assoc> tileAssocs;

  bool load(CString filename);
  bool save(CString filename);

  // ACCESS/RUNTIME MODE:
  // std::vector<Assoc> hash;
  std::map<CString,Assoc> hash;
  void buildHash();
  void buildCreatureHash();
  void buildThingHash();

  bool charFromHash(int myChar, int& x, int& y, int creatureThingIndex, TileEnum tileType);

  Assoc* getAssoc(CString key);
  std::map<int, CPoint> creatureTile; // Maps from monster-index to tilepos.
  std::map<int, CPoint> thingTile; // Maps from thing-index to tilepos.

};


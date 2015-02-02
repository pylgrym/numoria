#pragma once

#include <vector>
#include <map>

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
  bool charFromHash(int myChar, int& x, int& y);
};


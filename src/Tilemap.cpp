#include "stdafx.h"
#include "Tilemap.h"

#include <fstream>
#include "debstr.h"



Tilemap::Tilemap()
{
}


Tilemap::~Tilemap()
{
}


bool Tilemap::save(CString filename) {
  std::ofstream f(filename);
  std::vector<Assoc>::iterator i;
  for (i = tileAssocs.begin(); i != tileAssocs.end(); ++i) {
    Assoc& assoc = *i;

    // CA2T uc(buf, CP_ACP);  // s.c_str()
    CT2A ac(assoc.key, CP_UTF8); // CP_ACP);  // s.c_str()

    f << '"' << (const char*)ac << '"' << " "; // assoc.key;
    f << assoc.tilepos.x << " ";
    f << assoc.tilepos.y << std::endl;
    //listview.InsertItem(0, assoc.key);
  }
  return true;
}

bool Tilemap::load(CString filename) {
  std::ifstream f(filename);
  Assoc assoc;
  char buf[128] = "";

  while (f.good() && !f.bad()) { // !f.eof()) {
    debstr() << "\nNext item..\n";

    f >> buf; //  s;
    debstr() << "(key)got:[" << buf << "]\n";
    if (!f.good()) { break;  }
    if (f.bad()) { break;  }

    CA2T uc(buf, CP_ACP);  
    CString s = uc;
    // Strip quotes:
    s = s.Left(s.GetLength()-1);
    s = s.Mid(1);
    assoc.key = s; // s.c_str();

    f >> assoc.tilepos.x; 
    debstr() << "x:[" << assoc.tilepos.x << "]\n";

    f >> assoc.tilepos.y; 
    debstr() << "y:[" << assoc.tilepos.y << "]\n";

    tileAssocs.push_back(assoc);
    debstr() << "end of that item..\n";
  }
  return true;
}






void Tilemap::buildHash() {
  // hash.resize(128);

  std::vector<Assoc>::iterator i;
  for (i = tileAssocs.begin(); i != tileAssocs.end(); ++i) {
    Assoc& assoc = *i;
    hash[assoc.key] = assoc;
  }

}


bool Tilemap::charFromHash(int myChar, int& x, int& y) {
  std::map<CString, Assoc>::iterator i;

  char buf[2] = "a"; buf[0] = myChar;

  // CT2A ac(assoc.key, CP_UTF8); // CP_ACP);  // s.c_str()
  CA2T uc(buf, CP_ACP);  // s.c_str()
  CString myKey=uc; 

  i = hash.find(myKey);
  if (i == hash.end()) {
    x = 0; y = 0;
    return false;
  }

  x = i->second.tilepos.x;
  y = i->second.tilepos.y;
  return true;
}


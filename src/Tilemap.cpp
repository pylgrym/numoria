#include "stdafx.h"
#include "Tilemap.h"

#include <fstream>
#include "debstr.h"

#include "config.h"
#include "constant.h"
#include "types.h"
#include "externs.h"

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

  debstr() << "\nitems..\n";
  while (f.good() && !f.bad()) { 

    std::string line;
    std::getline(f, line);

    size_t lastQuote = line.find_last_of('"');
    if (lastQuote == std::string::npos) { continue; } // error.
    size_t firstQuote = line.find_first_of('"');
    std::string key = line.substr(firstQuote+1, (lastQuote - firstQuote)-1);

    std::string numPart = line.substr(lastQuote + 2);

    // JG: this is brittle against bad formatting
    // - would be better if it parsed for found integers.
    size_t numDivider = numPart.find_first_of(' '); 
    std::string leftNum = numPart.substr(0, numDivider);
    std::string rightNum = numPart.substr(numDivider+1);
    int x = stoi(leftNum);
    int y = stoi(rightNum);

    CA2T uc(key.c_str(), CP_ACP);  
    CString s = uc;

    Assoc assoc;
    assoc.key = s;
    assoc.tilepos.x = x;
    assoc.tilepos.y = y;

    debstr() << "tile:[" << assoc.tilepos.x << " / " << assoc.tilepos.y << "]'" << key.c_str() << "'\n";
    tileAssocs.push_back(assoc);
  }
  return true;
}





Assoc* Tilemap::getAssoc(CString key) {
  std::map<CString, Assoc>::iterator i;
  i = hash.find(key);
  if (i == hash.end()) { return NULL;  }
  return &i->second;
}


void Tilemap::buildHash() {
  // hash.resize(128);

  std::vector<Assoc>::iterator i;
  for (i = tileAssocs.begin(); i != tileAssocs.end(); ++i) {
    Assoc& assoc = *i;
    hash[assoc.key] = assoc;
  }

  for (int j = 0; j < MAX_CREATURES; ++j) {
    creature_type& creatureType = c_list[j];

    // CT2A ac(assoc.key, CP_UTF8); // CP_ACP);  // s.c_str()
    CA2T uc(creatureType.name, CP_ACP);  // s.c_str()
    CString myKey=uc; 

    Assoc* pAssoc = getAssoc(myKey);
    if (pAssoc != NULL) {
      creatureTile[j] = pAssoc->tilepos;
    }

  }

}


bool Tilemap::charFromHash(int myChar, int& x, int& y, int creatureIndex) {
  if (creatureIndex > 1) {
    std::map<int, CPoint>::iterator j;
    j = creatureTile.find(creatureIndex); // Maps from monster-index to tilepos.
    if (j != creatureTile.end()) {
      x = j->second.x;
      y = j->second.y;
      return true;
    }
  }


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


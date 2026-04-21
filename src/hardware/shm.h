#ifndef SHM_H
#define SHM_H

#include <bits/stdc++.h>

class Shm {

public:
  int sId = -1;
  int sSize = 0;
  int sLoadAddress = 0;
  int sEndAddress = 0;
  int sLinkedPrcCount = 0;
  std::string prcOne = "null";
  std::string prcTwo = "null";

  // DEFAULT
  Shm(int id, int size, int address, std::string name)
      : sId{id}, sSize{size}, sLoadAddress{address},
        sEndAddress{address + size} {}
};
#endif

// FIGURE OUT WHERE THE HELL TO LOAD THIS AND KEEP TRACK OF// ALL SHARED MEMORY
// LOCATIONS, MAYBE USE LOADER IDK??
//  FIGURE OUT HOW TO ALLOCATE MEMORY FOR IT

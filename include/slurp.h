#pragma once

#include <cstdio>
#include <cstring>

inline char* slurp (const char* path) {
  FILE    *infile;
  char    *buffer;
  int    numbytes;

  infile = fopen(path,"r");

  fseek(infile,0,SEEK_END);
  numbytes = ftell(infile);
  rewind(infile);

  buffer = new char[numbytes+1];
  fread(buffer,1,numbytes,infile);
  buffer[numbytes] = 0;
  return buffer;
}

inline const char* slurpc (const char* path) {
  FILE    *infile;
  char    *buffer;
  int    numbytes;

  infile = fopen(path,"r");

  fseek(infile,0,SEEK_END);
  numbytes = ftell(infile);
  rewind(infile);

  buffer = new char[numbytes+1];
  fread(buffer,1,numbytes,infile);
  buffer[numbytes] = 0;
  return buffer;
}

inline const unsigned char* slurpuc (const char* path) {
  FILE    *infile;
  unsigned char    *buffer;
  int    numbytes;

  infile = fopen(path,"r");

  fseek(infile,0,SEEK_END);
  numbytes = ftell(infile);
  rewind(infile);

  buffer = new unsigned char[numbytes+1];
  fread(buffer,1,numbytes,infile);
  buffer[numbytes] = 0;
  return buffer;
}

inline char** slurpCFG(char* input, int& count){
  count = 0;

  char* pos = input;
  // Count number of entries
  while (pos != 0){
    pos = strchr(pos+1,'=');
    if (pos)
      count++;
  }

  // Convert tokens to strings
  char** output = new char*[count*2];

  pos = input;  
  for (int i=0; i < count; i++){
    output[i*2] = pos;
    // Replace = with null terminator, advance position
    pos = strchr(pos,'='); *pos = 0; pos++;
    output[(i*2)+1] = pos;
    // Replace newline with null terminator, advance position
      pos = strchr(pos,'\n'); *pos = 0; pos++;
  }


  count *= 2;

  return output;
}
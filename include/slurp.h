#pragma once

#include <cstdio>

inline const char* slurpc (const char* path) {
  FILE    *infile;
  char    *buffer;
  int    numbytes;

  infile = fopen(path,"r");

  fseek(infile,0,SEEK_END);
  numbytes = ftell(infile);
  rewind(infile);

  buffer = new char[numbytes];
  fread(buffer,1,numbytes,infile);
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

  buffer = new unsigned char[numbytes];
  fread(buffer,1,numbytes,infile);
  return buffer;
}
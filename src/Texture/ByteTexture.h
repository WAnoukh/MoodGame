#pragma once
#include "ByteTexture.h"

struct ByteTexture
{
  static ByteTexture* LoadTexture(char const* path);
  unsigned char* data;
  int width;
  int height;
  int numCol;
};


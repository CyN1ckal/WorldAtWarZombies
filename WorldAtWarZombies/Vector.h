#pragma once

struct Vector4 {
  float x, y, z, w;
};

struct Vector3 {
  float x, y, z;
};

struct Vector2 {
  float x, y;
};

float VecDistance(Vector3 src, Vector3 dst);
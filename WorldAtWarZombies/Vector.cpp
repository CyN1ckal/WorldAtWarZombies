#include "pch.h"

float VecDistance(Vector3 src, Vector3 dst) {
  float ReturnValue = (dst.x - src.x) * (dst.x - src.x) +
                      (dst.y - src.y) * (dst.y - src.y) +
                      (dst.z - src.z) * (dst.z - src.z);

  return sqrt(ReturnValue);
}
#include "wrapping_integers.hh"
#include <cstdint>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.

  return Wrap32(static_cast<uint32_t>(n) + zero_point.raw_value_);
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  // Your code here.
  Wrap32 target = wrap(checkpoint, zero_point);
  int32_t offset = raw_value_ - target.raw_value_;
  int64_t ans = checkpoint + offset;
  if (ans >= 0) {
    return ans;
  }
  else {
    return ans + (1ul << 32);
  }
}

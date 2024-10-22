#include "reassembler.hh"
#include "byte_stream.hh"
#include <cstdint>
#include <iostream>
#include <sys/types.h>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.

  int64_t overlap = next_index_ - first_index;
  if ( overlap >= 0 ) {
    if ( overlap >= (int64_t)data.size() ) {
      goto end;
    }
    int64_t cap = output_.writer().available_capacity();

    if ( cap > (int64_t)data.size() - overlap ) {
      cap = data.size() - overlap;
    }
    output_.writer().push( data.substr( overlap, cap ) );
    next_index_ += cap;

    writePendingToStream();
  } else if ( capacity_ - bytes_pending_ >= data.size() ) {
    if ( is_last_substring ) {
      pending_[first_index] = data.substr( 0, capacity_ - first_index );
      bytes_pending_ += capacity_ - first_index;
    } else {

      handleRepeatInPending( first_index, data );
    }
  }

end:
  if ( is_last_substring ) {
    last_appeared_ = true;
    end_index_ = first_index + data.size();
  }

  if ( last_appeared_ && next_index_ == end_index_ ) {
    output_.writer().close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return bytes_pending_;
}

void Reassembler::writePendingToStream()
{
  for ( auto it = pending_.begin(); it != pending_.end(); ++it ) {
    if ( it->first > next_index_ ) {
      break;
    }

    if ( it->first + it->second.size() <= next_index_ ) {
      bytes_pending_ -= it->second.size();
      pending_.erase( it );
      continue;
    }
    // 当key 小于等于next_index时, 重叠部分丢弃
    int64_t overlap = next_index_ - it->first;
    output_.writer().push( it->second.substr( overlap ) );
    next_index_ += it->second.size() - overlap;
    bytes_pending_ -= it->second.size();
    pending_.erase( it );
  }
}

void Reassembler::handleRepeatInPending( uint64_t first_index, std::string data )
{
  if ( data.empty() ) {
    return;
  }

  for ( auto it = pending_.begin(); it != pending_.end(); ++it ) {

    uint64_t data_end = first_index + data.size();
    uint64_t it_start = it->first;
    uint64_t it_end = it->first + it->second.size();

    // data的起始位置在it的范围之后, 直接下一个
    if ( first_index >= it_end ) {
      continue;
    }

    // data的结束位置在it的范围之前, 直接退出
    if ( data_end <= it_start ) {
      break;
    }

    // data刚好在it的范围之内
    if ( first_index >= it_start && data_end <= it_end ) {
      return;
    }

    // it刚好在data的范围之内
    if ( first_index <= it_start && data_end >= it_end ) {
      handleRepeatInPending( first_index, data.substr( 0, it_start - first_index ) );
      handleRepeatInPending( it_end, data.substr( it_end - first_index ) );

      return;
    }

    // data的起始位置在it的起始之前, 且data的结束位置在it的结束之后
    if ( first_index < it_start && data_end <= it_end ) {
      handleRepeatInPending( first_index, data.substr( 0, it_start - first_index ) );

      return;
    }

    // data的起始位置在it的起始之后, 且data的结束位置在it的结束之后
    if ( first_index >= it_start && data_end > it_end ) {
      handleRepeatInPending( it_end, data.substr( it_end - first_index ) );

      return;
    }
  }

  // 如果上面一直没有return, 说明data和it没有重叠部分
  int insertCap = std::min( data.size(), capacity_ - first_index );
  pending_[first_index] = data.substr( 0, insertCap );
  bytes_pending_ += insertCap;
  
}
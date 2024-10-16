#include "byte_stream.hh"
#include <cstdint>
#include <string_view>
#include <iostream>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {
  buffer_.clear();
}

bool Writer::is_closed() const
{
  // Your code here.
  return is_closed_;
}

void Writer::push( string data )
{
  // Your code here.
  if (data.empty()) {
    return ;
  }
  
  uint64_t len = data.size();
  if (len > available_capacity()) {
    len = available_capacity();
  }

  bytes_pushed_ += len;
  buffer_.insert(buffer_.end(), data.begin(), data.begin() + len);
}

void Writer::close()
{
  // Your code here.
  is_closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - buffer_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return buffer_.size() == 0 && is_closed_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes_popped_;
}

string_view Reader::peek() const
{
  // Your code here.
  // peek_.assign(buffer_.begin(), buffer_.end());
  // std::cout << peek_ << " " << buffer_.size() << std::endl;
  return std::string_view(buffer_);
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if (len > bytes_buffered()) {
    len = bytes_buffered();
  }

  bytes_popped_ += len;
  // buffer_ = buffer_.substr(len);
  buffer_.erase(buffer_.begin(), buffer_.begin() + len);
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffer_.size();
}

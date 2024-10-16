#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return is_closed_;
}

void Writer::push( string data )
{
  // Your code here.
  uint64_t len = data.size();
  if (len <= available_capacity()) {
    buffer_.append(data);
    bytes_pushed_ += len;
    
    return;
  }

  bytes_pushed_ += available_capacity();
  buffer_.append(data.substr(0, available_capacity()));
  
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
  return std::string_view(buffer_);
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if (len <= bytes_buffered()) {
    buffer_ = buffer_.substr(len);
    bytes_popped_ += len;

    return;
  }

  bytes_popped_ += bytes_buffered();
  buffer_ = "";
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffer_.size();
}

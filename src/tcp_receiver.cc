#include "tcp_receiver.hh"
#include "wrapping_integers.hh"
#include <cstdint>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  // Your code here.
  if (message.RST) {
    reader().set_error();
  }
  if (message.SYN) {
    zero_point_ = Wrap32(message.seqno);
    ackno_.emplace(message.seqno);
  }

  // 如果ackno_有值, 说明已经建立了TCP连接
  if (ackno_.has_value()) {
    uint64_t check_point = writer().bytes_pushed() + 1;
    uint64_t first_index = 
    Wrap32(message.SYN ? message.seqno + 1 : message.seqno).unwrap(zero_point_, check_point) - 1;
    reassembler_.insert(first_index, std::move(message.payload), message.FIN);
    ackno_ = ackno_->wrap(writer().bytes_pushed() + 1 + writer().is_closed(), zero_point_);
  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  return {ackno_, 
      static_cast<uint16_t>(std::min(reassembler_.writer().available_capacity(), static_cast<uint64_t>(UINT16_MAX))),
      reader().has_error()
      };
}

CXX = clang++
CXXFLAGS = -std=c++17 -Wall -g

TARGET = custom_test

SRCS = custom_test.cc ../src/byte_stream.cc ../src/byte_stream_helpers.cc

HEADERS = ../src/byte_stream.hh

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
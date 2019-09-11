#---- $@ 目标文件的名称		$^ 所有的依赖文件，以空格分开，不包含重复的依赖文件	$< 第一个依赖文件的名称 ----#

CXX = g++
CXXFLAGS += -std=c++14 -Wall -g
#LDFLAGS = -lprotobuf
LDFLAGS = `pkg-config --cflags --libs protobuf`

all: myinfo test_in test_out

myinfo:
	protoc -I=./ --cpp_out=./ myinfo.proto

test_in:test_in.c myinfo.pb.cc
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
 
test_out:test_out.c myinfo.pb.cc
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
 
clean:
	rm -rf *.o
	rm -rf test_in
	rm -rf test_out
	rm -rf myinfo.pb.*
	rm -rf *.log

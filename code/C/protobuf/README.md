# protobuf demo

Protobuf: Google Protocol Buffer 是 Google 公司内部的混合语言数据标准，用于 RPC 系统和持续数据存储系统。

#### 介绍
简要介绍 protobuf 在C++中的使用。

#### 依赖
libprotobuf-dev protobuf-compiler

###根据.proto生成对应的.cc和.h文件
protoc -I=./ --cpp_out=./ myinfo.proto

###编译
g++ -Wall -std=c++14 test_in.c myinfo.pb.cc -o test_in -lprotobuf
g++ -Wall -std=c++14 test_out.c myinfo.pb.cc -o test_out -lprotobuf

###运行
./test_in
./test_out

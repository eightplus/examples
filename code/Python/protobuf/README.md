# protobuf demo

#### 介绍
简要介绍 protobuf 在 python 中的使用。

#### 依赖
python-protobuf

###根据.proto生成对应的py文件(myinfo_pb2.py)
protoc -I=./ --python_out=./ myinfo.proto

###运行
 序列化: 将对象写入文件
 python test_in.py 1.log

 反序列化: 从文件中读取对象
 python test_out.py 1.log

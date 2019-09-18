/*
 * Copyright (C) 2018 ~ 2020 kobe24_lixiang@126.com
 *
 * Authors:
 *  lixiang    kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * sudo apt install libboost-dev libboost1.58-dev libboost-serialization1.58-dev
 * g++ -o test test.cpp -L/usr/lib -lboost_serialization
 */

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;


//侵入式
class MyInfo
{
public:
    MyInfo() : m_id(0), m_name("") {}
    MyInfo(int id, string name) : m_id(id)
      , m_name(name)
    {

    }

    int getId() const {return m_id;}
    string getName() const {return m_name;}

private:
    //为使当前的结构能支持序列化，得加入下面的代码段
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar& m_id;
        ar& m_name;
    }

private:
     int m_id;
     string m_name;
};


//序列化基类MyInfo的子类MyInfoChild：在子类的序列化函数里先序列化基类，然后再序列化子类的数据成员
class MyInfoChild: public MyInfo
{
public:
   MyInfoChild() : m_weight(0.0){}
   MyInfoChild(int id, string name, float weight) : MyInfo(id, name), m_weight(weight){}

   float getWeight() const{return m_weight;}

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<MyInfo>(*this);
        ar & m_weight;
    }

private:
   float m_weight;
};




struct MyHouse
{
    string address;
    string areacode;

private:
    //为使当前的结构能支持序列化，得加入下面的代码段
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & address;
        ar & areacode;
    }
};



class MyData
{
public:
    MyData() {}
    MyData(int id, string name, string sex) : m_id(id)
      , m_name(name)
      , m_sex(sex)
    {

    }

    vector<MyHouse> m_hostList;
    int getId() const {return m_id;}
    string getName() const {return m_name;}
    string getSex() const {return m_sex;}

private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar,const unsigned int version)//序列化函数
    {
        ar & m_id;
        ar & m_name & m_sex;
        ar & m_hostList;
    }

private:
    int m_id;
    std::string m_name;
    std::string m_sex;

};

void printMyData(MyData &mydata)
{
    printf("MyData ID: %d, NAME: %s, SEX: %s\n", mydata.getId(), mydata.getName().c_str(), mydata.getSex().c_str());
    vector<MyHouse>::iterator iter = mydata.m_hostList.begin();
    while (iter!=mydata.m_hostList.end())
    {
        printf("House address: %s, areacode:%s\n", iter->address.c_str(), iter->areacode.c_str());
        iter++;
    }
}



//非侵入式
class MyNoneIntrusiveData
{
public:
   MyNoneIntrusiveData():m_id(0), m_name("") {}
   MyNoneIntrusiveData(int id, string name):m_id(id), m_name(name){}

   int m_id;
   string m_name;
};
namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive & ar, MyNoneIntrusiveData &da, const unsigned int version)
        {
            ar & da.m_id;
            ar & da.m_name;
        }
    }//namespace serialization
}//namespace boost

//序列化基类MyNoneIntrusiveData的子类MyNoneIntrusiveDataChild：在子类的序列化函数里先序列化基类，然后再序列化子类的数据成员
class MyNoneIntrusiveDataChild: public MyNoneIntrusiveData
{
public:
    MyNoneIntrusiveDataChild() : m_weight(0.0){}
    MyNoneIntrusiveDataChild(int id, string name, float weight) : MyNoneIntrusiveData(id, name), m_weight(weight){}

    float m_weight;
};
namespace boost {
   namespace serialization {
       template<class Archive>
       void serialize(Archive & ar, MyNoneIntrusiveDataChild & da, const unsigned int version)
       {
           ar & boost::serialization::base_object<MyNoneIntrusiveData>(da);
           ar & da.m_weight;
       }
   }//namespace serialization
}//namespace boost


int main(int argc, char* argv[])
{
    MyInfo info(2019, "lixiang");


    /*--------------Test 1--------------*/
    //使用二进制方式序列化
    stringstream binary_sstream;// or "ostringstream binary_sstream"
    boost::archive::binary_oarchive binary_oa(binary_sstream);
    binary_oa << info;//序列化到一个stringstream里
    string binary_content = binary_sstream.str();//binary_content保存了序列化后的数据

    //使用二进制方式反序列化
    MyInfo binary_info;
    istringstream iss(binary_content);
    boost::archive::binary_iarchive iarchive(iss);
    iarchive >> binary_info;//从一个保存序列化数据的string里面反序列化，从而得到原来的对象
    printf("Binary ID: %d, NAME: %s\n", binary_info.getId(), binary_info.getName().c_str());


    /*--------------Test 2--------------*/
    //使用文本方式序列化
    stringstream text_sstream;
    boost::archive::text_oarchive text_oa(text_sstream);
    text_oa << info;
    string text_content = text_sstream.str();//text_content保存了序列化后的数据
    printf("Serialized text_content: %s\n", text_content.c_str());

    //使用文本方式反序列化
    MyInfo text_info;
    boost::archive::text_iarchive(text_sstream) >> text_info;
    printf("Text ID: %d, NAME: %s\n", text_info.getId(), text_info.getName().c_str());


    /*--------------Test 3--------------*/
    //使用二进制方式序列化
    MyInfoChild child_data1(201901001, "lixiang", 56.5);
    ostringstream os1;
    boost::archive::binary_oarchive binary_oa2(os1);
    binary_oa2 << child_data1;//序列化到一个ostringstream里
    string child_content1 = os1.str();//child_content1保存了序列化后的数据

    //使用二进制方式反序列化
    MyInfoChild child_data2;
    istringstream iss2(child_content1);
    boost::archive::binary_iarchive iarchive2(iss2);
    iarchive2 >> child_data2;//从一个保存序列化数据的string里面反序列化，得到原来的对象
    printf("Child Binary ID: %d, NAME: %s, WEIGHT: %.1f\n", child_data2.getId(), child_data2.getName().c_str(), child_data2.getWeight());


    /*--------------Test 4--------------*/
    MyData my_data(2019, "lixiang", "man");
    //为被序列化对象添加两条记录
    MyHouse house1, house2;
    house1.address = "湖南岳阳";
    house1.areacode = "0730";
    house2.address = "湖南长沙";
    house2.areacode = "0731";
    my_data.m_hostList.push_back(house1);
    my_data.m_hostList.push_back(house2);

    //使用文本方式序列化
    stringstream text2_sstream;
    boost::archive::text_oarchive(text2_sstream) << my_data;
    printf("Serialized data: %s\n", text2_sstream.str().c_str());

    //使用文本方式反序列化
    MyData my_out_data;
    boost::archive::text_iarchive(text2_sstream) >> my_out_data;
    printMyData(my_out_data);


    /*--------------Test 5  非嵌入式--------------*/
    //使用二进制方式序列化
    MyNoneIntrusiveData data3(8888, "Hello, World!");
    ostringstream os2;
    boost::archive::binary_oarchive binary_oa3(os2);
    binary_oa3 << data3;//序列化到一个ostringstream里
    std::string binary_content2 = os2.str();//content保存了序列化后的数据。

    //使用二进制方式反序列化
    MyNoneIntrusiveData data4;
    istringstream iss3(binary_content2);
    boost::archive::binary_iarchive iarchive3(iss3);
    iarchive3 >> data4;//从一个保存序列化数据的string里面反序列化，从而得到原来的对象
    printf("NoneIntrusiveData ID: %d, NAME: %s\n", data4.m_id, data4.m_name.c_str());


    /*--------------Test 6--------------*/
    //使用二进制方式序列化
    MyNoneIntrusiveDataChild child_data3(201901002, "lixiang", 57.8);
    ostringstream os3;
    boost::archive::binary_oarchive binary_oa4(os3);
    binary_oa4 << child_data3;//序列化到一个ostringstream里
    string child_content2 = os3.str();//child_content2保存了序列化后的数据

    //使用二进制方式反序列化
    MyNoneIntrusiveDataChild child_data4;
    istringstream iss4(child_content2);
    boost::archive::binary_iarchive iarchive4(iss4);
    iarchive4 >> child_data4;//从一个保存序列化数据的string里面反序列化，得到原来的对象
    printf("NoneIntrusiveDataChild Binary ID: %d, NAME: %s, WEIGHT: %.1f\n", child_data4.m_id, child_data4.m_name.c_str(), child_data4.m_weight);

    return 0;
}

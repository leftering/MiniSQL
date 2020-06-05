#pragma once
#include "pch.h"
struct Data {
    int type;
    int datai;
    float dataf;
    std::string datas;
};
class Tuple {
private:
    std::vector<Data> contents;  //存储元组里的每个数据的信息
    bool deleted;
public:
    Tuple() :deleted(false) {};
    Tuple(const Tuple& tuple_in){//拷贝元组
        deleted = tuple_in.deleted;
        for (int i = 0; i < tuple_in.contents.size(); i ++){
            contents.push_back(tuple_in.contents[i]);
        }
    };  
    void addData(Data data_in);  //新增元组
    std::vector<Data> getData()const;  //返回数据
    void eraseData(int i);
    int getSize();  //返回元组的数据数量
    bool isDeleted();
    void setDeleted();
    void showTuple();  //显示元组中的所有数据
};

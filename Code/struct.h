#pragma once
#ifndef _STRUCT_H_
#define _STRUCT_H_

#include "pch.h"

struct Attribute {
    int attri_num;  
    std::string attri_name[32]; 
    short attri_type[32]; 
    bool is_unique[32];  //判断每个属性是否unique，是为true
    short primary_key;  //判断是否存在主键,-1为不存在，其他则为主键的所在位置
    bool has_index[32]; //判断是否存在索引
};

#endif

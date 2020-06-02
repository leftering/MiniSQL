#pragma once
#ifndef _BUFFER_MANAGER_H_
#define _BUFFER_MANAGER_H_ 1

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "pch.h"
#include "define.h"
#include "record_manager.h"


// BufferManager类。对外提供操作缓冲区的接口。
class BufferManager {
private:
    Block* Pages;           // 缓冲池，实际上就是一个元素为Page的数组，实际内存空间将分配在堆上
    int page_num;           // 记录总页数
    int current_position;   // 时钟替换策略需要用到的变量
    int getEmptyPageId();
    // 将对应文件的对应块载入对应内存页，对于文件不存在返回-1，否则返回0
    bool loadDiskBlock(int page_id, std::string table_name, int block_id);
public:
    // 构造函数
    BufferManager(int buffer_size = MAXBUFFERSIZE) :current_position(0), page_num(min(buffer_size, MAXBUFFERSIZE)), Pages(new Block[buffer_size]) {};
    // 析构函数
    ~BufferManager();
    // 通过页号得到页的句柄(一个页的头地址)
    char* getPage(std::string table_name, int block_id);
    // 标记page_id所对应的页已经被修改
    void modifyPage(int page_id);
    // 钉住一个页
    void pinPage(int page_id);
    // 解除一个页的钉住状态(需要注意的是一个页可能被多次钉住，该函数只能解除一次)
    // 如果对应页的pin_count_为0，则返回-1
    void unpinPage(int page_id);
    // 将对应内存页写入对应文件的对应块。
    void flushPage(int page_id);
};

#endif
#pragma once
#ifndef _BUFFER_MANAGER_H_
#define _BUFFER_MANAGER_H_

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "pch.h"
#include "define.h"
#include "record_manager.h"

class Block
{
private:
	char data[BLOCKSIZE];
	std::string table_name;
	int block_id;
	bool dirty_bit;
	bool use_bit;
	bool is_pinned;
	int ptr;
	int free_space;
public:
	Block(std::string table = "", int _block_id = -1, bool _dirty_bit = false, bool _use_bit = false, bool _is_pinned = false) {
		table_name = table;
		block_id = _block_id;
		dirty_bit = _dirty_bit;
		use_bit = _use_bit;
		is_pinned = _is_pinned;
		for (int i = 0; i < BLOCKSIZE; i++) {
			data[i] = -1;
		}
		ptr = 0;
		free_space = BLOCKSIZE - 3;
		sprintf(data + 1, "%08x", BLOCKSIZE - 1);
	}
	~Block();
	void setTableName(std::string table);
	std::string getTableName();
	void setBlockId(int _block_id);
	int getBlockId();
	void setDirty(bool _dirty);
	bool isDirty();
	void setUsed(bool _use);
	bool isUsed();
	void setPinned(bool _pinned);
	bool isPinned();
	char* getData();

	char* getNextRecord();
};

// BufferManager类。对外提供操作缓冲区的接口。
class BufferManager {
private:
    Block* Pages;           // 缓冲池，实际上就是一个元素为Page的数组，实际内存空间将分配在堆上
    int page_num;           // 记录总页数
    int current_position;   // 时钟替换策略需要用到的变量
    int getEmptyPageId();
    // 将对应文件的对应块载入对应内存页，对于文件不存在返回-1，否则返回0
    bool loadDiskBlock(int page_id, std::string table_name, int block_id);
    void initialize();
public:
    // 构造函数
    BufferManager(int buffer_size = MAXBUFFERSIZE) :current_position(0), page_num(min(buffer_size, MAXBUFFERSIZE))
    {
        initialize();
    };
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
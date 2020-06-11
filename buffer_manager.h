#pragma once
#ifndef _BUFFER_MANAGER_H
#define _BUFFER_MANAGER_H

#pragma once
#include "pch.h"
#include "define.h"
class Block
{
private:
	BYTE data[BLOCKSIZE];
	std::string table_name;
	int block_id;
	bool dirty_bit;
	bool use_bit;
	bool is_pinned;
public:
	Block(std::string table = "", int _block_id = -1, bool _dirty_bit = false, bool _use_bit = false, bool _is_pinned = false) {
		table_name = table;
		block_id = _block_id;
		dirty_bit = _dirty_bit;
		use_bit = _use_bit;
		is_pinned = _is_pinned;
		for (int i = 0; i < BLOCKSIZE; i++) {
			data[i] = 255;
		}
		data[0] = 0;
		short free_space_ptr = BLOCKSIZE - 3;
		memcpy(data + 1, &free_space_ptr, sizeof(short));
		short free_space = BLOCKSIZE - 5;
		memcpy(data + BLOCKSIZE - 2, &free_space, sizeof(short));
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
	BYTE* getData();

	BYTE* getRecord(int ith);
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
	BufferManager(int page_num_ = MAXBUFFERSIZE) : current_position(0), page_num(min(page_num_, MAXBUFFERSIZE)) {
		initialize();
	};
	// 析构函数
	~BufferManager();
	// 通过页号得到页的句柄(一个页的头地址)
	Block* getPage(std::string table_name, int block_id);
	// 标记page_id所对应的页已经被修改
	void modifyPage(int page_id);
	// 钉住一个页
	void pinPage(int page_id);
	// 解除一个页的钉住状态(需要注意的是一个页可能被多次钉住，该函数只能解除一次)
	// 如果对应页的pin_count_为0，则返回-1
	void unpinPage(int page_id);
	// 将对应内存页写入对应文件的对应块。
	void flushPage(int page_id);
	int getBlockNum(std::string table_name);
	int getPageId(std::string table_name, int block_id);
};

#endif // !_BUFFER_MANAGER_H

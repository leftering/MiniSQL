#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_ 

#include "pch.h"
#include "define.h"

class Block
{
private:
	char data[BLOCKSIZE];
	std::string filename;
	int block_id;
	bool dirty_bit;
	bool use_bit;
	bool is_pinned;
public:
	Block(std::string table = "", int _block_id = -1, bool _dirty_bit = false, bool _use_bit = false, bool _is_pinned = false) {
		filename = RECORDPATH + table + ".data";
		block_id = _block_id;
		dirty_bit = _dirty_bit;
		use_bit = _use_bit;
		is_pinned = _is_pinned;
		for (int i = 0; i < BLOCKSIZE; i++) {
			data[i] = 0;
		}
		data[1] = sprintf(data + 1, "%08x", BLOCKSIZE - 1);
	}
	~Block();
	int getBlockId();
	void setDirty(bool _dirty);
	bool isDirty();
	void setUsed(bool _use);
	bool isUsed();
	void setPinned(bool _pinned);
	bool isPinned();
	char* getData();
};

#endif
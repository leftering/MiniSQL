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
public:
	Block(std::string table = "", int _block_id = -1) {
		filename = RECORDPATH + table + ".data";
		block_id = _block_id;
		for (int i = 0; i < BLOCKSIZE; i++) {
			data[i] = 0;
		}
		data[1] = sprintf(data + 1, "%08x", BLOCKSIZE - 1);
	}
	void InsertRecord();
};

#endif
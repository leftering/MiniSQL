#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_ 

#include "pch.h"
#include "config.h"

class Block
{
private:
	char data[BLOCKSIZE];
	std::string filename;
	int block_id;
	bool dirty;
	bool locked;
public:

};

#endif
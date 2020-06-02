#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_ 

#include "pch.h"
#include "define.h"
#include "catalog.h"
#include "buffer_manager.h"

extern BufferManager buffer_manager;

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

class RecordManager
{
public:

	// Select record from table. Return number of records selected
	int select(std::string table_name, const int* type, const int colId, const int op, const char* opValue, vector<char*>* records);

	// Insert record into table. Return new index id
	int insert(const char* table_name, const int* type, const char* data);

	// Delete id-th record from table. Return true if success
	bool remove(const char* tableName, const vector<int>* ids);

	// Create table. Return true if success
	bool createTable(const char* tableName);

	// Drop table. Return true if success
	bool dropTable(const char* tableName);

	// Check if record satisfy all conditions
	bool checkRecord(const char* record, std::string table_name, const int* type, const int colId, const int op, const char* opValue);

private:

	// Compare string
	bool charCmp(const char* a, const char* b, int op);

	// Compare int
	bool intCmp(const char* a, const char* b, int op);

	// Compare float
	bool floatCmp(const char* a, const char* b, int op);

	int getBlockNum(std::string table_name);
};
#endif
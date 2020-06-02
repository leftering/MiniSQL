#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_ 

#include "pch.h"
#include "define.h"
#include "catalog.h"
#include "buffer_manager.h"

extern BufferManager buffer_manager;

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
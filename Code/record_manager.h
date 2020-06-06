#pragma once

#include "pch.h"
#include "define.h"
#include "catalog.h"
#include "buffer_manager.h"
#include "tuple.h"
#include "interpreter.h"

extern BufferManager buffer_manager;
extern Interpreter In;

class RecordManager
{
public:

	// Select record from table. Return number of records selected
	int select(std::vector<int> col_ids, std::vector<Where_clause>wheres, std::vector<int> logic, std::vector<Tuple>* tuples);

	// Insert record into table. Return new index id
	int insert(Tuple record);
	
	// Delete id-th record from table. Return true if success
	int remove(std::vector<Where_clause>wheres, std::vector<int>logic);
	

	// Drop table. Return true if success
	// bool dropTable(const char* table_name);

	// Check if record satisfy all conditions
	bool check(Tuple record, std::vector<Where_clause> wheres, std::vector<int> logic);
	bool check_unique(Tuple record);
private:

	template<typename T> bool cmp(T a, T b, std::string op) {
		if (op == "=")
			return (a == b);
		else if (op == "<>")
			return (a != b);
		else if (op == ">=")
			return (a >= b);
		else if (op == "<=")
			return (a <= b);
		else if (op == ">")
			return (a > b);
		else if (op == "<")
			return (a < b);
		else
			return false;
	};
	Tuple read2tuple(BYTE* record, table_info T);
	void insert2block(BYTE* data, std::vector<Data> records, short record_size, short free_space);
	void remove4block(BYTE* data, int record_id, int col_num);
};
#pragma once
#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_
#include "tuple.h"
#include "pch.h"
#include "interpreter.h"
#include "buffer_manager.h"
#include "catalog.h"

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
	bool check_unique(table_info T, Tuple record);
	Tuple read2tuple(BYTE* record, table_info T);
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
	void insert2block(BYTE* data, std::vector<Data> records, short record_size, short free_space);
	void remove4block(BYTE* data, int record_id, int col_num);
	address find_addr_low(table_info T, std::string attributename, Where_clause where, int attr_index);
	address find_addr_up(table_info T, std::string attributename, Where_clause where, int attr_index);
};

std::string get_ith_value(table_info T, BYTE record[], int i);
#endif
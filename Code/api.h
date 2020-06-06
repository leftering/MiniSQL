#pragma once
#include "interpreter.h"
#include "catalog.h"
#include "pch.h"
#include "record_manager.h"
extern RecordManager record_manager;

bool create_table(Interpreter* in);

int insert_record(string table_name, string values[]);

bool execfile(Interpreter* in, string file_name);

bool get_col_ids(vector<int>* col_ids, string str, string table_name, Interpreter* in);

bool api_select(string table_name, vector<int> col_ids, vector<Where_clause> w_clause, vector<int> logic);

bool api_delete(string table_name, vector<Where_clause> w_clouse, vector<int> logic);

bool is_unique(string table_name, string col_name);

#ifndef API_H
#define API_H
#include "interpreter.h"
#include "pch.h"

bool create_table(Interpreter* in);

bool insert_record(string table_name, string values[]);

bool execfile(Interpreter* in,string file_name);

bool get_col_ids(vector<int>* col_ids, string str, string table_name, Interpreter* in);

bool api_select(string table_name, vector<int> col_ids, vector<Where_clouse> w_clause, vector<int> logic);

#endif // !API_H

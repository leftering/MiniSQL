#ifndef API_H
#define API_H
#include "interpreter.h"
#include "catalog.h"
#include "pch.h"
#include "record_manager.h"
#include "tuple.h"
#include "define.h"
extern RecordManager record_manager;

bool create_table(Interpreter* in);

bool insert_record(string table_name, string values[]);

bool execfile(Interpreter* in,string file_name);

<<<<<<< HEAD
=======
bool get_col_ids(vector<int>* col_ids, string str, string table_name, Interpreter* in);

bool select(string table_name, vector<int> col_ids, vector<Where_clouse> w_clause, vector<int> logic);

>>>>>>> parent of 9f280bb... add api select
#endif // !API_H

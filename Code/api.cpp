#include "interpreter.h"
#include "catalog.h"
#include "pch.h"
#include "api.h"
#include "record_manager.h"

bool create_table(Interpreter* in) {
  if (in->table.write_table_info())
	return true;
  else
	return false;
}

bool insert_record(string table_name, string values[]) {
  table_info table;
  int type[32];
  char data[32 * 8];
  int now = 0;
  if (table.get_table_info(table_name)) {
	for (int i = 0;i < table.col_num;i++) {
	  if (table.col[i].col_type == COL_INT) {
		int v = stoi(values[i]);
		memcpy(data + now, &v, sizeof(v));
		type[i] = -2;
		now += sizeof(v);
	  }
	  else if (table.col[i].col_type == COL_FLOAT) {
		float v = stof(values[i]);
		memcpy(data + now, &v, sizeof(v));
		type[i] = -1;
		now += sizeof(v);
	  }
	  else if (table.col[i].col_type == COL_CHAR) {
		const char *v = values[i].c_str();
		memcpy(data + now, v, sizeof(v));
		type[i] = sizeof(v);
		now += sizeof(v);
	  }
	  else {
		return false;
	  }
	}
	// call insert;
	return true;
  }
  else {
	return false;
  }
}

bool execfile(Interpreter* in,string file_name) {
  ifstream fin(file_name);
  if (!fin) {
	cout << file_name;
	return false;
  }
  streambuf *oldcin = cin.rdbuf(fin.rdbuf());
  while (!cin.eof()) {
	in->read_operation();
  }
  cin.rdbuf(oldcin);
  fin.close();
  return true;
}

bool get_col_ids(vector<int>* col_ids, string str, string table_name, Interpreter* in) {
  if (!in->table.get_table_info(table_name)) {
	return false;
  }
  if (str == "*") {
	for (int i = 0;i < in->table.col_num;i++) {
	  col_ids->push_back(i);
	}
	return true;
  }
  else {
	for (int i = 0;i < in->table.col_num;i++) {
	  if (str.find(in->table.col[i].col_name) != -1) {
		col_ids->push_back(i);
	  }
	}
	return true;
  }
}

bool api_select(string table_name, vector<int> col_ids, vector<Where_clouse> w_clause, vector<int> logic) {
  //call select in record manager here;
  return true;
}

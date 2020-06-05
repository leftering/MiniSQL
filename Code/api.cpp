#include "api.h"
bool create_table(Interpreter* in) {
	if (in->table.write_table_info()) {
		std::string filename = RECORDPATH + in->table.table_name + ".data";
		FILE* fp = fopen(filename.c_str(), "w");
		fclose(fp);
		return true;
  }
  else
	return false;
}

bool insert_record(string table_name, string values[]) {
  table_info table;
  Tuple record;
  if (table.get_table_info(table_name)) {
	for (int i = 0;i < table.col_num;i++) {
	  Data data;
	  if (table.col[i].col_type == COL_INT) {
		int v = stoi(values[i]);
		data.type = -2;
		data.datai = v;
		record.addData(data);
	  }
	  else if (table.col[i].col_type == COL_FLOAT) {
		float v = stof(values[i]);
		data.type = -1;
		data.dataf = v;
		record.addData(data);
	  }
	  else if (table.col[i].col_type == COL_CHAR) {
		std::string v = values[i];
		data.type = v.length();
		data.datas = v;
		record.addData(data);
	  }
	  else {
		return false;
	  }
	}
	record_manager.insert(table_name, record);
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
<<<<<<< HEAD
=======

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

bool select(string table_name, vector<int> col_ids, vector<Where_clouse> w_clause, vector<int> logic) {
  //call select in record manager here;
  return true;
}
>>>>>>> parent of 9f280bb... add api select

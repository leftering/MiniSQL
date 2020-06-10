#include "api.h"
#include "catalog.h"
#include "tuple.h"
#include "define.h"

extern clock_t finish, start;

bool create_table(Interpreter* in) {
	if (in->table.write_table_info()) {
		std::string filename = RECORDPATH + in->table.table_name + ".data";
		FILE* fp = fopen(filename.c_str(), "wb");
		fclose(fp);
		return true;
	}
	else
		return false;
}

int insert_record(string values[], int num) {
	static table_info table = In.table;
	std::string table_name = table.table_name;
	Tuple record;
	if (num < table.col_num) {
		return 2013;
	}
	if (table.table_name == table_name || table.get_table_info(table_name)) {
		for (int i = 0; i < table.col_num; i++) {
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
		table_info T = In.table;
		int cnt = record_manager.insert(record); //insert without index
		// call insert with index here
		if (cnt <= 0) {
			return cnt;
		}
		else {
			cout << cnt << " record(s) inserted" << endl;
		}
		return true;
	}
	else {
		return -1;
	}
}

bool execfile(Interpreter* in, string file_name) {
	ifstream fin(file_name);
	if (!fin) {
	  cout << "Error: " << strerror(errno);
	  system("pause");
		cout << file_name;
		return false;
	}
	streambuf* oldcin = cin.rdbuf(fin.rdbuf());
	while (!cin.eof()) {
		in->read_operation();
	}
	cin.rdbuf(oldcin);
	fin.close();
	fin.clear();
	return true;
}

bool get_col_ids(vector<int>* col_ids, string str, string table_name, Interpreter* in) {
	if (!in->table.get_table_info(table_name)) {
		return false;
	}
	if (str == "*") {
		for (int i = 0; i < in->table.col_num; i++) {
			col_ids->push_back(i);
		}
		return true;
	}
	else {
		for (int i = 0; i < in->table.col_num; i++) {
			if (str.find(in->table.col[i].col_name) != -1) {
				col_ids->push_back(i);
			}
		}
		return true;
	}
}

bool api_select(string table_name, vector<int> col_ids, vector<Where_clause> w_clause, vector<int> logic) {
	// below are select without index, add index selet  here
	//call select in record manager here;
	std::vector<Tuple> tuples;
	int cnt = record_manager.select(col_ids, w_clause, logic, &tuples);
	finish = clock();
	if (cnt <= 0) {
		return false;
	}
	static table_info T;
	if (T.table_name != table_name)
		T.get_table_info(table_name);
	for (int i = 0; i < col_ids.size(); i++) {
		cout << "-----------------";
	}
	cout << endl;
	for (int i = 0; i < col_ids.size(); i++) {
		std::cout << setw(16) << setfill(' ') << left << T.col[col_ids[i]].col_name << '|';
	}
	cout << endl;
	for (int i = 0; i < col_ids.size(); i++) {
		cout << "----------------|";
	}
	cout << endl;
	for (int i = 0; i < tuples.size(); i++) {
		if (tuples[i].isDeleted() == false) {
			tuples[i].showTuple();
			cout << endl;
		}
	}
	for (int i = 0; i < col_ids.size(); i++) {
		cout << "-----------------";
	}
	cout << endl;
	cout << cnt << " records selected" << endl;
	return true;
}

int api_delete(string table_name, vector<Where_clause> w_clouse, vector<int> logic) {
	if (!In.table.get_table_info(table_name)) {
	  return -1;
	}
	int cnt;
	cnt = record_manager.remove(w_clouse, logic);
	// call index delete here
	if (cnt < 0) {
		return false;
	}
	else {
		cout << cnt << " record(s) deleted" << endl;
	}
	return true;
}


bool is_unique(string table_name, string col_name) {
  table_info table;
  if (!table.get_table_info(table_name)) {
	return false;
  }
  for (int i = 0;i < table.col_num;i++) {
	if (col_name == table.col[i].col_name && table.col[i].unique == 1) {
	  return true;
	}
  }
  return false;
}


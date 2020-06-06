#include "api.h"

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

bool insert_record(string table_name, string values[]) {
	static table_info table;
	Tuple record;
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
		record_manager.insert(table_name, record);
		return true;
	}
	else {
		return false;
	}
}

bool execfile(Interpreter* in, string file_name) {
	ifstream fin(file_name);
	if (!fin) {
		cout << file_name;
		return false;
	}
	streambuf* oldcin = cin.rdbuf(fin.rdbuf());
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
	//call select in record manager here;
	std::vector<Tuple> tuples;
	int cnt = record_manager.select(table_name, col_ids, w_clause, logic, &tuples);
	if (cnt <= 0) {
		return false;
	}
	static table_info T;
	if(T.table_name != table_name)
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
		if (tuples[i].isDeleted() == true) {
			continue;
		}
		tuples[i].showTuple();
		cout << endl;
	}
	for (int i = 0; i < col_ids.size(); i++) {
		cout << "-----------------";
	}
	cout << endl;
	return true;
}

bool api_delete(string table_name, vector<Where_clause> w_clouse, vector<int> logic) {
	int cnt = record_manager.remove(table_name, w_clouse, logic);
	
	cout << cnt << " recotd(s) deleted. " << endl;

	return true;
}

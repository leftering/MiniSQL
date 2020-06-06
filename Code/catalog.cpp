#include "catalog.h"
#include "pch.h"

bool table_info::write_table_info() {
	ofstream fout(this->table_name);
	if (!fout) {
		return false;
	}
	else {
		fout << this->table_name << endl;
		fout << this->col_num << endl;
		for (int i = 0; i < this->col_num; i++) {
			fout << this->col[i].col_name << '\t';
			fout << this->col[i].col_type << '\t';
			fout << this->col[i].char_length << '\t';
			fout << this->col[i].have_index << '\t';
			fout << this->col[i].primary_key << '\t';
			fout << this->col[i].unique << '\t';
			fout << endl;
		}
	}
	fout.close();
	return true;
}

bool table_info::get_table_info(string table_name) {
	ifstream fin(table_name);
	if (!fin) {
	  cout << "Error: " << strerror(errno);
	  system("pause");
		return false;
	}
	else {
		fin >> this->table_name;
		fin >> this->col_num;
		for (int i = 0; i < this->col_num; i++) {
			fin >> this->col[i].col_name;
			int type;
			fin >> type;
			this->col[i].col_type = (colunm_type)type;
			fin >> this->col[i].char_length;
			fin >> this->col[i].have_index;
			fin >> this->col[i].primary_key;
			fin >> this->col[i].unique;
		}
	}
	fin.close();
	fin.clear();
	return true;


	// static FILE *fp = NULL;
	// static string fname = "";
	// if (fname != table_name)
	   //fp = fopen(table_name.c_str(), "r");
	// fname = table_name;
	// if (!fp) {
	   //cout << "Error: " << strerror(errno);
	   //system("pause");
	   //return false;
	// }
	// fseek(fp, 0, SEEK_SET);
	// fscanf(fp, "%s%d", this->table_name, &(this->col_num));
	// for (int i = 0;i < this->col_num;i++) {
	   //fscanf(fp, "%s%d%d%d%d%d", this->col[i].col_name, &(this->col[i].col_type), &(this->col[i].char_length), &(this->col[i].have_index), &(this->col[i].primary_key), &(this->col[i].unique));
	// }
	// fclose(fp);
	// return true;
}

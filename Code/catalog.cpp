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
	for (int i = 0;i < this->col_num;i++) {
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
  cout << table_name << endl;
  if (!fin) {
	return false;
  }
  else {
	fin >> this->table_name;
	fin >> this->col_num;
	for (int i = 0;i < this->col_num;i++) {
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
  return true;
}

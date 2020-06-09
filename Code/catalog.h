#pragma once
#ifndef _CATALOG_H_
#define _CATALOG_H_

#include "pch.h"
typedef enum {
  COL_INT = 0,
  COL_FLOAT,
  COL_CHAR
}colunm_type; // data type

class column_info {
public:
  string col_name;
  colunm_type col_type;
  int char_length;
  bool unique;
  bool primary_key;
  bool have_index;
  column_info() {};
};

class table_info {
public:
  string table_name;
  int col_num;
  column_info col[32];	// a table at most have 32 columns
  table_info() :table_name("") {};
  bool get_table_info(string table_name);
  bool write_table_info();
};

#endif // !API_H

#ifndef CATALOG_H
#define CATALOG_H

#include "pch.h"

typedef enum {
  COL_INT = 0,
  COL_FLOAT,
  COL_CHAR
}colunm_type;

class column_info {
public:
  string col_name;
  colunm_type col_type;
  int char_length;
  bool unique;
  bool primary_key;
  column_info() {};
};

class table_info {
public:
  string table_name;
  int col_num;
  column_info col[32];
  table_info() {};
};

#endif // !API_H

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
	  return true;
	}
	// call record;
  }
  else {
	return false;
  }
}

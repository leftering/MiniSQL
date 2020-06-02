#include "interpreter.h"
#include "catalog.h"
#include "pch.h"
#include "api.h"

bool Create_table(Interpreter* in) {
  if (in->table.write_table_info())
	in->table.get_table_info("stu1");
  in->table.write_table_info();
	return true;
  //else
	return false;
}

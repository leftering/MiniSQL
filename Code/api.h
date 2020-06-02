#ifndef API_H
#define API_H
#include "interpreter.h"

bool create_table(Interpreter* in);

bool insert_record(string table_name, string values[]);

bool execfile(Interpreter* in,string file_name);

#endif // !API_H

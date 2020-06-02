#ifndef API_H
#define API_H
#include "interpreter.h"

bool create_table(Interpreter* in);

bool insert_record(string table_name, string values[]);

#endif // !API_H

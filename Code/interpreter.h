#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "pch.h"
#include "catalog.h"

typedef enum {
  CREATE_TABLE = 0,
  DROP_TABLE,
  CREATE_INDEX,
  DROP_INDEX,
  SELECT,
  INSERT,
  DELETE,
  QUIT,
  EXECFILE,
  EMPTY
}operation_type; // sql operation type

typedef enum {
  READING = 0,
  FINISH,
  ERROR
}user_status; // user input status

struct Where_clouse {
  string attr;
  string value;
  string operation;
};

struct DB_exception{
  int code;
  string title;
  string msg;
};

class Interpreter {
public:

  user_status status;
  operation_type operation;
  table_info table;
  Where_clouse w_clouse;
  clock_t start, finish;
  DB_exception error;

  void read_operation(); // read sql sentense

  void log_status();

  void set_error(int code);

  Interpreter();

  ~Interpreter();

};

#endif // !INTERPRETER_H

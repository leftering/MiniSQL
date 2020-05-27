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

class Interpreter {
public:

  user_status status;
  operation_type operation;
  table_info table;

  void read_operation(); // read sql sentense

  Interpreter();

  ~Interpreter();

};

#endif // !INTERPRETER_H

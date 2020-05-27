#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "pch.h"

typedef enum {
  CREATE_TABLE = 0,
  DROP_TABLE,
  CREATE_INDEX,
  DROP_INDEX,
  SELECT,
  INSERT,
  DELETE,
  QUIT
}operation_type;

typedef enum {
  READING = 0,
  FINISH,
  ERROR
}user_status;

class Interpreter {
public:
  user_status status;
  operation_type operation;

  void read_operation();

  Interpreter();

  ~Interpreter();

private:
  void get_operation(string command);
};

#endif // !INTERPRETER_H

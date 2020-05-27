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

  void read_operation(); // read sql sentense

  Interpreter();

  ~Interpreter();

private:

  string get_word(string command,int* position); // get a word for an sentense from the position

};

#endif // !INTERPRETER_H

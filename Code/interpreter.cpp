#include "pch.h"
#include "interpreter.h"

bool is_break_char(char ch);

Interpreter::Interpreter() {
  this->status = READING;
  this->operation = EMPTY;
}

Interpreter::~Interpreter() {

}

void Interpreter::read_operation() {
  string command;
  int position = 0;
  getline(cin, command, ';');
  transform(command.begin(), command.end(), command.begin(), tolower);
  string operation = this->get_word(command, &position);
  if (strcmp(operation.c_str(), "ERROR") == 0) {
	this->status = ERROR;
	this->operation = EMPTY;
  }
  else if (strcmp(operation.c_str(), "create") == 0) {
	string create_type = get_word(command, &position);
	if (strcmp(create_type.c_str(), "table") == 0) {
	  this->operation = CREATE_TABLE;
	}
	else if (strcmp(create_type.c_str(), "index") == 0) {
	  this->operation = CREATE_INDEX;
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "drop") == 0) {
	string drop_type = get_word(command, &position);
	if (strcmp(drop_type.c_str(), "table") == 0) {
	  this->operation = DROP_TABLE;
	}
	else if (strcmp(drop_type.c_str(), "index") == 0) {
	  this->operation = DROP_INDEX;
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "select") == 0) {
	this->operation = SELECT;
  }
  else if (strcmp(operation.c_str(), "delete") == 0) {
	this->operation = DELETE;
  }
  else if (strcmp(operation.c_str(), "insert") == 0) {
	this->operation = INSERT;
  }
  else if (strcmp(operation.c_str(), "execfile") == 0) {
	this->operation = EXECFILE;
  }
  else if (strcmp(operation.c_str(), "quit") == 0) {
	this->operation = QUIT;
	this->status = FINISH;
  }
  else {
	this->status = ERROR;
	this->operation = EMPTY;
  }
}

string Interpreter::get_word(string command,int *position) {
  if (*position == command.length())
	return "ERROR";
  int L = *position, R;
  while (is_break_char(command[L])) {
	L++;
  }
  R = L;
  while (!is_break_char(command[R])) {
	R++;
  }
  if (R == L) {
	return "ERROR";
  }
  else {
	*position = R;
	return command.substr(L, R - L);
  }
}

bool is_break_char(char ch) {
  if (ch == ' ' || ch == '\n' || ch == ',' || ch == '\0')
	return true;
  return false;
}
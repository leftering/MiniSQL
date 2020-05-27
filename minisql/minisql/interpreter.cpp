#include "pch.h"
#include "interpreter.h"

Interpreter::Interpreter() {
  this->status = READING;
}

Interpreter::~Interpreter() {

}

void Interpreter::read_operation() {
  string command;
  transform(command.begin(), command.end(), command.begin(), tolower);
  getline(cin, command, ';');
  this->get_operation(command);
}

void Interpreter::get_operation(string command) {
  int L = -1, R = -1;
  for (int i = 0;i < command.length();i++) {
	if (command[i] != ' ') {
	  L = i;
	  break;
	}
  }
  for (int i = L;i < command.length();i++) {
	if (command[i] == ' ') {
	  R = i;
	  break;
	}
  }
  if (L == -1 || R == -1) {
	this->status = ERROR;
	return;
  }
  string operation = command.substr(L, R - L);
  if (strcmp(operation.c_str(), "select") == 0) {
	this->operation = SELECT;
  }
  else {
	this->status = ERROR;
  }
}
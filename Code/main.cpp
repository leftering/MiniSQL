#include "pch.h"
#include "interpreter.h"

int main() {
  Interpreter In;

  while (true) { // event loop
	cout << "MiniSQL >> ";
	In.status = READING;
	In.operation = EMPTY;
	In.read_operation();
	if (In.status != ERROR) {
	  cout << "Operation: " << In.operation << endl;
	  if (In.operation == QUIT) {
		break;
	  }
	  if (In.operation == CREATE_TABLE) {
		cout << "create table" << endl;
	  }
	}
	else{
	  cout << "ERROR" << endl;
	}
  }


  return 0;
}
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
	  if (In.operation == QUIT) {
		break;
	  }
	  if (In.operation == CREATE_TABLE) {
		cout << "create table" << endl;
	  }
	  if (In.operation == CREATE_INDEX) {
		cout << "create index" << endl;
	  }
	  if (In.operation == DROP_TABLE) {
		cout << "drop table" << endl;
	  }
	  if (In.operation == DROP_INDEX) {
		cout << "drop index" << endl;
	  }
	  if (In.operation == SELECT) {
		cout << "select" << endl;
	  }
	  if (In.operation == DELETE) {
		cout << "delete" << endl;
	  }
	  if (In.operation == INSERT) {
		cout << "insert" << endl;
	  }
	  if (In.operation == EXECFILE) {
		cout << "execfile" << endl;
	  }
	  if (In.operation == EMPTY) {
		cout << "empty" << endl;
	  }
	}
	else{
	  cout << "ERROR" << endl;
	}
  }


  return 0;
}
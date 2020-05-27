#include "pch.h"
#include "interpreter.h"

int main() {
  Interpreter In;

  while (1) { // event loop
	cout << "MiniSQL >> ";
	In.status = READING;
	In.operation = EMPTY;
	In.read_operation();
	if (In.status != ERROR) {
	  if (In.operation == QUIT) {
		break;
	  }
	  cout << "Operation: " << In.operation << "\n";
	}
	else if (In.status == ERROR) {
	  cout << "ERROR\n";
	}
  }


  return 0;
}
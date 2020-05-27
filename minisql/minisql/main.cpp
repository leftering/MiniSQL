#include "pch.h"
#include "interpreter.h"

int main() {
  Interpreter In;

  while (1) { // event loop
	cout << "MiniSQL >> ";
	In.status = READING;
	In.read_operation();
	if (In.status != ERROR) {
	  cout << "Operation: " << In.operation << "\n";
	}
	else if (In.status == ERROR) {
	  cout << "ERROR:xxx\n";
	}
  }


  return 0;
}
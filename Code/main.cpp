#include "pch.h"
#include "interpreter.h"
#include "record_manager.h"
#include "buffer_manager.h"

int main() {
  Interpreter In;
  BufferManager buffer_manager;

  while (true) { // event loop
	cout << "MiniSQL >> ";
	In.status = READING;
	In.operation = EMPTY;
	In.read_operation();
	if (In.operation == QUIT) {
	  break;
	}
  }
  return 0;
}
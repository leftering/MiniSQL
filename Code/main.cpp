#include "pch.h"
#include "interpreter.h"
#include "record_manager.h"
#include "buffer_manager.h"

BufferManager buffer_manager;

int main() {
  Interpreter In;

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
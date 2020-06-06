#include "pch.h"
#include "interpreter.h"
#include "record_manager.h"
#include "buffer_manager.h"

BufferManager buffer_manager;
RecordManager record_manager;
Interpreter In;

int main() {
  cout << "Welcome to MiniSQL !" << endl << endl;
  while (true) { // event loop
	cout << "MiniSQL >> ";
	In.status = READING;
	In.operation = EMPTY;
	In.read_operation();
	if (In.operation == QUIT) {
	  break;
	}
  }
  cout << "Quit MiniSQL, bye !" << endl << endl;
  return 0;
}
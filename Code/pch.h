#ifndef PCH_H_
#define PCH_H_

//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include <cctype>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <map>
typedef unsigned char BYTE;
using namespace std;

typedef struct addr* address;
struct addr
{
	int block_id;//page is block.
	int record_id;//record_id;
	address last_addr;
	address next_addr;
};

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

struct Where_clause {
	string attr;
	string value;
	string operation;
};

struct DB_exception {
	int code;
	string title;
	string msg;
};

#endif // !PCH_H

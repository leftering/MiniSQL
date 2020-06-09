#pragma once
#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "pch.h"
#include "catalog.h"

class Interpreter {
public:
	user_status status;
	operation_type operation;
	table_info table;
	vector<Where_clause> w_clouse;
	clock_t start, finish;
	DB_exception error;

	void read_operation(); // read sql sentense

	void log_status(clock_t start, clock_t finish);

	void set_error(int code);

	Interpreter();

	~Interpreter();

};
#endif // !_INTERPRETER_H

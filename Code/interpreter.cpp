#include "interpreter.h"

string get_word(string command, int& position);
bool is_break_char(char ch);
string get_brackets(string command,int& position);
bool parse_cols(string cols_str, string table_name, Interpreter* in);
string get_comma(string str, int& position , bool& end);
bool set_primary(string col_name, Interpreter* in);

Interpreter::Interpreter() {
  this->status = READING;
  this->operation = EMPTY;
}

Interpreter::~Interpreter() {}

void Interpreter::read_operation() {
  string command;
  int position = 0;
  getline(cin, command, ';');
  transform(command.begin(), command.end(), command.begin(), tolower);
  string operation = get_word(command, position);
  if (strcmp(operation.c_str(), "ERROR") == 0) {
	this->status = ERROR;
	this->operation = EMPTY;
  }
  else if (strcmp(operation.c_str(), "create") == 0) {
	string create_type = get_word(command, position);
	if (strcmp(create_type.c_str(), "table") == 0) {
	  string new_table_name = get_word(command, position);
	  string cols_info = get_brackets(command, position);
	  if (parse_cols(cols_info, new_table_name, this)) {
		this->operation = CREATE_TABLE;
	  }
	  else {
		this->status = ERROR;
		this->operation = EMPTY;
	  }
	}
	else if (strcmp(create_type.c_str(), "index") == 0) {
	  this->operation = CREATE_INDEX;
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "drop") == 0) {
	string drop_type = get_word(command, position);
	if (strcmp(drop_type.c_str(), "table") == 0) {
	  this->operation = DROP_TABLE;
	}
	else if (strcmp(drop_type.c_str(), "index") == 0) {
	  this->operation = DROP_INDEX;
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "select") == 0) {
	this->operation = SELECT;
  }
  else if (strcmp(operation.c_str(), "delete") == 0) {
	this->operation = DELETE;
  }
  else if (strcmp(operation.c_str(), "insert") == 0) {
	this->operation = INSERT;
  }
  else if (strcmp(operation.c_str(), "execfile") == 0) {
	this->operation = EXECFILE;
  }
  else if (strcmp(operation.c_str(), "quit") == 0) {
	this->operation = QUIT;
	this->status = FINISH;
  }
  else {
	this->status = ERROR;
	this->operation = EMPTY;
  }
}

string get_word(string command,int& position) {
  if (position == command.length())
	return "ERROR";
  int L = position, R;
  while (is_break_char(command[L])) {
	L++;
  }
  R = L;
  while (!is_break_char(command[R])) {
	R++;
  }
  if (R == L) {
	return "ERROR";
  }
  else {
	position = R;
	return command.substr(L, R - L);
  }
}

bool is_break_char(char ch) {
  if (ch == ' ' || ch == '\n' || ch == ',' || ch == '\0' || ch == '(' || ch == ')')
	return true;
  return false;
}

string get_brackets(string command, int& position) {
  if (position == command.length())
	return "ERROR";
  int L = position, R;
  while (is_break_char(command[L]) && command[L] != '(') {
	L++;
  }
  if (command[L] != '(') {
	return "ERROR";
  }
  R = L;
  while (command[R] != ')' && R < command.length()) {
	R++;
  }
  if (command[R] != ')') {
	return "ERROR";
  }
  position = R;
  return command.substr(L + 1, R - L -1);
}

bool parse_cols(string cols_str, string table_name, Interpreter* in) {
  in->table.table_name = table_name;
  in->table.col_num = 0;
  int position = 0;
  bool end = false;
  while (!end) {
	string col = get_comma(cols_str, position, end);
	if (strcmp(col.c_str(), "ERROR") == 0) {
	  return false;
	}
	else {
	  int col_position = 0;
	  string name = get_word(col, col_position);
	  if (strcmp(name.c_str(), "ERROR") == 0) {
		return false;
	  }
	  if (strcmp(name.c_str(), "primary") == 0) {
		name = get_word(col, col_position);
		if (strcmp(name.c_str(), "key") == 0) {
		  name = get_word(col, col_position);
		  if (strcmp(name.c_str(), "ERROR") == 0) {
			return false;
		  }
		  else {
			if (!set_primary(name, in)) {
			  return false;
			}
		  }
		}
		else {
		  return false;
		}
	  }
	  else {
		string typestr = get_word(col, col_position);
		colunm_type type;
		int char_length = 0;
		bool unique = false;
		if (strcmp(typestr.c_str(), "ERROR") == 0) {
		  return false;
		}
		else if (strcmp(typestr.c_str(), "int") == 0) {
		  type = COL_INT;
		  if (strcmp(get_word(col, col_position).c_str(), "unique") == 0) {
			unique = true;
		  }
		}
		else if (strcmp(typestr.c_str(), "float") == 0) {
		  type = COL_FLOAT;
		  if (strcmp(get_word(col, col_position).c_str(), "unique") == 0) {
			unique = true;
		  }
		}
		else if (strcmp(typestr.c_str(), "char") == 0) {
		  type = COL_CHAR;
		  string char_length_str = get_word(col, col_position);
		  if (strcmp(char_length_str.c_str(), "ERROR") == 0) {
			return false;
		  }
		  char_length = atoi(char_length_str.c_str());
		  if (char_length == 0) {
			return false;
		  }
		  if (strcmp(get_word(col, col_position).c_str(), "unique") == 0) {
			unique = true;
		  }
		}
		else {
		  return false;	
		}
		in->table.col[in->table.col_num].col_name = name;
		in->table.col[in->table.col_num].col_type = type;
		in->table.col[in->table.col_num].char_length = char_length;
		in->table.col[in->table.col_num].primary_key = false;
		in->table.col[in->table.col_num].unique = unique;
		in->table.col_num++;
	  }
	}
  }
  return true;
}

string get_comma(string str, int& position, bool& end) {
  int L = position + 1, R;
  if (L >= str.length()) {
	return "ERROR";
  }
  R = L;
  while (str[R] != ',' && R < str.length()) {
	R++;
  }
  position = R;
  if (position == str.length()) {
	end = true;
  }
  return str.substr(L, R - L);
}

bool set_primary(string col_name, Interpreter* in) {
  for (int i = 0;i < in->table.col_num;i++) {
	if (in->table.col[i].col_name == col_name) {
	  in->table.col[i].primary_key = true;
	  return true;
	}
  }
  return false;
}

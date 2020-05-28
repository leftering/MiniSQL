#include "interpreter.h"

string get_word(string command, int& position);	// get next word from postion
bool is_break_char(char ch);  // whether ch is a break char
string get_brackets(string command,int& position);	// get string in (), without ()
bool parse_cols(string cols_str, string table_name, Interpreter* in); // parse create table columns
string get_comma(string str, int& position , bool& end);  // get next string form positon, from ',' to next ','
bool set_primary(string col_name, Interpreter* in);	// set 'col_name' as primary key

const string str_ERROR = "ERROR";

Interpreter::Interpreter() { 
  this->status = READING;
  this->operation = EMPTY;
}

Interpreter::~Interpreter() {}

void Interpreter::read_operation() {
  string command;
  int position = 0;
  getline(cin, command, ';'); // command = SQL sentence
  transform(command.begin(), command.end(), command.begin(), tolower);	// all words in lowercase
  string operation = get_word(command, position); // get first word ( 'create', 'drop', 'select', 'delete', 'insert', 'execfile', 'quit' )
  if (operation == str_ERROR) {
	this->status = ERROR;
	this->operation = EMPTY;
  }
  else if (strcmp(operation.c_str(), "create") == 0) {
	string create_type = get_word(command, position);
	if (strcmp(create_type.c_str(), "table") == 0) {  // create table
	  string new_table_name = get_word(command, position);	// get table name
	  string cols_info = get_brackets(command, position); // get all columns info
	  if (parse_cols(cols_info, new_table_name, this)) {  // parse columns info
		// call API here
		this->operation = CREATE_TABLE;
	  }
	  else {
		this->status = ERROR;
		this->operation = EMPTY;
	  }
	}
	else if (strcmp(create_type.c_str(), "index") == 0) { // create index
	  string index_name = get_word(command, position);	// get index name
	  string str_on = get_word(command, position);
	  string table_name = get_word(command, position);	// get table name
	  string col_name = get_word(command, position);  // get index column name
	  cout << index_name << str_on << table_name << col_name;
	  if (index_name != str_ERROR && strcmp(str_on.c_str(), "on") == 0 && table_name != str_ERROR && col_name != str_ERROR) {
		// call API here
		this->operation = CREATE_INDEX;
	  }
	  else {
		this->status = ERROR;
		this->operation = EMPTY;
	  }
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "drop") == 0) {
	string drop_type = get_word(command, position);
	if (strcmp(drop_type.c_str(), "table") == 0) {	// drop table
	  string table_name = get_word(command, position); // get table name
	  if (table_name != str_ERROR) {
		// call API here
		this->operation = DROP_TABLE;
	  }
	  else {
		this->status = ERROR;
		this->operation = EMPTY;
	  }
	}
	else if (strcmp(drop_type.c_str(), "index") == 0) {	// drop index
	  string index_name = get_word(command, position); // get index name
	  if (index_name != str_ERROR) {
		// call API here
		this->operation = DROP_INDEX;
	  }
	  else {
		this->status = ERROR;
		this->operation = EMPTY;
	  }
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "select") == 0) {	// select
	string str_star = get_word(command, position);
	string str_from = get_word(command, position);	// get table name
	string table_name = get_word(command, position);
	if (strcmp(str_star.c_str(), "*") == 0 && strcmp(str_from.c_str(), "from") == 0 && table_name != str_ERROR) {
	  string str_where = get_word(command, position);
	  if (strcmp(str_where.c_str(), "where") == 0) {  // select with where
		string where_clause = command.substr(position);
		// call API here
		this->operation = SELECT;
	  }
	  else {  // select without where
		// call API here
		this->operation = SELECT;
	  }
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "delete") == 0) {	// delete
	string str_from = get_word(command, position);
	string table_name = get_word(command, position);  // get table name
	if (strcmp(str_from.c_str(), "from") == 0 && table_name != str_ERROR) {
	  string str_where = get_word(command, position);
	  if (strcmp(str_where.c_str(), "where") == 0) {  // delete with where
		string where_clause = command.substr(position);
		// call API here
		this->operation = DELETE;
	  }
	  else {  // delete without where
		// call API here
		this->operation = DELETE;
	  }
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "insert") == 0) {	// insert
	string str_into = get_word(command, position);
	string table_name = get_word(command, position);  // get table name
	string str_values = get_word(command, position);
	string values = get_brackets(command, position);  // get insert values
	if (strcmp(str_into.c_str(), "into") == 0 && table_name != str_ERROR && strcmp(str_values.c_str(), "values") == 0 && values != str_ERROR) {
	  // call API here
	  this->operation = INSERT;
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "execfile") == 0) {  // execfile
	string file_name = get_word(command, position); // get file name
	if (file_name != str_ERROR) {
	  // call api here
	  this->operation = EXECFILE;
	}
	else {
	  this->status = ERROR;
	  this->operation = EMPTY;
	}
  }
  else if (strcmp(operation.c_str(), "quit") == 0) {  // quit
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
	return str_ERROR;
  int L = position, R;
  while (is_break_char(command[L])) {
	L++;
  }
  R = L;
  while (!is_break_char(command[R])) {
	R++;
  }
  if (R == L) {
	return str_ERROR;
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
	return str_ERROR;
  int L = position, R;
  while (is_break_char(command[L]) && command[L] != '(') {
	L++;
  }
  if (command[L] != '(') {
	return str_ERROR;
  }
  R = L;
  while (command[R] != ')' && R < command.length()) {
	R++;
  }
  if (command[R] != ')') {
	return str_ERROR;
  }
  position = R;
  return command.substr(L + 1, R - L -1);
}

bool parse_cols(string cols_str, string table_name, Interpreter* in) {
  in->table.table_name = table_name;  // set table name
  in->table.col_num = 0;
  int position = 0;
  bool end = false; // whether all columns are parsed;
  while (!end) {
	string col = get_comma(cols_str, position, end);  // get a column info
	if (col.c_str() == str_ERROR) {
	  return false;
	}
	else {
	  int col_position = 0;
	  string name = get_word(col, col_position);  // get column name
	  if (name == str_ERROR) {
		return false;
	  }
	  if (strcmp(name.c_str(), "primary") == 0) { // name == 'primary' ?
		name = get_word(col, col_position);
		if (strcmp(name.c_str(), "key") == 0) {
		  name = get_word(col, col_position); // get primary key name
		  if (name == str_ERROR) {
			return false;
		  }
		  else {
			if (!set_primary(name, in)) { // set primary key
			  return false;
			}
		  }
		}
		else {
		  return false;
		}
	  }
	  else {
		string typestr = get_word(col, col_position); // get colunm type
		colunm_type type;
		int char_length = 0;
		bool unique = false;
		if (typestr == str_ERROR) {
		  return false;
		}
		else if (strcmp(typestr.c_str(), "int") == 0) { // int
		  type = COL_INT;
		  if (strcmp(get_word(col, col_position).c_str(), "unique") == 0) {	// unique column ?
			unique = true;
		  }
		}
		else if (strcmp(typestr.c_str(), "float") == 0) { // float
		  type = COL_FLOAT;
		  if (strcmp(get_word(col, col_position).c_str(), "unique") == 0) {
			unique = true;
		  }
		}
		else if (strcmp(typestr.c_str(), "char") == 0) {  // char
		  type = COL_CHAR;
		  string char_length_str = get_word(col, col_position);
		  if (char_length_str == str_ERROR) {
			return false;
		  }
		  char_length = atoi(char_length_str.c_str());	// get char length
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
		in->table.col[in->table.col_num].col_name = name; //  all words in this column paresd vaild, save column into table 
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
	return str_ERROR;
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

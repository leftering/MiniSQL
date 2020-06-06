#include "interpreter.h"
#include "api.h"
#include "pch.h"

string get_word(string command, int& position);	// get next word from postion
bool is_break_char(char ch);  // whether ch is a break char
string get_brackets(string command, int& position);	// get string in (), without ()
bool parse_cols(string cols_str, string table_name, Interpreter* in); // parse create table columns
string get_comma(string str, int& position, bool& end);  // get next string form positon, from ',' to next ','
bool set_primary(string col_name, Interpreter* in);	// set 'col_name' as primary key
bool get_where(string where_clause, vector<Where_clause>* w_clouse, vector<int>* logic);
string get_where_op(string where_clouse, int posi);

const string str_ERROR = "ERROR";

Interpreter::Interpreter() {
	this->status = READING;
	this->operation = EMPTY;
}

Interpreter::~Interpreter() {}

void Interpreter::read_operation() {
	clock_t start, finish;
	this->set_error(0);
	string command;
	int position = 0;
	getline(cin, command, ';'); // command = SQL sentence
	if (command.length() < 3)
		return;
	 //cout << "Command: " << command << ";" << endl;
	start = clock();
	transform(command.begin(), command.end(), command.begin(), tolower);	// all words in lowercase
	string operation = get_word(command, position); // get first word ( 'create', 'drop', 'select', 'delete', 'insert', 'execfile', 'quit' )
	if (operation == str_ERROR) {
		this->status = ERROR;
		this->operation = EMPTY;
		this->set_error(0);
	}
	else if (strcmp(operation.c_str(), "create") == 0) {
		string create_type = get_word(command, position);
		if (strcmp(create_type.c_str(), "table") == 0) {  // create table
			string new_table_name = get_word(command, position);	// get table name
			string cols_info = get_brackets(command, position); // get all columns info
			if (parse_cols(cols_info, new_table_name, this)) {  // parse columns info and create table
				this->operation = CREATE_TABLE;
			}
			else {
				this->status = ERROR;
				this->operation = EMPTY;
				this->set_error(2001); // syntax error table info error
			}
		}
		else if (strcmp(create_type.c_str(), "index") == 0) { // create index
			string index_name = get_word(command, position);	// get index name
			string str_on = get_word(command, position);
			string table_name = get_word(command, position);	// get table name
			string col_name = get_word(command, position);  // get index column name
			cout << index_name << str_on << table_name << col_name;
			if (index_name != str_ERROR && strcmp(str_on.c_str(), "on") == 0 && table_name != str_ERROR && col_name != str_ERROR) {
				// call create index
			  if (is_unique(table_name, col_name))
				;
			  else
				cout << "not unique key" << endl;

			  for (int i = 0;i < 100000000;i++);
				this->operation = CREATE_INDEX;
			}
			else {
				this->status = ERROR;
				this->operation = EMPTY;
				this->set_error(2002); // syntax error create index 
			}
		}
		else {
			this->status = ERROR;
			this->operation = EMPTY;
			this->set_error(2003); //syntax error create;
		}
	}
	else if (strcmp(operation.c_str(), "drop") == 0) {
		string drop_type = get_word(command, position);
		if (strcmp(drop_type.c_str(), "table") == 0) {	// drop table
			string table_name = get_word(command, position); // get table name
			if (table_name != str_ERROR) {
				// call drop table
			  remove(table_name.c_str());
			  this->operation = DROP_TABLE;
			}
			else {
				this->status = ERROR;
				this->operation = EMPTY;
				this->set_error(2010);
			}
		}
		else if (strcmp(drop_type.c_str(), "index") == 0) {	// drop index
			string index_name = get_word(command, position); // get index name
			if (index_name != str_ERROR) {
			  //drop index
				this->operation = DROP_INDEX;
			}
			else {
				this->status = ERROR;
				this->operation = EMPTY;
				this->set_error(2009);
			}
		}
		else {
			this->status = ERROR;
			this->operation = EMPTY;
			this->set_error(2008);
		}
	}
	else if (strcmp(operation.c_str(), "select") == 0) {	// select
		string str_star = get_word(command, position);
		string str_from = get_word(command, position);	// get table name
		string table_name = get_word(command, position);
		if (str_star != str_ERROR && strcmp(str_from.c_str(), "from") == 0 && table_name != str_ERROR) {
			string str_where = get_word(command, position);
			vector<int> col_ids;
			col_ids.clear();
			get_col_ids(&col_ids, str_star, table_name, this);
			if (strcmp(str_where.c_str(), "where") == 0) {  // select with where
				string where_clause = command.substr(position);
				vector<int> logic;
				logic.clear();
				this->w_clouse.clear();
				get_where(where_clause, &this->w_clouse, &logic);
				api_select(table_name, col_ids, this->w_clouse, logic);
				this->operation = SELECT;
			}
			else {  // select without where
				vector<Where_clause> wheres;
				vector<int>logic;
				api_select(table_name, col_ids, wheres, logic);
				this->operation = SELECT;
			}
		}
		else {
			this->status = ERROR;
			this->operation = EMPTY;
			this->set_error(2007);
		}
	}
	else if (strcmp(operation.c_str(), "delete") == 0) {	// delete
		string str_from = get_word(command, position);
		string table_name = get_word(command, position);  // get table name
		if (strcmp(str_from.c_str(), "from") == 0 && table_name != str_ERROR) {
			string str_where = get_word(command, position);
			if (strcmp(str_where.c_str(), "where") == 0) {  // delete with where
				string where_clause = command.substr(position);
				int position = 0;
				vector<int> logic;
				logic.clear();
				this->w_clouse.clear();
				get_where(where_clause, &this->w_clouse, &logic);
				if (api_delete(table_name, this->w_clouse, logic) != 1) {
				  this->status = ERROR;
				  this->operation = EMPTY;
				  this->set_error(2015);
				}
				else {
				  this->operation = DELETE;
				}
			}
			else {  // delete without where
				vector<Where_clause> wheres;
				vector<int>logic;
				api_delete(table_name, wheres, logic);
				this->operation = DELETE;
			}
		}
		else {
			this->status = ERROR;
			this->operation = EMPTY;
			this->set_error(2006);
		}
	}
	else if (strcmp(operation.c_str(), "insert") == 0) {	// insert
		string str_into = get_word(command, position);
		string table_name = get_word(command, position);  // get table name
		string str_values = get_word(command, position);
		string value = get_brackets(command, position);  // get insert values
		string values[32];
		bool end = false;
		int i = 0, v_posi = 0;
		while (!end) {
			int zero = 0;
			values[i++] = get_word(get_comma(value, v_posi, end), zero);
			zero = 0;
		}
		if (strcmp(str_into.c_str(), "into") == 0 && table_name != str_ERROR && strcmp(str_values.c_str(), "values") == 0 && value != str_ERROR) {
			if (this->table.get_table_info(table_name) == false) {
				this->set_error(2012);
				return;
		  }
		  int code = insert_record(values, i);
		  if (code == 1) {
			this->operation = INSERT;
		  }
		  else {
			this->status = ERROR;
			this->operation = EMPTY;
			if (code == 0) {
			  this->set_error(2011);
			}
			else if (code == 2013) {
				this->set_error(2013);
			}
			else if (code == -2014) {
			  this->set_error(2014);
			}
		  }
		}
		else {
			this->status = ERROR;
			this->operation = EMPTY;
			this->set_error(2005);
		}
	}
	else if (strcmp(operation.c_str(), "execfile") == 0) {  // execfile
		string file_name = get_word(command, position); // get file name
		if (file_name != str_ERROR && execfile(this, file_name)) {
			this->operation = EXECFILE;
		}
		else {
			this->status = ERROR;
			this->operation = EMPTY;
			this->set_error(2004);
		}
	}
	else if (strcmp(operation.c_str(), "quit") == 0) {  // quit
		this->operation = QUIT;
		this->status = FINISH;
	}
	else {
		this->status = ERROR;
		this->operation = EMPTY;
		this->set_error(2000);
	}
	finish = clock();
	this->log_status(start, finish);
}

string get_word(string command, int& position) {
	if (position == command.length())
		return str_ERROR;
	int L = position, R;
	while (is_break_char(command[L]) && L < command.length()) {
		L++;
	}
	if (L == command.length()) {
	  return str_ERROR;
	}
	R = L;
	while (!is_break_char(command[R]) && R < command.length()) {
		R++;
	}
	if (R <= L) {
		return str_ERROR;
	}
	else {
		position = R;
		return command.substr(L, R - L);
	}
}

bool is_break_char(char ch) {
	if (ch == ' ' || ch == '\n' || ch == '`' || ch == '\0' || ch == '(' || ch == ')' || ch == '\'' || ch == '\t' || ch == '\'' || ch == '<' || ch == '>' || ch == '=')
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
	int ct = 0;
	while (R < command.length()) {
		if (command[R] == '(')
			ct++;
		if (command[R] == ')' && --ct == 0) {
			break;
		}
		R++;
	}
	if (command[R] != ')') {
		return str_ERROR;
	}
	position = R;
	return command.substr(L + 1, R - L - 1);
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
				in->table.col[in->table.col_num].have_index = false;
				in->table.col_num++;
			}
		}
	}
	if (create_table(in)) {
		return true;
	}
	return false;
}

string get_comma(string str, int& position, bool& end) {
	int L = position, R;
	if (L >= str.length()) {
		return str_ERROR;
	}
	R = L;
	while (str[R] != ',' && R < str.length()) {
		R++;
	}
	position = R + 1;
	if (position >= str.length()) {
		end = true;
	}
	return str.substr(L, R - L);
}

bool set_primary(string col_name, Interpreter* in) {
	for (int i = 0; i < in->table.col_num; i++) {
		if (in->table.col[i].col_name == col_name) {
			in->table.col[i].primary_key = true;
			return true;
		}
	}
	return false;
}

void Interpreter::log_status(clock_t start, clock_t finish) {
	if (this->status != ERROR) {
		cout << "Success: ";
		if (this->operation == CREATE_TABLE) {
			cout << "create table" << endl;
		}
		if (this->operation == CREATE_INDEX) {
			cout << "create index" << endl;
		}
		if (this->operation == DROP_TABLE) {
			cout << "drop table" << endl;
		}
		if (this->operation == DROP_INDEX) {
			cout << "drop index" << endl;
		}
		if (this->operation == SELECT) {
			cout << "select" << endl;
		}
		if (this->operation == DELETE) {
			cout << "delete" << endl;
		}
		if (this->operation == INSERT) {
			cout << "insert" << endl;
		}
		if (this->operation == EXECFILE) {
			cout << "execfile" << endl;
		}
		if (this->operation == EMPTY) {
			cout << "empty" << endl;
		}
	}
	else {
		cout << "ERROR: " << this->error.code << " " << this->error.title << endl;
		cout << "message: " << this->error.msg << endl << endl;
	}
	  cout << "( " << ((finish - start) / (double)CLOCKS_PER_SEC) << " Sec" << " )" << endl << endl;
}

void Interpreter::set_error(int code) {
	this->error.code = code;
	switch (code)
	{
	case 2000:
		this->error.title = "WRONG OPERATION";
		this->error.msg = "only operation of ( 'create', 'drop', 'select', 'delete', 'insert', 'execfile', 'quit' ) vaild.";
		break;
	case 2001:
		this->error.title = "SYNTAX ERROR";
		this->error.msg = "create table syntax error.";
		break;
	case 2002:
		this->error.title = "SYNTAX ERROR";
		this->error.msg = "create index syntax error.";
		break;
	case 2003:
		this->error.title = "SYNTAX ERROR";
		this->error.msg = "only ( 'index', 'table' ) can be created.";
		break;
	case 2004:
		this->error.title = "EXECFILE ERROR";
		this->error.msg = "can not open file or file content invaild.";
		break;
	case 2005:
		this->error.title = "INSERT ERROR";
		this->error.msg = "insert syntax error;";
		break;
	case 2006:
		this->error.title = "SYNTAX ERROR";
		this->error.msg = "delete syntax error.";
		break;
	case 2007:
		this->error.title = "SYNTAX ERROR";
		this->error.msg = "selete syntax error.";
		break;
	case 2008:
		this->error.title = "SYNTAX ERROR";
		this->error.msg = "only ( 'index', 'table' ) can be dropped.";
		break;
	case 2009:
		this->error.title = "DROP INDEX ERROR";
		this->error.msg = "index name illegal;";
		break;
	case 2010:
		this->error.title = "DROP TABLE ERROR";
		this->error.msg = "table name illegal;";
		break;
	case 2011:
	  this->error.title = "INSERT ERROR";
	  this->error.msg = "unsupport data type;";
	  break;
	case 2012:
	  this->error.title = "INSERT ERROR";
	  this->error.msg = "table name invaild;";
	  break;
	case 2013:
	  this->error.title = "INSERT ERROR";
	  this->error.msg = "Column count doesn't match value count;";
	  break;
	case 2014:
	  this->error.title = "INSERT ERROR";
	  this->error.msg = "Duplicate entry for unique key;";
	  break;
	case 2015:
	  this->error.title = "DELETE ERROR";
	  this->error.msg = "Table tablename doesn't exist;";
	  break;
	default:
		this->error.title = "UNKNOWN ERROR";
		this->error.msg = "unknown error;";
		break;
	}
}

bool get_where(string where_clause, vector<Where_clause>* w_clouse, vector<int>* logic) {
	bool end = false;
	int posi = 0;
	bool and_or = false;
	while (!end) {
		Where_clause w;
		w.attr = get_word(where_clause, posi);
		w.operation = get_where_op(where_clause, posi);
		w.value = get_word(where_clause, posi);
		w_clouse->push_back(w);
		if (get_word(where_clause, posi) == "and") {
			and_or = true;
			logic->push_back(1);
		}
		else if (get_word(where_clause, posi) == "or") {
			and_or = true;
			logic->push_back(0);
		}
		else {
			end = true;
		}
	}
	return true;
}

string get_where_op(string where_clouse, int posi) {
  while (where_clouse[posi] == ' ' && posi < where_clouse.length() - 1) {
	posi++;
  }
  if (where_clouse[posi] == '=') {
	return "=";
  }
  else if (where_clouse[posi] == '<') {
	if (where_clouse[posi + 1] == '>') {
	  return "<>";
	}
	return "<";
  }
  else if (where_clouse[posi] == '>') {
	return ">";
  }
  return str_ERROR;
}

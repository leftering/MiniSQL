#pragma once
#include "pch.h"
#include "bplustree.h"
#include "buffer_manager.h"
#include "record_manager.h"
#include "interpreter.h"
#include "define.h"

extern RecordManager record_manager;
extern BufferManager buffer_manager;
extern Interpreter In;

class type_tablelist {
public:
    vector<bptree<int>*> int_treelist;
    vector<bptree<string>*> string_treelist;
    vector<bptree<float>*> float_treelist;
public:
    type_tablelist();
    ~type_tablelist();
    void write_all_tree_to_file();
    bptree<int>* find_int_tree(string filename, string attributename);
    bptree<string>* find_string_tree(string filename, string attributename);
    bptree<float>* find_float_tree(string filename, string attributename);
    bptree<int>* create_tree_int(string indexname, string table_name, string attributename, char type);
    bptree<string>* create_tree_string(string indexname, string table_name, string attributename, char type);
    bptree<float>* create_tree_float(string indexname, string table_name, string attributename, char type);
    int drop_tree_int(string indexname, string table_name, string attribute_name);
    int drop_tree_float(string indexname, string table_name, string attribute_name);
    int drop_tree_string(string indexname, string table_name, string attribute_name);
    void delete_tree_int(string table_name, string attributename);
    void delete_tree_float(string table_name, string attributename);
    void delete_tree_string(string table_name, string attributename);
};
int insert_index_int(string table_name, string attributename, int key, address a);
int insert_index_string(string table_name, string attributename, string key, address a);
int insert_index_float(string table_name, string attributename, float key, address a);
int delete_index_int(string table_name, string attributename, int key);
int delete_index_string(string table_name, string attributename, string key);
int delete_index_float(string table_name, string attributename, float key);
address find_index_int(string table_name, string attributename, int key);
address find_index_string(string table_name, string attributename, string key);
address find_index_float(string table_name, string attributename, float key);
address find_scope_int_low(string table_name, string attributename, int key);
address find_scope_int_up(string table_name, string attributename, int key);
address find_scope_string_low(string table_name, string attributename, string key);
address find_scope_string_up(string table_name, string attributename, string key);
address find_scope_float_low(string table_name, string attributename, float key);
address find_scope_float_up(string table_name, string attributename, float key);
void delete_scope_int(string table_name, string attributename, int lowkey, int upkey);
void delete_scope_string(string table_name, string attributename, string lowkey, string upkey);
void delete_scope_float(string table_name, string attributename, float lowkey, float upkey);

//写文件，读文件，从buffer创建初始索引。
void write_to_file_int(bptree<int>* tree);

bptree<int>* read_from_file_int(string filename);

void write_to_file_float(bptree<float>* tree);

bptree<float>* read_from_file_float(string filename);

void write_to_file_string(bptree<string>* tree);

bptree<string>* read_from_file_string(string filename);

int create_index_from_record(string index_name, string tablename, string attributename);

int drop_index(string index_name);

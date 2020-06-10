#include "record_manager.h"
#include "define.h"
#include "index.h"

bool RecordManager::check(Tuple record, std::vector<Where_clause> wheres, std::vector<int> logic)
{
	bool flag = true;
	int where_num = wheres.size();
	bool* flags = new bool[where_num];
	memset(flags, true, where_num);
	table_info T = In.table;
	for (int i = 0; i < where_num; i++) {
		Where_clause wherei = wheres[i];
		int index = -1;
		for (int j = 0; j < T.col_num; j++) {
			if (T.col[j].col_name == wherei.attr) {
				index = j;
				break;
			}
		}
		if (index == -1) {
			return false;
		}
		if (record.getData()[index].type == -2) {
			if (!cmp(record.getData()[index].datai, std::stoi(wherei.value), wherei.operation)) {
				flags[i] = false;
			}
			
		}
		else if (record.getData()[index].type == -1) {
			if (!cmp(record.getData()[index].dataf, std::stof(wherei.value), wherei.operation)) {
				flags[i] = false;
			}
			
		}
		else {
			if (!cmp(record.getData()[index].datas, wherei.value, wherei.operation)) {
				flags[i] = false;
			}
		}
	}
	flag = flag && flags[0];
	if (logic.size()) {
		for (int i = 0; i < where_num - 1; i++) {
			if (logic[i] == 0) {
				flag = flag || flags[i + 1];
			}
			else {
				flag = flag && flags[i + 1];
			}
		}
	}
	return flag;
}

Tuple RecordManager::read2tuple(BYTE* record, table_info T)
{
	Tuple tuple;
	int offset = T.col_num;
	for (int i = 0; i < T.col_num; i++) {
		int size = record[i];
		Data data;
		// ¶ÁÈ¡Ò»¸öÊôÐÔ
		if (T.col[i].col_type == COL_INT) {
			data.type = -2;
			memcpy(&(data.datai), record + offset, size);
		}
		else if (T.col[i].col_type == COL_FLOAT) {
			data.type = -1;
			memcpy(&(data.dataf), record + offset, size);
		}
		else {
			data.type = size;
			char* r = new char[size];
			memcpy(r, record + offset, size);
			std::string s(r, size);
			data.datas = s;
		}
		offset += size;
		tuple.addData(data);
	}
	return tuple;
}

address RecordManager::find_addr_equal(table_info T, std::string attributename, Where_clause where, int attr_index)
{
	address naddr = NULL;
	if (T.col[attr_index].col_type == COL_INT) {
		int key = std::stoi(where.value);
		naddr = find_index_int(T.table_name, T.col[attr_index].col_name, key);
	}
	else if (T.col[attr_index].col_type == COL_FLOAT) {
		float key = std::stof(where.value);
		naddr = find_index_float(T.table_name, T.col[attr_index].col_name, key);
	}
	else {
		std::string key = where.value;
		naddr = find_index_string(T.table_name, T.col[attr_index].col_name, key);
	}
	return naddr;
}

address RecordManager::find_addr_low(table_info T, std::string attributename, Where_clause where, int attr_index)
{
	address naddr = NULL;
	if (T.col[attr_index].col_type = COL_INT) {
		int low_bound = std::stoi(where.value);
		naddr = find_scope_int_low(T.table_name, T.col[attr_index].col_name, low_bound);
	}
	else if (T.col[attr_index].col_type = COL_FLOAT) {
		float low_bound = std::stof(where.value);
		naddr = find_scope_float_low(T.table_name, T.col[attr_index].col_name, low_bound);
	}
	else {
		std::string low_bound = where.value;
		naddr = find_scope_string_low(T.table_name, T.col[attr_index].col_name, low_bound);
	}
	return naddr;
}

address RecordManager::find_addr_up(table_info T, std::string attributename, Where_clause where, int attr_index)
{
	address naddr;
	if (T.col[attr_index].col_type = COL_INT) {
		int low_bound = std::stoi(where.value);
		naddr = find_scope_int_up(T.table_name, T.col[attr_index].col_name, low_bound);
	}
	else if (T.col[attr_index].col_type = COL_FLOAT) {
		float low_bound = std::stof(where.value);
		naddr = find_scope_float_up(T.table_name, T.col[attr_index].col_name, low_bound);
	}
	else {
		std::string low_bound = where.value;
		naddr = find_scope_string_up(T.table_name, T.col[attr_index].col_name, low_bound);
	}
	return naddr;
}

int RecordManager::select(std::vector<int>col_ids, std::vector<Where_clause>wheres, std::vector<int> logic, std::vector<Tuple>* tuples)
{
	int cnt = 0;
	table_info T = In.table;
	std::string table_name = T.table_name;

	bool flag = false;
	std::vector<int> col_id;
	for (int i = 0; i < T.col_num; i++) {
		if (T.col[i].have_index == true) {
			col_id.push_back(i);
		}
	}

	int where_index, attr_index;
	for (int i = 0; i < col_id.size(); i++) {
		int j;
		for (j = 0; j < wheres.size(); j++) {
			if (wheres[j].attr == T.col[col_id[i]].col_name) {
				where_index = j;
				attr_index = col_id[i];
				flag = true;
				break;
			}
		}
		if (j < wheres.size()) {
			break;
		}
	}

	if (flag && (logic.size() == 0 || logic[0] == 1)) {
		address naddr = NULL;
		int direction = 0;
		if (wheres[where_index].operation == ">" || wheres[where_index].operation == ">=") {
			naddr = find_addr_low(T, T.col[attr_index].col_name, wheres[where_index], attr_index);
		}
		else if (wheres[where_index].operation == "<" || wheres[where_index].operation == "<=") {
			naddr = find_addr_up(T, T.col[attr_index].col_name, wheres[where_index], attr_index);
			direction = 1;
		}
		else if (wheres[where_index].operation == "=") {
			naddr = find_addr_equal(T, T.col[attr_index].col_name, wheres[where_index], attr_index);
			direction = 2;
		}
		wheres.erase(wheres.begin() + where_index);
		if (logic.size() >= 1) {
			logic.pop_back();
		}
		
		if (naddr == NULL) {
			return 0;
		}
		else {
			while (naddr != NULL) {
				Block* blocki = buffer_manager.getPage(table_name, naddr->block_id);
				BYTE* recordi = (*blocki).getRecord(naddr->record_id);
				Tuple tuple = read2tuple(recordi, T);
				if (!check(tuple, wheres, logic)) {
					tuple.setDeleted();
				}
				else {
					int index = col_ids.size() - 1;
					if (index + 1 != T.col_num && index >= 0) {
						for (int k = T.col_num - 1; k >= 0; k--) {
							if (index >= 0 && k == col_ids[index]) {
								index--;
								continue;
							}
							else {
								tuple.eraseData(k);
							}
						}
					}
					cnt++;
				}
				tuples->push_back(tuple);
				if (direction == 1) {
					naddr = naddr->last_addr;
				}
				else if(direction == 0){
					naddr = naddr->next_addr;
				}
				else {
					cout << "index select" << endl;
					naddr = NULL;
				}
			}
		}
		return cnt;
	}

	int block_num = buffer_manager.getBlockNum(table_name);
	for (int i = 0; i < block_num; i++) {
		Block* blocki = buffer_manager.getPage(table_name, i);
		BYTE* blocki_data = (*blocki).getData();
		int record_num = blocki_data[0];
		for (int j = 1; j <= record_num; j++) {
			BYTE* recordj = (*blocki).getRecord(j);
			Tuple tuple = read2tuple(recordj, T);
			if (!check(tuple, wheres, logic)) {
				tuple.setDeleted();
			}
			else{
				int index = col_ids.size() - 1;
				if (index + 1 != T.col_num && index >= 0) {
					for (int k = T.col_num - 1; k >= 0; k--) {
						if (index >= 0 && k == col_ids[index]) {
							index--;
							continue;
						}
						else {
							tuple.eraseData(k);
						}
					}
				}
				cnt++;
			}
			tuples->push_back(tuple);
		}
	}
	return cnt;
}

void RecordManager::insert2block(BYTE* data, std::vector<Data> records, short record_size, short free_space)
{
	short ptr;
	memcpy(&ptr, data + 1, sizeof(short));
	ptr = ptr - (record_size - 1);
	data[0] += 1;
	memcpy(data + 2 * data[0] + 1, &ptr, sizeof(short));
	short free_ptr = ptr - 1;
	memcpy(data + 1, &free_ptr, sizeof(short));
	free_space -= (record_size + 2);
	memcpy(data + BLOCKSIZE - 2, &free_space, sizeof(short));
	int attri_num = records.size();
	short offset = ptr + attri_num;
	for (int i = 0; i < attri_num; i++) {
		if (records[i].type == -2) {
			memset(data + ptr + i, sizeof(int), 1);
			memcpy(data + offset, &records[i].datai, sizeof(int));
			offset += sizeof(int);
		}
		else if (records[i].type == -1) {
			memset(data + ptr + i, sizeof(float), 1);
			memcpy(data + offset, &records[i].dataf, sizeof(float));
			offset += sizeof(float);
		}
		else {
			memset(data + ptr + i, records[i].type, 1);
			memcpy(data + offset, records[i].datas.c_str(), records[i].type);
			offset += records[i].type;
		}
	}
}

bool RecordManager::check_unique(table_info T, Tuple record)
{
	T = In.table;
	int col_num = T.col_num;
	std::vector<Where_clause>wheres;
	std::vector<int>logic;
	std::vector<int> col_ids;
	for (int i = 0; i < col_num; i++) {
		if (T.col[i].primary_key == true || T.col[i].unique == true) {
			Where_clause whr;
			whr.attr = T.col[i].col_name;
			int type = record.getData()[i].type;
			if (type == -2) {
				if (T.col[i].have_index == true && find_index_int(T.table_name, T.col[i].col_name, record.getData()[i].datai) != NULL) {
					return false;
				}
				whr.value = to_string(record.getData()[i].datai);
			}
			else if (type == -1) {
				if (T.col[i].have_index == true && find_index_float(T.table_name, T.col[i].col_name, record.getData()[i].dataf) != NULL) {
					return false;
				}
				whr.value = to_string(record.getData()[i].dataf);
			}
			else {
				if (T.col[i].have_index == true && find_index_string(T.table_name, T.col[i].col_name, record.getData()[i].datas) != NULL) {
					return false;
				}
				whr.value = record.getData()[i].datas;
			}
			if (T.col[i].have_index == false) {
				whr.operation = "=";
				wheres.push_back(whr);
				logic.push_back(0);
			}
		}
	}
	if (logic.size() > 0) {
		logic.pop_back();
	}
	std::vector<Tuple> tuples;
	if (select(col_ids, wheres, logic, &tuples) != 0) {
		return false;
	}
	return true;
}

int RecordManager::insert(Tuple record)
{
	static table_info T = In.table;
	std::string table_name = T.table_name;
	int col_num = T.col_num;
	short record_size = 0;
	for (int i = 0; i < col_num; i++) {
		int type = record.getData()[i].type;
		if (type == -2) {
			record_size += sizeof(int);
		}
		else if (type == -1) {
			record_size += sizeof(float);
		}
		else {
			record_size += type;
		}
	}
	record_size += col_num;
	int block_num = buffer_manager.getBlockNum(table_name);
	for (int i = 0; i <= block_num; i++) {
		Block* blocki = buffer_manager.getPage(table_name, i);
		BYTE* data = (*blocki).getData();
		short free_space;
		memcpy(&free_space, data + BLOCKSIZE - 2, sizeof(short));
		if (free_space >= (record_size + 2)) {
			if (check_unique(T, record)) {
				insert2block(data, record.getData(), record_size, free_space);
				buffer_manager.modifyPage(buffer_manager.getPageId(table_name, i));
				//if there is a index, insert it
				int k;
				address nadd = create_addr();
				nadd->block_id = i;
				nadd->record_id = data[0];
				for(k = 0; k < T.col_num; k ++)
				{
					if(T.col[k].have_index == true)
					{
						if(T.col[k].col_type == 0)
							insert_index_int(T.table_name,T.col[k].col_name,record.getData()[k].datai,nadd);
						else if(T.col[k].col_type == 1)
							insert_index_float(T.table_name,T.col[k].col_name,record.getData()[k].dataf,nadd);
						else if(T.col[k].col_type == 2)
							insert_index_string(T.table_name,T.col[k].col_name,record.getData()[k].datas,nadd);
					}
				}
				//index over
				return 1;
				break;
			}
			else {
				return -2014;	// unique¼üÖØ¸´
			}
		}
		block_num++;
	}
	return 1;
}

void RecordManager::remove4block(BYTE* data, int record_id, int col_num)
{
	short record_size = col_num;
	short ptr;
	memcpy(&ptr, data + 2 * record_id + 1, sizeof(short));
	for (int i = 0; i < col_num; i++) {
		BYTE attri_size = 0;
		memcpy(&attri_size, data + ptr + i, 1);
		record_size += (short)attri_size;
	}
	short free_space;
	memcpy(&free_space, data + BLOCKSIZE - 2, sizeof(short));
	free_space += (record_size + 2);
	memcpy(data + BLOCKSIZE - 2, &free_space, sizeof(short));
	data[0] -= 1;
	short free_ptr, start_ptr = ptr + record_size - 1;
	memcpy(&free_ptr, data + 1, sizeof(short));
	free_ptr += record_size;
	memcpy(data + 1, &free_ptr, sizeof(short));
	for (short i = start_ptr; i > free_ptr; i--) {
		data[i] = data[i - record_size];
	}
	for (short i = 3; i <= 2 * data[0] + 1; i+=2) {
		short sizei;
		memcpy(&sizei, data + i + 2, sizeof(short));
		sizei += record_size;
		memcpy(data + i, &sizei, sizeof(short));
	}
}

std::string get_ith_value(table_info T, BYTE record[], int i)
{
	std::string s;
	int offset = T.col_num;
	for (int j = 0; j < i; j ++) {
		offset += record[j];
	}
	if (T.col[i].col_type == COL_INT) {
		int value;
		memcpy(&value, record + offset, sizeof(int));
		s = to_string(value);
	}
	else if (T.col[i].col_type == COL_FLOAT) {
		float value;
		memcpy(&value, record + offset, sizeof(float));
		s = to_string(value);
	}
	else {
		char* value = NULL;
		memcpy(value, record + offset, record[i]);
		s = value;
	}
	return s;
}

int RecordManager::remove(std::vector<Where_clause>wheres, std::vector<int>logic)
{
	int cnt = 0;
	table_info T = In.table;
	std::string table_name = T.table_name;

	bool flag = false;
	std::vector<int> col_id;
	for (int i = 0; i < T.col_num; i++) {
		if (T.col[i].have_index == true) {
			col_id.push_back(i);
			flag = true;
		}
	}
	if (flag && (logic.size() == 0 || logic[0] == 1)) {
		int where_index, attr_index;
		for (int i = 0; i < col_id.size(); i++) {
			int j;
			for (j = 0; j < wheres.size(); j++) {
				if (wheres[j].attr == T.col[col_id[i]].col_name) {
					where_index = j;
					attr_index = col_id[i];
					break;
				}
			}
			if (j < wheres.size()) {
				break;
			}
		}
		address naddr =	NULL;
		int direction = 0;
		if (wheres[where_index].operation == ">" || wheres[where_index].operation == ">=") {
			naddr = find_addr_low(T, T.col[col_id[attr_index]].col_name, wheres[where_index], attr_index);
		}
		else if (wheres[where_index].operation == "<" || wheres[where_index].operation == "<=") {
			naddr = find_addr_up(T, T.col[col_id[attr_index]].col_name, wheres[where_index], attr_index);
			direction = 1;
		}
		wheres.erase(wheres.begin() + where_index);
		logic.pop_back();
		if (naddr == NULL) {
			return 0;
		}
		else {
			while (naddr != NULL) {
				Block* blocki = buffer_manager.getPage(table_name, naddr->block_id);
				BYTE* blocki_data = (*blocki).getData();
				BYTE* recordi = (*blocki).getRecord(naddr->record_id);
				Tuple tuple = read2tuple(recordi, T);
				if (check(tuple, wheres, logic)) {
					remove4block(blocki_data, naddr->record_id, T.col_num);
					buffer_manager.modifyPage(buffer_manager.getPageId(table_name, naddr->block_id));
					cnt++;
					for (int i = 0; i < T.col_num; i++) {
						if (T.col[i].have_index == true) {
							if (T.col[i].col_type == COL_INT) {
								int value = stoi(get_ith_value(T, recordi, i));
								delete_scope_int(table_name, T.col[i].col_name, value, value);
							}
							else if (T.col[i].col_type == COL_FLOAT) {
								float value = stof(get_ith_value(T, recordi, i));
								delete_scope_float(table_name, T.col[i].col_name, value, value);
							}
							else {
								std::string value = get_ith_value(T, recordi, i);
								delete_scope_string(table_name, T.col[i].col_name, value, value);
							}
						}
					}
				}
				if (direction) {
					naddr = naddr->last_addr;
				}
				else {
					naddr = naddr->next_addr;
				}
			}
		}
		return cnt;
	}

	int block_num = buffer_manager.getBlockNum(table_name);
	for (int i = 0; i < block_num; i++) {
		Block* blocki = buffer_manager.getPage(table_name, i);
		BYTE* blocki_data = (*blocki).getData();
		int record_num = blocki_data[0];
		for (int j = 1; j <= record_num; j++) {
			BYTE* recordj = (*blocki).getRecord(j);
			Tuple tuple = read2tuple(recordj, T);
			if (check(tuple, wheres, logic)) {
				remove4block(blocki_data, j, T.col_num);
				buffer_manager.modifyPage(buffer_manager.getPageId(table_name, i));
				record_num = blocki_data[0];
				cnt++;
				j--;
			}
		}
	}
	return cnt;
	/*
	//the way of checking index is similar to select
	...
	//to delete record, still need to use selete to find addr then use addr to delete.
	
	//these below are used for delete index
	//if scope delete:
	delete_scope_int(table_name,attribute_name,lower_key,upper_key);//delete the one before the upper key
	//but the upper key don't delete
	//if single delete:
	delete_index_int(table_name,attribute_name,key);
	*/
}

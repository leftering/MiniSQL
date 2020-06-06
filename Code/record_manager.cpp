#include "record_manager.h"

bool RecordManager::check(Tuple record, std::string table_name, std::vector<Where_clause> wheres, std::vector<int> logic)
{
	bool flag = true;
	int where_num = wheres.size();
	bool* flags = new bool[where_num];
	memset(flags, true, where_num);
	table_info T;
	T.get_table_info(table_name);
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
		// 读取一个属性
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

int RecordManager::select(std::string table_name, std::vector<int>col_ids, std::vector<Where_clause>wheres, std::vector<int> logic, std::vector<Tuple>* tuples)
{
	int cnt = 0;
	table_info T;
	if (T.get_table_info(table_name) == false) {
		return -2010;		// 表不存在
	}
	int block_num = buffer_manager.getBlockNum(table_name);
	for (int i = 0; i < block_num; i++) {
		Block* blocki = buffer_manager.getPage(table_name, i);
		BYTE* blocki_data = (*blocki).getData();
		int record_num = blocki_data[0];
		for (int j = 1; j <= record_num; j++) {
			BYTE* recordj = (*blocki).getRecord(j);
			Tuple tuple = read2tuple(recordj, T);
			if (!check(tuple, table_name, wheres, logic)) {
				tuple.setDeleted();
			}
			else{
				int index = col_ids.size() - 1;
				if (index >= 0) {
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

bool RecordManager::check_unique(std::string table_name, Tuple record)
{
	table_info T;
	T.get_table_info(table_name);
	int col_num = T.col_num;
	for (int i = 0; i < col_num; i++) {
		if (T.col[i].primary_key == true || T.col[i].unique == true) {
			std::vector<int> col_ids;
			std::vector<Where_clause>wheres;
			Where_clause whr;
			whr.attr = T.col[i].col_name;
			int type = record.getData()[i].type;
			if (type == -2) {
				whr.value = to_string(record.getData()[i].datai);
			}
			else if (type == -1) {
				whr.value = to_string(record.getData()[i].dataf);
			}
			else {
				whr.value = record.getData()[i].datas;
			}
			whr.operation = "=";
			wheres.push_back(whr);
			std::vector<Tuple> tuples;
			std::vector<int>logic;
			if (select(table_name, col_ids, wheres, logic, &tuples) != 0) {
				return false;
			}
		}
	}
	return true;
}

int RecordManager::insert(std::string table_name, Tuple record)
{
	static table_info T;
	if (T.get_table_info(table_name) == false) {
		return -2010;
	}
	int col_num = T.col_num, act_col_num = record.getData().size();
	if (col_num != act_col_num) {
		return -2011;	// 输入属性不够
	}
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
			if (check_unique(table_name, record)) {
				insert2block(data, record.getData(), record_size, free_space);
				buffer_manager.modifyPage(buffer_manager.getPageId(table_name, i));
				return 1;
				break;
			}
			else {
				return -2012;	// unique键重复
			}
		}
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

int RecordManager::remove(std::string table_name, std::vector<Where_clause>wheres, std::vector<int>logic)
{
	int cnt = 0;
	table_info T;
	if (T.get_table_info(table_name) == false) {
		return -2010;
	}
	int block_num = buffer_manager.getBlockNum(table_name);
	for (int i = 0; i < block_num; i++) {
		Block* blocki = buffer_manager.getPage(table_name, i);
		BYTE* blocki_data = (*blocki).getData();
		int record_num = blocki_data[0];
		for (int j = 1; j <= record_num; j++) {
			BYTE* recordj = (*blocki).getRecord(j);
			Tuple tuple = read2tuple(recordj, T);
			if (check(tuple, table_name, wheres, logic)) {
				remove4block(blocki_data, j, T.col_num);
				buffer_manager.modifyPage(buffer_manager.getPageId(table_name, i));
				record_num = blocki_data[0];
				cnt++;
				j--;
			}
		}
	}
	return cnt;
}
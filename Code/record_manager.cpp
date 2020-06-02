#include "record_manager.h"
Block::~Block()
{
	delete[]data;
}
void Block::setTableName(std::string table)
{
	table_name = table;
}
std::string Block::getTableName()
{
	return table_name;
}
void Block::setBlockId(int _block_id)
{
	block_id = _block_id;
}
int Block::getBlockId()
{
	return block_id;
}
void Block::setDirty(bool _dirty)
{
	dirty_bit = _dirty;
}
bool Block::isDirty()
{
	return dirty_bit;
}
void Block::setUsed(bool _use)
{
	use_bit = _use;
}
bool Block::isUsed()
{
	return use_bit;
}
void Block::setPinned(bool _pinned)
{
	is_pinned = _pinned;
}
bool Block::isPinned()
{
	return is_pinned;
}
char* Block::getData()
{
	return data;
}
char* Block::getNextRecord()
{
	if (ptr == 0 || ptr > data[0]) {
		return NULL;
	}
	int start = data[ptr * 2 + 1], end = data[ptr * 2 + 3] - 1;
	char* record = new char[end - start + 1];;
	for (int i = start; i <= end; i++) {
		record[i - start] = data[i];
	}
	ptr = ptr + 1;
	return record;
}

int RecordManager::getBlockNum(std::string table_name) {
	char* p;
	int block_num = -1;
	do {
		p = buffer_manager.getPage(table_name, ++block_num);
	} while (p != NULL);
	return block_num;
}

bool RecordManager::checkRecord(const char* record, std::string table_name, const int colId, const int op, const char* opValue)
{
	std::string record_value;
	table_info TABLE;
	TABLE.get_table_info(table_name);
	std::string attriName = TABLE.col[colId].col_name;
	if (attriName == "int") {
		record_value = ;
	}

	return true;
}

int RecordManager::select(std::string table_name, std::string colName, const int op, const char* opValue, vector<char*>* records)
{
	int cnt = 0;
	table_info TABLE;
	if (TABLE.get_table_info(table_name) == false) {
		return -1;
	}
	int block_num = getBlockNum(table_name);
	if (block_num == 0) {
		return -1;
	}
	for (int i = 0; i < block_num; i++) {
		char* page = buffer_manager.getPage(table_name, i);
		int record_num = page[0];
		for (int i = 1; i <= record_num; i++) {
			int start = page[2 * i + 1], end = page[2 * i + 3] - 1;
			char* record = new char[end - start + 1];
			memcpy(record, page + start, sizeof(record));
			cnt++;
			records->push_back(record);
		}
	}
	return cnt;
}

int RecordManager::insert(const char* table_name, const int* type, const char* data)
{

}
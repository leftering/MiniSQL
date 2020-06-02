#include "record_manager.h"

int RecordManager::getBlockNum(std::string table_name) {
	char* p;
	int block_num = -1;
	do {
		p = buffer_manager.getPage(table_name, ++block_num);
	} while (p != NULL);
	return block_num;
}

bool RecordManager::checkRecord(const char* record, std::string table_name, const int* type, const int colId, const int op, const char* opValue)
{
	int offset = 0;
	std::string record_value;
	for (int i = 0; i < colId; i++) {
		if (type[i] == -2) {
			offset += sizeof(int) / sizeof(char);
		}
		else if (type[i] == -1) {
			offset += sizeof(float) / sizeof(char);
		}
		else {
			offset += type[i];
		}
	}
	if (type[colId] == -2) {
		int rv;
		memcpy(&rv, record + offset, sizeof(rv));
		record_value = to_string(rv);
	}
	else if (type[colId] == -1) {
		float rv;
		memcpy(&rv, record + offset, sizeof(float));
		record_value = to_string(rv);
	}
	else {
		for (int i = 0; i < type[colId]; i++) {
			record_value = record_value + record[offset + i];
		}
	}
	// <, <=, <>, =, >=, >
	switch (op)
	{
	case 0:if (strcmp(record_value.c_str(), opValue) < 0) return true; break;
	case 1:if (strcmp(record_value.c_str(), opValue) <= 0) return true; break;
	case 2:if (strcmp(record_value.c_str(), opValue) != 0) return true; break;
	case 3:if (strcmp(record_value.c_str(), opValue) == 0) return true; break;
	case 4:if (strcmp(record_value.c_str(), opValue) >= 0) return true; break;
	case 5:if (strcmp(record_value.c_str(), opValue) > 0) return true; break;
	default:
		break;
	}
	return false;
}

int RecordManager::select(std::string table_name, const int* type, const int colId, const int op, const char* opValue, vector<char*>* records)
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
			if (checkRecord(record, table_name, type, colId, op, opValue)) {
				cnt++;
				records->push_back(record);
			}
		}
	}
	return cnt;
}
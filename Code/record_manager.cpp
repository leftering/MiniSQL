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

int RecordManager::insert(const char* table_name, const int* type, const char* data)
{
    std::string filename = RECORDPATH + table_name + ".data";
    table_info TABLE;
    //检测表是否存在
    if (TABLE.get_table_info(table_name) == false) {
        return -1;
    }
    Attribute attr = catalog_manager.getAttribute(tmp_name);
    std::vector<Data> v = tuple.getData();
    //检测插入的元组的各个属性是否合法
    for (int i = 0; i < v.size(); i++) {
        if (v[i].type != attr.type[i])
            throw tuple_type_conflict();
    }
    Table table = selectRecord(tmp_name);
    std::vector<Tuple>& tuples = table.getTuple();
    //检测是否存在主键冲突
    if (attr.primary_key >= 0) {
        if (isConflict(tuples, v, attr.primary_key) == true)
            throw primary_key_conflict();
    }
    //检测是否存在unqiue冲突
    for (int i = 0; i < attr.num; i++) {
        if (attr.unique[i] == true) {
            if (isConflict(tuples, v, i) == true)
                throw unique_conflict();
        }
    }

    //异常检测完成

    //获取表所占的块的数量
    // int block_num = getFileSize(table_name) / PAGESIZE;
    // 改为
    int block_num = getBlockNum(table_name);
    //处理表文件大小为0的特殊情况
    if (block_num <= 0)
        block_num = 1;
    //获取表的最后一块的句柄
    char* p = buffer_manager.getPage(table_name, block_num - 1);
    int i;
    //寻找第一个空位
    for (i = 0; p[i] != '\0' && i < PAGESIZE; i++);
    int j;
    int len = 0;
    //计算插入的tuple的长度
    for (j = 0; j < v.size(); j++) {
        Data d = v[j];
        switch (d.type) {
        case -1: {
            int t = getDataLength(d.datai);
            len += t;
        }; break;
        case 0: {
            float t = getDataLength(d.dataf);
            len += t;
        }; break;
        default: {
            len += d.datas.length();
        };
        }
    }
    len += v.size() + 7;
    int block_offset;//最终记录所插入的块的编号
    //如果剩余的空间足够插入该tuple
    if (PAGESIZE - i >= len) {
        block_offset = block_num - 1;
        //插入该元组
        insertRecord1(p, i, len, v);
        //写回表文件
        int page_id = buffer_manager.getPageId(table_name, block_num - 1);
        // buffer_manager.flushPage(page_id , table_name , block_num - 1);
        // 改为
        buffer_manager.modifyPage(page_id);
    }
    //如果剩余的空间不够
    else {
        block_offset = block_num;
        //新增一个块
        char* p = buffer_manager.getPage(table_name, block_num);
        //在新增的块中插入该元组
        insertRecord1(p, 0, len, v);
        //写回表文件
        int page_id = buffer_manager.getPageId(table_name, block_num);
        // buffer_manager.flushPage(page_id , table_name , block_num);
        // 改为
        buffer_manager.modifyPage(page_id);
    }

    //更新索引
    IndexManager index_manager(tmp_name);
    for (int i = 0; i < attr.num; i++) {
        if (attr.has_index[i] == true) {
            std::string attr_name = attr.name[i];
            std::string file_path = "INDEX_FILE_" + attr_name + "_" + tmp_name;
            std::vector<Data> d = tuple.getData();
            index_manager.insertIndex(file_path, d[i], block_offset);
        }
    }
}
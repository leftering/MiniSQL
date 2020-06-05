#include "buffer_manager.h"
#include "pch.h"

Block::~Block()
{
    
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
BYTE* Block::getData()
{
    return data;
}
BYTE* Block::getRecord(int ith)
{
    if (ith > data[0]) {
        return NULL;
    }
    int start = data[ith * 2 + 1], end = data[ith * 2 + 3] - 1;
    BYTE* record = new BYTE[end - start + 1];
    memcpy(record, data + start, end - start + 1);
    return record;
}

void BufferManager::initialize()
{
    Pages = new Block[page_num];
}

BufferManager::~BufferManager()
{
	for (int i = 0; i < page_num; i ++) {
        if (Pages[i].isPinned() == false) {
            flushPage(i);
        }
	}
	delete[]Pages;
}
void BufferManager::flushPage(int page_id)
{
	Block page = Pages[page_id];
	std::string filename = RECORDPATH + page.getTableName() + ".data";
	int block_id = page.getBlockId();
    FILE* f = fopen(filename.c_str(), "r+");

    if (f == NULL)
        return;

    fseek(f, BLOCKSIZE * block_id, SEEK_SET);
    BYTE* data = page.getData();
    fwrite(data, BLOCKSIZE, 1, f);

    fclose(f);
    return;
}
Block* BufferManager::getPage(std::string table_name, int block_id)
{
    for (int i = 0; i < page_num; i++) {
        if (Pages[i].getBlockId() == block_id && table_name == Pages[i].getTableName()) {
            Pages[i].setUsed(true);
            return Pages+ i;
        }
    }
    int empty = getEmptyPageId();
    if (block_id < getBlockNum(table_name) && loadDiskBlock(empty, table_name, block_id)) {
        return Pages + empty;
    }
    Block EMPTY(table_name, block_id, false, true, false);
    Pages[empty] = EMPTY;
    return Pages + empty;
}
bool BufferManager::loadDiskBlock(int page_id, std::string table_name, int block_id) {
    Pages[page_id] = Block(table_name, block_id, false, true, false);
    std::string filename = RECORDPATH + table_name + ".data";
    FILE* f = fopen(filename.c_str(), "r");
    if (f == NULL)
        return false;

    fseek(f, BLOCKSIZE * block_id, SEEK_SET);
    BYTE* data = Pages[page_id].getData();
    if (fread(data, BLOCKSIZE, 1, f) == 0) {
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}
void BufferManager::modifyPage(int page_id)
{
    Pages[page_id].setDirty(true);
}
void BufferManager::pinPage(int page_id)
{
    Pages[page_id].setPinned(true);
}
void BufferManager::unpinPage(int page_id)
{
    Pages[page_id].setPinned(false);
}
int BufferManager::getEmptyPageId()
{
    int empty = current_position;
    if (Pages[current_position].isUsed() == false && Pages[current_position].isDirty() == false) {
        current_position = (current_position + 1) % page_num;
        return empty;
    }
    else {
        int current = current_position;
        while (1) {
            for (int i = 0; i < page_num; i++) {
                if (Pages[current].isUsed() == false && Pages[current].isDirty() == true) {
                    current_position = current + 1;
                    return current;
                }
                Pages[current].setUsed(false);
                current = (current + 1) % page_num;
            }
        }
    }
}
int BufferManager::getBlockNum(std::string table_name)
{
    std::string filepath = RECORDPATH + table_name + ".data";
    FILE* fp = fopen(filepath.c_str(), "r+");
    fseek(fp, 0, SEEK_END);
    int num;
    num = ftell(fp) / BLOCKSIZE;
    return num;
}

int BufferManager::getPageId(std::string table_name, int block_id) {
    for (int i = 0; i < page_num; i++) {
        std::string tmp_table_name = Pages[i].getTableName();
        int tmp_block_id = Pages[i].getBlockId();
        if (tmp_table_name == table_name && tmp_block_id == block_id)
            return i;
    }
    return -1;
}

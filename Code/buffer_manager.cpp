#include "buffer_manager.h"
#include "pch.h"

BufferManager::~BufferManager()
{
	for (int i = 0; i < page_num; i ++) {
		flushPage(i);
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
    char* data = page.getData();
    fwrite(data, BLOCKSIZE, 1, f);

    fclose(f);
    return;
}
char* BufferManager::getPage(std::string table_name, int block_id)
{
    for (int i = 0; i < page_num; i++) {
        if (Pages[i].getBlockId() == block_id && table_name == Pages[i].getTableName()) {
            Pages[i].setUsed(true);
            return Pages[i].getData();
        }
    }
    int empty = getEmptyPageId();
    if (loadDiskBlock(empty, table_name, block_id)) {
        return Pages[empty].getData();
    }
    return NULL;
}
bool BufferManager::loadDiskBlock(int page_id, std::string table_name, int block_id) {
    Pages[page_id] = Block(table_name, block_id, false, true, false);
    std::string filename = RECORDPATH + table_name + ".data";
    FILE* f = fopen(filename.c_str(), "r");
    if (f == NULL)
        return false;

    fseek(f, BLOCKSIZE * block_id, SEEK_SET);
    char* data = Pages[page_id].getData();
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

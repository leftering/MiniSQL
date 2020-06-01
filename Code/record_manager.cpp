#include "record_manager.h"
Block::~Block()
{
	delete[]data;
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
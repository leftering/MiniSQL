#include "bplustree.h"
address create_addr()
{
	address x;
	x = new struct addr;
	x->block_id = -1;
	x->record_id = -1;
	x->last_addr = NULL;
	x->next_addr = NULL;
	return x;
}
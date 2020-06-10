#pragma once
#include "pch.h"
#define rootstate 2
#define interstate 1
#define leafstate 0
#define nodecapacity 25//how many keys in a node of B+tree 
//同一个地址必须只能建立一次索引
//data in block is in char, so the value is char 

address create_addr();

//leaf : key, addr(page), parents, sibling
//inter: key, parents, children, number of children
template<class K>
class indexnode {
public:
	int NodeState;	/* Internal/Leaf/Root */
	indexnode* sibling;
	indexnode* parent;
	vector<K> key;
	vector<address> page;//pointer
	vector<indexnode<K>*> children;//pointer
	int node_number;
	indexnode();
	~indexnode();
};

template <class K>
indexnode<K>::indexnode()
{
	key = vector<K>(0);
	page = vector<address>(0);
	children = vector<indexnode<K>*>(0);
	this->NodeState = interstate;
	this->sibling = NULL;
	this->parent = NULL;
	this->node_number = 0;
}
template <class K>
indexnode<K>::~indexnode()
{
	sibling = NULL;
	parent = NULL;
	int i;
	while (page.size() > 0)page.pop_back();
	while (key.size() > 0)key.pop_back();
	for (i = 0; i < this->children.size(); i++)
	{
		if (children[i] != NULL)this->children[i]->~indexnode<K>();
	}

}


template <class K>
class bptree
{
public:
	char data_type;//i means int , s means string, f means float,
	int nodenumber;//只是在输出到文件的时候，需要使用这个来标号
	string indexname;
	string index_filename;
	string index_attributename;
	indexnode<K>* rootnode;
	indexnode<K>* aim;
	map <address, K> ak;//建立从address到K的映射，可以利用address来找到key
	map <address, int>indexed;//用来标记这个地址是否建立了索引，建立了就不能再建。
	int aim_id;//aim 和aim_id可以用于所有find功能的函数，这些函数使用之后，可以通过aim和aim_id来定位当前找到的address
public:
	bptree(string indexname, string index_filename, string index_attributename, char datatype);
	~bptree();
	void disposetree(void);
	//void updateblock(indexnode<K>* unode);//?
	void insertindex(K key, address a);
	void deleteindex(K k);
	void deletes(indexnode<K>* index, int num);
	void deletescope(K lowk, K upk);
	void merge(indexnode<K>* temp);
	//void lockindex(indexnode<K> node);//?
	int check_nodestate(indexnode <K>* cnode);
	address find_index_of_key(K key);
	address lowerbound_of_key(K key);
	address upperbound_of_key(K key);
	void split(indexnode<K>* temp);//node 's split
	void refresh();
	indexnode<K>* findsmallest(indexnode<K>* temp);
};
template <class K>
int bptree<K>::check_nodestate(indexnode<K>* cnode)
{
	if (cnode->NodeState == rootstate) return 2;
	else if (cnode->NodeState == interstate) return 1;
	else if (cnode->NodeState == leafstate) return 0;
	else return -1;
}
template <class K>
bptree<K>::bptree(string indexname, string index_filename, string index_attributename, char datatype)
{
	this->indexname = indexname;
	this->index_filename = index_filename;
	ofstream out((this->indexname + ".txt").c_str());
	out << index_filename << endl;
	out << index_attributename << endl;
	out.close();
	this->index_attributename = index_attributename;
	this->data_type = datatype;
	indexnode<K>* x;
	x = new(indexnode<K>);
	this->rootnode = x;
	this->rootnode->NodeState = leafstate;
	this->nodenumber = 1;
	aim_id = -1;
	aim = NULL;
}

template <class K>
bptree<K>::~bptree()
{
	aim = NULL;
	ak.erase(ak.begin(), ak.end());
	indexed.erase(indexed.begin(), indexed.end());
	rootnode->~indexnode<K>();
}

template <class K>
address bptree<K>::find_index_of_key(K k)
{
	indexnode<K>* temp;
	temp = this->rootnode;
	int i = 0;
	while (temp->NodeState != leafstate)
	{
		for (i = 0; i < temp->key.size(); i++)
		{
			if (temp->key[i] > k)
			{
				temp = temp->children[i];
				break;
			}
			else if (temp->key[i] == k)
			{
				temp = temp->children[i + 1];
				break;
			}
			else if (temp->key[temp->key.size() - 1] <= k)
			{
				temp = temp->children[temp->key.size()];
				break;
			}
		}
	}
	for (i = 0; i < temp->key.size(); i++)
	{
		if (temp->key[i] == k)
		{
			aim_id = i;
			aim = temp;
			return temp->page[i];
		}
	}
	aim_id = -1;
	aim = NULL;
	return NULL;
}

template <class K>
address bptree<K>::lowerbound_of_key(K k)
{
	// cout<<"lowerbound_of_key"<<endl;
	indexnode<K>* temp;
	temp = this->rootnode;
	int i = 0;
	while (temp->NodeState != leafstate)
	{
		for (i = 0; i < temp->key.size(); i++)
		{
			if (temp->key[i] > k)
			{
				temp = temp->children[i];
				break;
			}
			else if (temp->key[i] == k)
			{
				temp = temp->children[i + 1];
				break;
			}
			else if (temp->key[temp->key.size() - 1] <= k)
			{
				temp = temp->children[temp->key.size()];
				break;
			}
		}
	}
	for (i = 0; i < temp->key.size(); i++)
	{
		if (temp->key[i] >= k)
		{
			aim_id = i;
			aim = temp;
			return temp->page[i];
		}
		else if (temp->key[temp->key.size() - 1] < k)
		{
			aim = temp->sibling;
			aim_id = 0;
			return temp->sibling->page[0];//修改过，
		}
	}
	aim_id = -1;
	aim = NULL;
	return NULL;//如果是NULL，说明就是这个函数里面没有比它大的了。
}

template <class K>
address bptree<K>::upperbound_of_key(K k)
{
	// cout<<"upperbound_of_key"<<endl;
	indexnode<K>* temp;
	temp = this->rootnode;
	int i = 0;
	while (temp->NodeState != leafstate)
	{
		for (i = 0; i < temp->key.size(); i++)
		{
			if (temp->key[i] > k)
			{
				temp = temp->children[i];
				break;
			}
			else if (temp->key[i] == k)
			{
				temp = temp->children[i + 1];
				break;
			}
			else if (temp->key[temp->key.size() - 1] <= k)
			{
				temp = temp->children[temp->key.size()];
				break;
			}
		}
	}
	for (i = 0; i < temp->key.size(); i++)
	{
		if (temp->key[i] > k)
		{
			aim = temp;
			aim_id = i;
			return temp->page[i];
		}
		else if (temp->key[temp->key.size() - 1] <= k)
		{
			aim = temp->sibling;
			aim_id = 0;
			return temp->sibling->page[0];//前闭后开
		}
	}
	aim_id = -1;
	aim = NULL;
	return NULL;
}

template <class K>
indexnode<K>* bptree<K>::findsmallest(indexnode<K>* temp)
{
	indexnode<K>* m = temp;
	while (m->NodeState != leafstate && m->children[0] != NULL)
	{
		m = m->children[0];
	}
	return m;
}

template <class K>
void bptree<K>::refresh()
{
	int i, j;
	indexnode<K>* temp = this->rootnode;
	indexnode<K>* orig;
	indexnode<K>* curr;
	orig = findsmallest(temp);
	curr = orig;
	// cout << "refresh Begin" << endl;
	while (orig != NULL)
	{
		while (curr->NodeState != rootstate && curr->parent != NULL)
		{
			// cout<<"begin for"<<endl;
			for (i = 0; i < curr->parent->children.size(); i++)
			{
				temp = curr->parent->children[i];
				// cout<<"for"<<endl;
				if (temp == curr)
				{
					// cout<<"hit"<<endl<<temp->key[0]<<endl;
					// cout<<curr->key[0]<<endl;
					// cout<<orig->key[0]<<endl;
					// cout<<"now is the no."<<i<<endl;
					break;
				}

			}
			if (i == 0)
			{
				curr = curr->parent;
				continue;
			}
			else if (i != 0)
			{

				// for(j = 0;j<curr->parent->key.size();j++){cout<<curr->parent->key[j]<<" ";}
				// cout<<endl;
				// cout<<orig->key[0]<<endl;
				// cout<<"give number"<<endl;
				curr->parent->key[i - 1] = orig->key[0];
				break;
			}
		}
		orig = orig->sibling;
		curr = orig;
		// if(orig != NULL)cout<<"new orig : "<<orig->key[0]<<endl;;
	}
	// cout<<"in the refresh : "<<rootnode->key[0]<<endl;
	// cout<<"refresh over"<<endl;
	// cout << "refresh END@@@" << endl;
}

template <class K>
void bptree<K>::insertindex(K k, address a)
{
	if (indexed[a] == 1)return;
	indexed[a] = 1;
	if (ak[a] != k)ak[a] = k;
	indexnode<K>* temp;
	temp = (this->rootnode);
	// cout<<"root state:"<<this->rootnode->NodeState<<endl;
	int i = 0;
	while (temp->NodeState != leafstate)
	{
		for (i = 0; i < temp->key.size(); i++)
		{
			if (temp->key[i] > k)
			{
				temp = (temp->children[i]);
				break;
			}
			else if (temp->key[i] == k)
			{
				temp = temp->children[i + 1];
				break;
			}
			else if (temp->key[temp->key.size() - 1] < k)
			{
				temp = temp->children[temp->key.size()];
				break;
			}
		}
	}
	for (i = 0; i < temp->key.size(); i++)
	{
		if (temp->key[i] == k)
		{
			a->last_addr = temp->page[i]->last_addr;
			a->next_addr = temp->page[i];
			a->last_addr->next_addr = a;
			temp->page[i]->last_addr = a;
			temp->page[i] = a;
			break;
		}
		else if (temp->key[i] > k)//insert
		{
			// cout<<"ori: > k"<<endl;
			int j;
			temp->page.push_back(a);
			for (j = temp->page.size() - 1; j > i; j--)
			{
				temp->page[j] = temp->page[j - 1];
			}
			temp->page[i] = a;
			temp->key.push_back(k);
			for (j = temp->key.size() - 1; j > i; j--)
			{
				temp->key[j] = temp->key[j - 1];
			}
			temp->key[i] = k;
			temp->page[i]->last_addr = temp->page[i + 1]->last_addr;
			temp->page[i]->next_addr = temp->page[i + 1];
			temp->page[i + 1]->last_addr = temp->page[i];
			if (temp->page[i]->last_addr != NULL)temp->page[i]->last_addr->next_addr = temp->page[i];
			if (temp->key.size() > nodecapacity - 1)//split;
			{
				this->split(temp);
			}
			break;
		}
		else if (temp->key[temp->key.size() - 1] < k)
		{
			// cout<<"ori all < k"<<endl;
			int j;
			i = temp->key.size();
			temp->page.push_back(a);
			temp->key.push_back(k);
			address tempaddr = temp->page[i - 1];
			while (tempaddr->next_addr != NULL && ak[tempaddr->next_addr] == ak[tempaddr])tempaddr = tempaddr->next_addr;
			temp->page[i]->last_addr = tempaddr;//no multiple
			temp->page[i]->next_addr = tempaddr->next_addr;

			if (temp->page[i]->last_addr != NULL)temp->page[i]->last_addr->next_addr = temp->page[i];
			if (temp->page[i]->next_addr != NULL)temp->page[i]->next_addr->last_addr = temp->page[i];
			if (temp->key.size() > nodecapacity - 1)//split;
			{
				this->split(temp);
			}
			break;
		}
	}
	if (temp->key.size() == 0)
	{
		temp->key.push_back(k);
		temp->page.push_back(a);
	}
	// cout<<"insert over"<<endl;

	refresh();
}

template <class K>
void bptree<K>::split(indexnode<K>* temp)
{
	int i, j;
	this->nodenumber++;
	int breakpoint;
	int sizechange;
	// static indexnode<K> newnodes[1000];
	int node_num = 0;
	indexnode<K>* newnode; //may be you need static
	newnode = new (indexnode<K>);
	// node_num++;
	if (temp->NodeState == leafstate && temp->parent != NULL)//leaf not root
	{
		// cout<<"leaf not root"<<endl;
		breakpoint = temp->key.size() / 2;
		sizechange = temp->key.size();
		// cout<<breakpoint<<"$$"<<sizechange<<endl;
		for (i = breakpoint; i < sizechange; i++)
		{
			newnode->key.push_back(temp->key[i]);
			newnode->page.push_back(temp->page[i]);
			newnode->key[i - breakpoint] = temp->key[i];
			newnode->page[i - breakpoint] = temp->page[i];

		}
		// cout<<"not stop"<<endl;

		for (i = breakpoint; i < sizechange; i++)
		{
			temp->key.pop_back();
			temp->page.pop_back();
		}
		// cout<<"not stop"<<endl;
		newnode->NodeState = leafstate;
		newnode->parent = temp->parent;
		if (newnode != temp->sibling&&temp->sibling != temp)
		{
			newnode->sibling = temp->sibling;
			temp->sibling = newnode;
		}
		else {
			newnode->sibling = NULL;
			temp->sibling = newnode;
		}
		newnode->parent->children.push_back(newnode);
		newnode->parent->key.push_back(newnode->key[0]);
		// cout<<"not stop"<<endl;
		for (j = newnode->parent->children.size() - 1; j >= 0; j--)
		{
			if (newnode->parent->children[j - 1] != temp)
			{
				newnode->parent->children[j] = newnode->parent->children[j - 1];
				newnode->parent->key[j - 1] = newnode->parent->key[j - 2];
			}
			else {
				newnode->parent->children[j] = newnode;
				newnode->parent->key[j - 1] = newnode->key[0];
				break;
			}
		}
	}
	else if (temp->NodeState == leafstate && temp->parent == NULL)//leaf and root
	{
		this->nodenumber++;
		//  cout<<"leaf and root"<<endl;
		breakpoint = temp->key.size() / 2;
		sizechange = temp->key.size();
		// cout<<breakpoint<<"$$"<<sizechange<<endl;
		for (i = breakpoint; i < sizechange; i++)
		{
			newnode->key.push_back(temp->key[i]);
			newnode->page.push_back(temp->page[i]);
			newnode->key[i - breakpoint] = temp->key[i];
			newnode->page[i - breakpoint] = temp->page[i];
		}

		for (i = breakpoint; i < sizechange; i++)
		{
			temp->key.pop_back();
			temp->page.pop_back();
		}
		// static indexnode<K> newroot;
		// indexnode<K>* root = &newroot;
		indexnode<K>* root;
		root = new(indexnode<K>);
		newnode->NodeState = leafstate;
		temp->NodeState = leafstate;
		root->NodeState = rootstate;
		temp->parent = root;
		newnode->parent = root;
		if (newnode != temp->sibling&&temp->sibling != temp)
		{
			newnode->sibling = temp->sibling;
			temp->sibling = newnode;
		}
		else {
			newnode->sibling = NULL;
			temp->sibling = newnode;
		}
		root->children.push_back(temp);
		root->children.push_back(newnode);
		root->key.push_back(newnode->key[0]);
		root->children[0] = temp;
		root->children[1] = newnode;
		root->key[0] = newnode->key[0];
		this->rootnode = root;
	}
	else if (temp->NodeState == rootstate)//rootnode
	{
		this->nodenumber++;
		// cout<<"root"<<endl;
		breakpoint = (temp->key.size() + 1) / 2;
		sizechange = temp->key.size();
		// cout<<breakpoint<<"$$"<<sizechange<<endl;
		for (i = breakpoint; i < sizechange + 1; i++)
		{
			newnode->children.push_back(temp->children[i]);
			newnode->children[i - breakpoint] = temp->children[i];

			temp->children[i]->parent = newnode;
			if (i != sizechange)
			{
				newnode->key.push_back(temp->key[i]);
				newnode->key[i - breakpoint] = temp->key[i];
			}
		}
		for (i = breakpoint; i < sizechange + 1; i++)
		{
			temp->key.pop_back();
			temp->children.pop_back();
		}
		newnode->NodeState = interstate;
		// static indexnode<K> newroot;
		indexnode<K>* root;
		root = new(indexnode<K>);
		root->NodeState = rootstate;
		temp->NodeState = interstate;
		temp->parent = root;
		newnode->parent = root;
		if (newnode != temp->sibling&&temp->sibling != temp)
		{
			newnode->sibling = temp->sibling;
			temp->sibling = newnode;
		}
		else {
			newnode->sibling = NULL;
			temp->sibling = newnode;
		}
		root->children.push_back(temp);
		root->children.push_back(newnode);
		root->key.push_back(newnode->key[0]);
		root->children[0] = temp;
		root->children[1] = newnode;
		root->key[0] = newnode->key[0];
		this->rootnode = root;
		// cout<<"in the split : " <<rootnode->key[0]<<endl;
	}
	else if (temp->NodeState == interstate)//中间结点
	{
		// cout<<"interstate"<<endl;
		breakpoint = (temp->key.size() + 1) / 2;
		sizechange = temp->key.size();
		// cout<<breakpoint<<"$$"<<sizechange<<endl;
		for (i = breakpoint; i < sizechange + 1; i++)
		{
			newnode->children.push_back(temp->children[i]);
			if (i != sizechange)
			{
				newnode->key.push_back(temp->key[i]);
				newnode->key[i - breakpoint] = temp->key[i];
			}
			newnode->children[i - breakpoint] = temp->children[i];
			temp->children[i]->parent = newnode;
		}
		for (i = breakpoint; i < sizechange + 1; i++)
		{
			temp->key.pop_back();
			temp->children.pop_back();
		}
		newnode->NodeState = interstate;
		newnode->parent = temp->parent;
		if (newnode != temp->sibling&&temp->sibling != temp)
		{
			newnode->sibling = temp->sibling;
			temp->sibling = newnode;
		}
		else {
			newnode->sibling = NULL;
			temp->sibling = newnode;
		}
		newnode->parent->children.push_back(newnode);
		newnode->parent->key.push_back(newnode->key[0]);
		for (j = newnode->parent->children.size() - 1; j >= 0; j--)
		{
			if (newnode->parent->children[j - 1] != temp)
			{
				newnode->parent->children[j] = newnode->parent->children[j - 1];
				newnode->parent->key[j - 1] = newnode->parent->key[j - 2];
			}
			else {
				newnode->parent->children[j] = newnode;
				newnode->parent->key[j - 1] = newnode->key[0];
				break;
			}
		}//对于父节点来说是一样的，都是在目标结点后面加一个新的结点。
	}
	if (temp->parent != NULL && temp->parent->key.size() > nodecapacity - 1 && node_num < 100)//split;迭代
	{
		node_num++;
		this->split(temp->parent);
	}
}

template <class K>
void bptree<K>::deleteindex(K k)
{
	indexnode<K>* temp;
	temp = (this->rootnode);
	int i = 0;
	while (temp->NodeState != leafstate)
	{
		for (i = 0; i < temp->key.size(); i++)
		{
			if (temp->key[i] > k)
			{
				temp = (temp->children[i]);
				break;
			}
			else if (temp->key[i] == k)
			{
				temp = temp->children[i + 1];
				break;
			}
			else if (temp->key[temp->key.size() - 1] < k)
			{
				temp = temp->children[temp->key.size()];
				break;
			}
		}
	}

	for (i = 0; i < temp->key.size(); i++)
	{
		if (temp->key[i] == k)//hit
		{
			// cout<<"ready to deletes"<<endl;
			// cout<<temp->page[i]->block_id<<endl;
			deletes(temp, i);
			break;
		}
		else if (temp->key[i] > k)
		{
			break;
		}
		else if (temp->key[temp->key.size() - 1] < k)
		{
			break;
		}

	}
}

template <class K>
void bptree<K>::deletescope(K lowk, K upk)
{
	address templow;
	address tempup;
	address curraddr;
	K smallestkey, largestkey;
	K currkey;
	templow = lowerbound_of_key(lowk);
	tempup = upperbound_of_key(upk);
	if (templow == NULL)return;//下界为空就说明没有符合条件的记录需要删除。
	smallestkey = ak[templow];
	largestkey = ak[tempup];
	currkey = smallestkey;
	curraddr = templow;
	while (currkey != largestkey)
	{
		while (ak[curraddr] == ak[curraddr->next_addr])
		{
			curraddr = curraddr->next_addr;
		}
		curraddr = curraddr->next_addr;
		deleteindex(currkey);
		currkey = ak[curraddr];
	}
	while (ak[curraddr] == ak[curraddr->next_addr])
	{
		curraddr = curraddr->next_addr;
	}
	curraddr = curraddr->next_addr;
	deleteindex(currkey);
	currkey = ak[curraddr];
}

template <class K>
void bptree<K>::deletes(indexnode<K>* index, int num)
{
	address erase_aim = index->page[num];//这里尽可能的支持key下面的多值，从而删除多个 属性值都是key的地址
	address lastaddr = erase_aim->last_addr;
	address next_eraseaim = erase_aim->next_addr;
	while (ak[next_eraseaim] == ak[erase_aim] && next_eraseaim != NULL)
	{
		indexed[erase_aim] = 0;//将它变掉。
		indexed.erase(erase_aim);
		ak.erase(erase_aim);
		erase_aim = next_eraseaim;
		next_eraseaim = erase_aim->next_addr;
	}
	indexed[erase_aim] = 0;//将它变掉。
	indexed.erase(erase_aim);
	ak.erase(erase_aim);
	address nextaddr = next_eraseaim;
	if (lastaddr != NULL)lastaddr->next_addr = nextaddr;
	if (nextaddr != NULL)nextaddr->last_addr = lastaddr;
	int i;
	for (i = num + 1; i < index->key.size(); i++)
	{
		index->key[i - 1] = index->key[i];
		index->page[i - 1] = index->page[i];
	}
	index->key.pop_back();
	index->page.pop_back();

	if (index->key.size() < nodecapacity / 2 && index->NodeState != rootstate && index->parent != NULL)
	{
		this->merge(index);//优先和自己的父节点的孩子合并。
	}
}

template <class K>
void bptree<K>::merge(indexnode<K>* temp)//merge完了可能还要再split
{
	int i, j, child_num, key_num;
	indexnode<K>* mergenode = NULL;
	indexnode<K>* secnode = NULL;
	this->nodenumber--;
	if (temp->NodeState == rootstate)
	{
		this->rootnode = temp->children[0];
		this->nodenumber--;
		this->rootnode->parent == NULL;
		if (this->rootnode->NodeState != leafstate)this->rootnode->NodeState = rootstate;
		return;
	}
	for (i = 0; i < temp->parent->children.size(); i++)
	{
		if (temp->parent->children[i] == temp)
		{
			if (i != temp->parent->children.size() - 1)
			{
				mergenode = temp;
				secnode = temp->sibling;
				break;
			}
			else {
				mergenode = temp->parent->children[i - 1];
				secnode = temp;
				break;
			}
		}
	}
	// if(mergenode != NULL) cout<<"mergenode : "<<mergenode->key[0]<<endl;
	// if(secnode != NULL)cout<<"secnode : "<<secnode->key[0]<<endl;
	if (mergenode != NULL) {
		child_num = mergenode->children.size();
		key_num = mergenode->key.size();
	}
	if (mergenode->NodeState == leafstate)
	{
		for (i = 0; i < secnode->key.size(); i++)
		{
			mergenode->page.push_back(secnode->page[i]);
			mergenode->key.push_back(secnode->key[i]);
			mergenode->page[key_num + i] = secnode->page[i];
			mergenode->key[key_num + i] = secnode->key[i];
		}
	}
	else if (mergenode->NodeState != leafstate)
	{
		if(secnode != NULL)
		for (i = 0; i < secnode->children.size(); i++)
		{
			mergenode->key.push_back(secnode->key[0]);
			mergenode->children.push_back(secnode->children[0]);
			if (i == 0)
			{
				mergenode->key[key_num] = secnode->key[0];
			}
			else mergenode->key[key_num + i] = secnode->key[i - 1];//?
			mergenode->children[child_num + i] = secnode->children[i];
			secnode->children[i]->parent = mergenode;
		}
	}
	mergenode->sibling = secnode->sibling;
	//对parent也要修改。
	for (i = 0; i < mergenode->parent->children.size(); i++)
	{
		if (mergenode->parent->children[i] == secnode)
		{
			for (j = i; j < mergenode->parent->children.size() - 1; j++)
			{
				mergenode->parent->children[j] = mergenode->parent->children[j + 1];
			}
			mergenode->parent->children.pop_back();
			for (j = i; j < mergenode->parent->key.size(); j++)
			{
				mergenode->parent->key[j - 1] = mergenode->parent->key[j];
			}
			mergenode->parent->key.pop_back();
		}
	}

	{secnode->parent = NULL;
	secnode->sibling = NULL;
	for (i = 0; i < secnode->page.size(); i++)secnode->page.pop_back();
	for (i = 0; i < secnode->children.size(); i++)secnode->children.pop_back();
	delete secnode; }

	if (mergenode->key.size() > nodecapacity - 1)this->split(mergenode);
	else if (mergenode->parent->key.size() < nodecapacity / 2 && mergenode->parent->NodeState != rootstate)this->merge(mergenode->parent);
	else if (mergenode->parent->key.size() == 0 && mergenode->parent->NodeState == rootstate)
	{
		this->nodenumber--;
		if (mergenode->NodeState != leafstate)mergenode->NodeState = rootstate;
		mergenode->parent = NULL;
		this->rootnode = mergenode;
	}
}

//新增的范围删除和选择有点奇怪，请理解含义再使用。
//选择是前闭后开的，比如，树中有15,16,17,18,19，范围选择函数给的两个参数key是16,18，意思就是选择所有>=16且<=18 的数。但是返回的是16和19,
//16是第一个大于等于16的，19是第一个大于18的，这就是返回值的前闭后开的意思，这么写有利于循环的写法
//范围选择必须要给定两个范围，所以必要的时候，需要设置该类型变量下的无穷大的数。
//范围删除是在范围选择的基础上写的，但是注意，范围删除没有返回值，按照上面的举例，会删掉16,17,18，即所有【16,18】的值。



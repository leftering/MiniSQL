#include "index.h"
extern type_tablelist a_table_list;//index
extern type_tablelist* t;
type_tablelist::type_tablelist()
{
    this->int_treelist = vector<bptree<int>*>(0);
    this->string_treelist = vector<bptree<string>*>(0);
    this->float_treelist = vector<bptree<float>*>(0);
    // read_from_record_file();
}
type_tablelist::~type_tablelist()
{
    write_all_tree_to_file();
    while (int_treelist.size() > 0)int_treelist.pop_back();
    while (float_treelist.size() > 0)float_treelist.pop_back();
    while (string_treelist.size() > 0)string_treelist.pop_back();
}

bptree<int>* type_tablelist::create_tree_int(string indexname, string table_name, string attributename, char type)
{
    bptree<int>* x = NULL;
    x = new bptree<int>(indexname, table_name, attributename, 'i');
    this->int_treelist.push_back(x);
    this->int_treelist[int_treelist.size() - 1] = x;
    // cout<<"in the create_tree_int: x = "<<x.index_filename<<"-"<<x.index_attributename<<"-"<<x.rootnode->NodeState<<endl;
    // cout<<"in the create_tree_int: int_treelist["<<int_treelist.size()-1 <<"] = "<<int_treelist[int_treelist.size()-1]->index_filename;
    // cout<<"-"<<int_treelist[int_treelist.size()-1]->index_attributename<<"-"<<int_treelist[int_treelist.size()-1]->rootnode->NodeState<<endl;
    return int_treelist[int_treelist.size() - 1];
}
bptree<string>* type_tablelist::create_tree_string(string indexname, string table_name, string attributename, char type)
{
    bptree<string>* x;
    x = new bptree<string>(indexname, table_name, attributename, 's');
    this->string_treelist.push_back(x);
    this->string_treelist[string_treelist.size() - 1] = x;
    return string_treelist[string_treelist.size() - 1];
}
bptree<float>* type_tablelist::create_tree_float(string indexname, string table_name, string attributename, char type)
{
    bptree<float>* x;
    x = new bptree<float>(indexname, table_name, attributename, 'f');
    this->float_treelist.push_back(x);
    this->float_treelist[float_treelist.size() - 1] = x;
    return float_treelist[float_treelist.size() - 1];
}

int type_tablelist::drop_tree_int(string indexname, string table_name, string attribute_name)
{
    int result;
    string filename;
    filename = indexname + ".txt";
    result = remove(filename.c_str());
    if (result == 1)return 0;
    int i, j;
    for (i = 0; i < int_treelist.size(); i++)
    {
        if (int_treelist[i]->index_filename == table_name && (int_treelist[i]->index_attributename == attribute_name))
        {
            int_treelist[i]->~bptree<int>();
            for (j = i + 1; j < int_treelist.size(); j++)
            {
                int_treelist[j - 1] = int_treelist[j];
            }
            int_treelist.pop_back();
        }
    }
    filename = table_name + "#" + attribute_name + ".tree";
    ifstream fin(filename.c_str());
    if (fin.is_open())//有文件
    {
        if (remove(filename.c_str()))return 0;
    }
    return 1;
}
int type_tablelist::drop_tree_float(string indexname, string table_name, string attribute_name)
{
    int result;
    string filename;
    filename = indexname + ".txt";
    result = remove(filename.c_str());
    if (result == 1)return 0;
    int i, j;
    for (i = 0; i < float_treelist.size(); i++)
    {
        if (float_treelist[i]->index_filename == table_name && (float_treelist[i]->index_attributename == attribute_name))
        {
            float_treelist[i]->~bptree<float>();
            for (j = i + 1; j < float_treelist.size(); j++)
            {
                float_treelist[j - 1] = float_treelist[j];
            }
            float_treelist.pop_back();
        }
    }
    filename = table_name + "#" + attribute_name + ".tree";
    ifstream fin(filename.c_str());
    if (fin.is_open())//有文件
    {
        if (remove(filename.c_str()))return 0;
    }
    return 1;
}
int type_tablelist::drop_tree_string(string indexname, string table_name, string attribute_name)
{
    int result;
    string filename;
    filename = indexname + ".txt";
    result = remove(filename.c_str());
    if (result == 1)return 0;
    int i, j;
    for (i = 0; i < string_treelist.size(); i++)
    {
        if (string_treelist[i]->index_filename == table_name && (string_treelist[i]->index_attributename == attribute_name))
        {
            string_treelist[i]->~bptree<string>();
            for (j = i + 1; j < string_treelist.size(); j++)
            {
                string_treelist[j - 1] = string_treelist[j];
            }
            string_treelist.pop_back();
        }
    }
    filename = table_name + "#" + attribute_name + ".tree";
    ifstream fin(filename.c_str());
    if (fin.is_open())//有文件
    {
        if (remove(filename.c_str()))return 0;
    }
    return 1;
}

//please use t as a extern global variable
//非范围查找，插入，删除
int insert_index_int(string table_name, string attributename, int key, address a)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        aimtree->insertindex(key, a);
        return 1;
    }
    else if (aimtree == NULL)
    {
        cout << "insert failed, can't find the index" << endl;
        return 0;
    }
}

int insert_index_string(string table_name, string attributename, string key, address a)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        aimtree->insertindex(key, a);
        cout << "insert succeeded" << endl;
        return 1;
    }
    else if (aimtree == NULL)
    {
        cout << "insert failed, can't find the index" << endl;
        return 0;
    }
}

int insert_index_float(string table_name, string attributename, float key, address a)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        aimtree->insertindex(key, a);
        cout << "insert succeeded" << endl;
        return 1;
    }
    else if (aimtree == NULL)
    {
        cout << "insert failed, can't find the index" << endl;
        return 0;
    }
}

int delete_index_int(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name, attributename);
    if (aimtree != NULL)
    {

        aimtree->deleteindex(key);
        cout << "delete succeeded" << endl;
        return 1;
    }
    else if (aimtree == NULL)
    {
        cout << "delete failed" << endl;
        return 0;
    }
}

int delete_index_string(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        aimtree->deleteindex(key);
        cout << "delete succeeded" << endl;
        return 1;
    }
    else if (aimtree == NULL)
    {
        cout << "delete failed" << endl;
        return 0;
    }
}

int delete_index_float(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        aimtree->deleteindex(key);
        cout << "delete succeeded" << endl;
        return 1;
    }
    else if (aimtree == NULL)
    {
        cout << "delete failed" << endl;
        return 0;
    }
}

address find_index_int(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        cout << "select succeeded" << endl;
        return aimtree->find_index_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }
}

address find_index_string(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        cout << "select succeeded" << endl;
        return aimtree->find_index_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }
}

address find_index_float(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        cout << "select succeeded" << endl;
        return aimtree->find_index_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }
}

//scope
//范围查找函数：由于需要返回两个边界，上界和下界，因此直接考虑调用了两个函数。且注意，前闭后开，返回的low是包含在所需空间里的，返回的up是不包含的。
//范围删除函数，无返回值，直接一次输入，但是参数同样需要表名、属性名、上界和下界。且同样是前闭后开。
address find_scope_int_low(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        return aimtree->lowerbound_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }
}

address find_scope_int_up(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        return aimtree->upperbound_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }

}

address find_scope_string_low(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        return aimtree->lowerbound_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }
}

address find_scope_string_up(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        return aimtree->upperbound_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }
}

address find_scope_float_low(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        return aimtree->lowerbound_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }
}

address find_scope_float_up(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name, attributename);
    if (aimtree != NULL)
    {
        return aimtree->upperbound_of_key(key);
    }
    else if (aimtree == NULL)
    {
        cout << "select failed" << endl;
        return NULL;
    }//如果是空的，查找应该得到null
}

void delete_scope_int(string table_name, string attributename, int lowkey, int upkey)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name, attributename);
    aimtree->deletescope(lowkey, upkey);
}

void delete_scope_string(string table_name, string attributename, string lowkey, string upkey)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name, attributename);
    aimtree->deletescope(lowkey, upkey);
}

void delete_scope_float(string table_name, string attributename, float lowkey, float upkey)
{
    bptree<float>* aimtree = NULL;
    aimtree->deletescope(lowkey, upkey);
}

//写文件，读文件，从buffer创建初始索引。
void write_to_file_int(bptree<int>* tree)
{
    int i;
    string filename = tree->index_filename;
    filename += "#";
    filename += tree->index_attributename;
    filename += ".tree";
    ofstream out(filename.c_str());
    vector<indexnode<int>*> nodes;
    nodes.push_back(tree->rootnode);
    indexnode<int>* curn;
    int nodenum = 0;//用于记录在文件中这是第几个结点。
    int addrnum = 0;
    while (nodes.size() != 0)
    {
        curn = nodes[nodes.size() - 1];
        curn->node_number = nodenum;
        nodes.pop_back();
        for (i = 0; i < curn->children.size(); i++)
        {
            if (curn->children[i] != NULL)nodes.insert(nodes.begin(), curn->children[i]);
            else {
                // cout<<"curn's child is a NULL"<<endl;
            }
        }
        nodenum++;
    }
    out << tree->nodenumber << endl;
    out << tree->indexname << endl;
    nodenum = 0;
    nodes.push_back(tree->rootnode);
    while (nodes.size() != 0)
    {
        curn = nodes[nodes.size() - 1];
        curn->node_number = nodenum;
        nodes.pop_back();
        out << "#" << nodenum << "?" << curn->NodeState << "@";
        if (curn->sibling != NULL)out << curn->node_number;
        else if (curn->sibling == NULL)out << -1;
        out << "*";
        nodenum++;
        for (i = 0; i < curn->children.size(); i++)
        {
            if (curn->children[i] != NULL)
            {
                nodes.insert(nodes.begin(), curn->children[i]);
                out << "$~" << curn->children[i]->node_number;
                if (i < curn->children.size() - 1)out << '<' << curn->key[i];
            }
            else {
                // cout<<"curn's child is a NULL"<<endl;
            }
        }
        for (i = 0; i < curn->page.size(); i++)
        {
            if (curn->page[i] != NULL)
            {
                out << '<' << curn->key[i] << "&~" << curn->page[i]->block_id << "&" << curn->page[i]->record_id;
            }
            else {
                // cout<<"curn's page is a NULL"<<endl;
            }
        }
        out << '~';
        out << endl;
    }
    out.close();
}

bptree<int>* read_from_file_int(string filename)
{
    int i, j;
    char ch;
    int input1;
    string input2;
    float input3;
    int nodepos;
    int state_change = 0;
    address lastone = NULL;//用来处理addr链表的关系。
    int currkey;//用来处理map的关系
    //0就是没有状态，1是准备接收nodenum，2是准备接收结点类型，3是准备接收sibling
    //4是准备接收子内容
    //5是children过渡状态，6是准备接收children
    //7是page过渡状态，8是准备接收page，9是page2过渡状态，10是准备接收page的二状态
    //11是key
    string tablename;
    string attributename;
    for (i = 0; i < filename.length(); i++)
    {
        if (filename[i] == '#')
        {
            for (j = 0; j < i; j++)
            {
                tablename += "x";
                tablename[j] = filename[j];
            }
            for (j = i + 1; j < filename.length() - 5; j++)
            {
                attributename += "x";
                attributename[j - i - 1] = filename[j];
            }
        }
    }
    int node_number;
    bptree<int>* tree;
    input2 = "tempfile";
    tree = new bptree<int>(input2, tablename, attributename, 'i');

    //记得对addr链表的处理和对map的处理。
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "Can't find the file, please check the file name." << endl;
        return NULL;
    }
    fin >> node_number;
    tree->nodenumber = node_number;
    fin >> input2;
    tree->indexname = input2;
    indexnode<int>** nodearr;
    nodearr = new indexnode<int> * [node_number];
    for (i = 0; i < node_number; i++)
    {
        nodearr[i] = new indexnode<int>;
        nodearr[i]->node_number = i;
    }
    while (!fin.eof())
    {
        if (state_change == 0)
        {
            fin >> ch;
            if (ch == '\n')continue;
            else if (ch == '#')state_change = 1;
            else if (ch == '?')state_change = 2;
            else if (ch == '@')state_change = 3;
            else if (ch == '*')state_change = 4;
        }
        else if (state_change == 1)
        {
            fin >> input1;
            nodepos = input1;
            state_change = 0;
        }
        else if (state_change == 2)
        {
            fin >> input1;
            nodearr[nodepos]->NodeState = input1;
            state_change = 0;
        }
        else if (state_change == 3)
        {
            fin >> input1;
            if (input1 != -1)
                nodearr[nodepos]->sibling = nodearr[input1];
            else if (input1 == -1)
                nodearr[nodepos]->sibling = NULL;
            state_change = 0;
        }
        else if (state_change == 4)
        {
            fin >> ch;//这里不同类型的可能会不同
            if (ch == '<')
            {
                state_change = 11;
            }
            else if (ch == '$')
            {
                state_change = 5;
            }
            else if (ch == '&')
            {
                state_change = 7;
            }
            else if (ch == '~')
            {
                state_change = 0;
            }
        }
        else if (state_change == 5)
        {
            fin >> ch;
            if (ch == '~')
            {
                state_change = 6;
            }
        }
        else if (state_change == 6)
        {
            fin >> input1;
            nodearr[nodepos]->children.push_back(NULL);
            nodearr[nodepos]->children[nodearr[nodepos]->children.size() - 1] = nodearr[input1];
            nodearr[input1]->parent = nodearr[nodepos];
            state_change = 4;
        }
        else if (state_change == 7)
        {
            fin >> ch;
            if (ch == '~')
            {
                state_change = 8;
            }
        }
        else if (state_change == 8)
        {
            fin >> input1;
            nodearr[nodepos]->page.push_back(NULL);
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1] = new struct addr;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->block_id = input1;
            state_change = 9;
        }
        else if (state_change == 9)
        {
            fin >> ch;
            if (ch == '&')
            {
                state_change = 10;
            }
        }
        else if (state_change == 10)
        {
            fin >> input1;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->record_id = input1;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->last_addr = lastone;
            if (lastone != NULL)lastone->next_addr = nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1];
            lastone = nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1];
            if (nodepos == tree->nodenumber - 1)nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->next_addr = NULL;
            tree->ak[nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]] = currkey;
            state_change = 4;
        }
        else if (state_change == 11)
        {
            fin >> input1;
            nodearr[nodepos]->key.push_back(input1);
            nodearr[nodepos]->key[nodearr[nodepos]->key.size() - 1] = input1;
            currkey = nodearr[nodepos]->key[nodearr[nodepos]->key.size() - 1];
            state_change = 4;
        }
    }
    tree->rootnode = nodearr[0];
    fin.close();
    return tree;
}

void write_to_file_float(bptree<float>* tree)
{
    int i;
    string filename = tree->index_filename;
    filename += "#";
    filename += tree->index_attributename;
    filename += ".tree";
    ofstream out(filename.c_str());
    vector<indexnode<float>*> nodes;
    nodes.push_back(tree->rootnode);
    indexnode<float>* curn;
    int nodenum = 0;//用于记录在文件中这是第几个结点。
    int addrnum = 0;
    while (nodes.size() != 0)
    {
        curn = nodes[nodes.size() - 1];
        curn->node_number = nodenum;
        nodes.pop_back();
        for (i = 0; i < curn->children.size(); i++)
        {
            if (curn->children[i] != NULL)nodes.insert(nodes.begin(), curn->children[i]);
            else {
                cout << "curn's child is a NULL" << endl;
            }
        }
        nodenum++;
    }
    out << tree->nodenumber << endl;
    out << tree->indexname << endl;
    nodenum = 0;
    nodes.push_back(tree->rootnode);
    while (nodes.size() != 0)
    {
        curn = nodes[nodes.size() - 1];
        curn->node_number = nodenum;
        nodes.pop_back();
        out << "#" << nodenum << "?" << curn->NodeState << "@";
        if (curn->sibling != NULL)out << curn->node_number;
        else if (curn->sibling == NULL)out << -1;
        out << "*";
        nodenum++;
        for (i = 0; i < curn->children.size(); i++)
        {
            if (curn->children[i] != NULL)
            {
                nodes.insert(nodes.begin(), curn->children[i]);
                out << "$~" << curn->children[i]->node_number;
                if (i < curn->children.size() - 1)out << '<' << curn->key[i];
            }
            else {
                cout << "curn's child is a NULL" << endl;
            }
        }
        for (i = 0; i < curn->page.size(); i++)
        {
            if (curn->page[i] != NULL)
            {
                out << '<' << curn->key[i] << "&~" << curn->page[i]->block_id << "&" << curn->page[i]->record_id;
            }
            else {
                cout << "curn's page is a NULL" << endl;
            }
        }
        out << '~';
        out << endl;
    }
    out.close();
}

bptree<float>* read_from_file_float(string filename)
{
    int i, j;
    char ch;
    int input1;
    string input2;
    float input3;
    int nodepos;
    int state_change = 0;
    address lastone = NULL;//用来处理addr链表的关系。
    float currkey;//用来处理map的关系
    //0就是没有状态，1是准备接收nodenum，2是准备接收结点类型，3是准备接收sibling
    //4是准备接收子内容
    //5是children过渡状态，6是准备接收children
    //7是page过渡状态，8是准备接收page，9是page2过渡状态，10是准备接收page的二状态
    //11是key
    string tablename;
    string attributename;
    for (i = 0; i < filename.length(); i++)
    {
        if (filename[i] == '#')
        {
            for (j = 0; j < i; j++)
            {
                tablename += "x";
                tablename[j] = filename[j];
            }
            for (j = i + 1; j < filename.length() - 5; j++)
            {
                attributename += "x";
                attributename[j - i - 1] = filename[j];
            }
        }
    }
    int node_number;//记录节点的数量，
    bptree<float>* tree;
    input2 = "xxx";
    tree = new bptree<float>(input2, tablename, attributename, 'f');
    //
    //记得对addr链表的处理和对map的处理。
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "Can't find the file, please check the file name." << endl;
        return NULL;
    }
    fin >> node_number;
    tree->nodenumber = node_number;
    fin >> input2;
    tree->indexname = input2;
    indexnode<float>** nodearr;
    nodearr = new indexnode<float> * [node_number];
    for (i = 0; i < node_number; i++)
    {
        nodearr[i] = new indexnode<float>;
        nodearr[i]->node_number = i;
    }
    while (!fin.eof())
    {
        if (state_change == 0)
        {
            fin >> ch;
            if (ch == '\n')continue;
            else if (ch == '#')state_change = 1;
            else if (ch == '?')state_change = 2;
            else if (ch == '@')state_change = 3;
            else if (ch == '*')state_change = 4;
        }
        else if (state_change == 1)
        {
            fin >> input1;
            nodepos = input1;
            state_change = 0;
        }
        else if (state_change == 2)
        {
            fin >> input1;
            nodearr[nodepos]->NodeState = input1;
            state_change = 0;
        }
        else if (state_change == 3)
        {
            fin >> input1;
            if (input1 != -1)
                nodearr[nodepos]->sibling = nodearr[input1];
            else if (input1 == -1)
                nodearr[nodepos]->sibling = NULL;
            state_change = 0;
        }
        else if (state_change == 4)
        {
            fin >> ch;//这里不同类型的可能会不同
            if (ch == '<')
            {
                state_change = 11;
            }
            else if (ch == '$')
            {
                state_change = 5;
            }
            else if (ch == '&')
            {
                state_change = 7;
            }
            else if (ch == '~')
            {
                state_change = 0;
            }
        }
        else if (state_change == 5)
        {
            fin >> ch;
            if (ch == '~')
            {
                state_change = 6;
            }
        }
        else if (state_change == 6)
        {
            fin >> input1;
            nodearr[nodepos]->children.push_back(NULL);
            nodearr[nodepos]->children[nodearr[nodepos]->children.size() - 1] = nodearr[input1];
            state_change = 4;
        }
        else if (state_change == 7)
        {
            fin >> ch;
            if (ch == '~')
            {
                state_change = 8;
            }
        }
        else if (state_change == 8)
        {
            fin >> input1;
            nodearr[nodepos]->page.push_back(NULL);
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1] = new struct addr;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->block_id = input1;
            state_change = 9;
        }
        else if (state_change == 9)
        {
            fin >> ch;
            if (ch == '&')
            {
                state_change = 10;
            }
        }
        else if (state_change == 10)
        {
            fin >> input1;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->record_id = input1;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->last_addr = lastone;
            if (lastone != NULL)lastone->next_addr = nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1];
            lastone = nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1];
            if (nodepos == tree->nodenumber - 1)nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->next_addr = NULL;
            tree->ak[nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]] = currkey;
            state_change = 4;
        }
        else if (state_change == 11)
        {
            fin >> input3;
            nodearr[nodepos]->key.push_back(input3);
            nodearr[nodepos]->key[nodearr[nodepos]->key.size() - 1] = input3;
            currkey = nodearr[nodepos]->key[nodearr[nodepos]->key.size() - 1];
            state_change = 4;
        }
    }
    tree->rootnode = nodearr[0];
    fin.close();
    return tree;
}

void write_to_file_string(bptree<string>* tree)
{
    int i;
    string filename = tree->index_filename;
    filename += "#";
    filename += tree->index_attributename;
    filename += ".tree";
    ofstream out(filename.c_str());
    vector<indexnode<string>*> nodes;
    nodes.push_back(tree->rootnode);
    indexnode<string>* curn;
    int nodenum = 0;//用于记录在文件中这是第几个结点。
    int addrnum = 0;
    while (nodes.size() != 0)
    {
        curn = nodes[nodes.size() - 1];
        curn->node_number = nodenum;
        nodes.pop_back();
        for (i = 0; i < curn->children.size(); i++)
        {
            if (curn->children[i] != NULL)nodes.insert(nodes.begin(), curn->children[i]);
            else {
                cout << "curn's child is a NULL" << endl;
            }
        }
        nodenum++;
    }
    out << tree->nodenumber << endl;
    out << tree->indexname << endl;
    nodenum = 0;
    nodes.push_back(tree->rootnode);
    while (nodes.size() != 0)
    {
        curn = nodes[nodes.size() - 1];
        curn->node_number = nodenum;
        nodes.pop_back();
        out << "#" << nodenum << "?" << curn->NodeState << "@";
        if (curn->sibling != NULL)out << curn->node_number;
        else if (curn->sibling == NULL)out << -1;
        out << "*";
        nodenum++;
        for (i = 0; i < curn->children.size(); i++)
        {
            if (curn->children[i] != NULL)
            {
                nodes.insert(nodes.begin(), curn->children[i]);
                out << "$~" << curn->children[i]->node_number;
                if (i < curn->children.size() - 1)out << '<' << curn->key[i] << endl;
            }
            else {
                cout << "curn's child is a NULL" << endl;
            }
        }
        for (i = 0; i < curn->page.size(); i++)
        {
            if (curn->page[i] != NULL)
            {
                out << '<' << curn->key[i] << endl << "&~" << curn->page[i]->block_id << "&" << curn->page[i]->record_id;
            }
            else {
                cout << "curn's page is a NULL" << endl;
            }
        }
        out << '~';
        out << endl;
    }

    out.close();
}

bptree<string>* read_from_file_string(string filename)
{
    int i, j;
    char ch;
    int input1;
    string input2;
    float input3;
    int nodepos;
    int state_change = 0;
    address lastone = NULL;//用来处理addr链表的关系。
    string currkey;//用来处理map的关系
    //0就是没有状态，1是准备接收nodenum，2是准备接收结点类型，3是准备接收sibling
    //4是准备接收子内容
    //5是children过渡状态，6是准备接收children
    //7是page过渡状态，8是准备接收page，9是page2过渡状态，10是准备接收page的二状态
    //11是key
    string tablename;
    string attributename;
    for (i = 0; i < filename.length(); i++)
    {
        if (filename[i] == '#')
        {
            for (j = 0; j < i; j++)
            {
                tablename += "x";
                tablename[j] = filename[j];
            }
            for (j = i + 1; j < filename.length() - 5; j++)
            {
                attributename += "x";
                attributename[j - i - 1] = filename[j];
            }
        }
    }
    int node_number;
    bptree<string>* tree;
    input2 = "xxx";
    tree = new bptree<string>(input2, tablename, attributename, 's');
    //
    //记得对addr链表的处理和对map的处理。
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "Can't find the file, please check the file name." << endl;
        return NULL;
    }
    fin >> node_number;
    tree->nodenumber = node_number;
    fin >> input2;
    tree->indexname = input2;
    indexnode<string>** nodearr;
    nodearr = new indexnode<string> * [node_number];
    for (i = 0; i < node_number; i++)
    {
        nodearr[i] = new indexnode<string>;
        nodearr[i]->node_number = i;
    }
    while (!fin.eof())
    {
        if (state_change == 0)
        {
            fin >> ch;
            if (ch == '\n')continue;
            else if (ch == '#')state_change = 1;
            else if (ch == '?')state_change = 2;
            else if (ch == '@')state_change = 3;
            else if (ch == '*')state_change = 4;
        }
        else if (state_change == 1)
        {
            fin >> input1;
            nodepos = input1;
            state_change = 0;
        }
        else if (state_change == 2)
        {
            fin >> input1;
            nodearr[nodepos]->NodeState = input1;
            state_change = 0;
        }
        else if (state_change == 3)
        {
            fin >> input1;
            if (input1 != -1)
                nodearr[nodepos]->sibling = nodearr[input1];
            else if (input1 == -1)
                nodearr[nodepos]->sibling = NULL;
            state_change = 0;
        }
        else if (state_change == 4)
        {
            fin >> ch;//这里不同类型的可能会不同
            if (ch == '<')
            {
                state_change = 11;
            }
            else if (ch == '$')
            {
                state_change = 5;
            }
            else if (ch == '&')
            {
                state_change = 7;
            }
            else if (ch == '~')
            {
                state_change = 0;
            }
        }
        else if (state_change == 5)
        {
            fin >> ch;
            if (ch == '~')
            {
                state_change = 6;
            }
        }
        else if (state_change == 6)
        {
            fin >> input1;
            nodearr[nodepos]->children.push_back(NULL);
            nodearr[nodepos]->children[nodearr[nodepos]->children.size() - 1] = nodearr[input1];
            state_change = 4;
        }
        else if (state_change == 7)
        {
            fin >> ch;
            if (ch == '~')
            {
                state_change = 8;
            }
        }
        else if (state_change == 8)
        {
            fin >> input1;
            nodearr[nodepos]->page.push_back(NULL);
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1] = new struct addr;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->block_id = input1;
            state_change = 9;
        }
        else if (state_change == 9)
        {
            fin >> ch;
            if (ch == '&')
            {
                state_change = 10;
            }
        }
        else if (state_change == 10)
        {
            fin >> input1;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->record_id = input1;
            nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->last_addr = lastone;
            if (lastone != NULL)lastone->next_addr = nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1];
            lastone = nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1];
            if (nodepos == tree->nodenumber - 1)nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]->next_addr = NULL;
            tree->ak[nodearr[nodepos]->page[nodearr[nodepos]->page.size() - 1]] = currkey;
            state_change = 4;
        }
        else if (state_change == 11)
        {
            // input2 = "";
            // fin>>ch;
            // while(ch != '>')
            // {
            //     input2 += "x";
            //     input2[input2.length()-1] = ch;
            //     fin>>ch;
            // }
            fin >> input2;
            nodearr[nodepos]->key.push_back(input2);
            nodearr[nodepos]->key[nodearr[nodepos]->key.size() - 1] = input2;
            currkey = nodearr[nodepos]->key[nodearr[nodepos]->key.size() - 1];
            state_change = 4;
        }
    }
    tree->rootnode = nodearr[0];
    fin.close();
    return tree;
}

bptree<int>* type_tablelist::find_int_tree(string filename, string attributename)
{
    int i;
    for (i = 0; i < int_treelist.size(); i++)
    {
        if (int_treelist[i]->index_filename == filename && (int_treelist[i]->index_attributename == attributename))
        {
            return int_treelist[i];
        }
    }
    string file_name;//这个才是存储的文件名
    file_name = filename + "#" + attributename + ".tree";
    ifstream fin(file_name.c_str());
    if (fin.is_open())
    {
        fin.close();
        bptree<int>* newtree = read_from_file_int(file_name);
        t->int_treelist.push_back(newtree);
        return newtree;
    }
    else return NULL;
}
bptree<string>* type_tablelist::find_string_tree(string filename, string attributename)
{
    int i;
    for (i = 0; i < string_treelist.size(); i++)
    {
        if (string_treelist[i]->index_filename == filename && (string_treelist[i]->index_attributename == attributename))
        {
            return string_treelist[i];
        }
    }
    string file_name;//这个才是存储的文件名
    file_name = filename + "#" + attributename + ".tree";
    ifstream fin(file_name.c_str());
    if (fin.is_open())
    {
        fin.close();
        bptree<string>* newtree = read_from_file_string(file_name);
        t->string_treelist.push_back(newtree);
        return newtree;
    }
    else return NULL;
}
bptree<float>* type_tablelist::find_float_tree(string filename, string attributename)
{
    int i;
    for (i = 0; i < float_treelist.size(); i++)
    {
        if (float_treelist[i]->index_filename == filename && (float_treelist[i]->index_attributename == attributename))
        {
            return float_treelist[i];
        }
    }
    string file_name;//这个才是存储的文件名
    file_name = filename + "#" + attributename + ".tree";
    ifstream fin(file_name.c_str());
    if (fin.is_open())
    {
        fin.close();
        bptree<float>* newtree = read_from_file_float(file_name);
        t->float_treelist.push_back(newtree);
        return newtree;
    }
    else return NULL;
}
//程序结束时调用。可以在全局变量的析构里面。
void type_tablelist::write_all_tree_to_file()
{
    int i;
    for (i = 0; i < this->int_treelist.size(); i++)
    {
        write_to_file_int(this->int_treelist[i]);
    }
    for (i = 0; i < this->float_treelist.size(); i++)
    {
        write_to_file_float(this->float_treelist[i]);
    }
    for (i = 0; i < this->string_treelist.size(); i++)
    {
        write_to_file_string(this->string_treelist[i]);
    }
}

int create_index_from_record(string index_name, string tablename, string attributename)//建立index
{
    if (t->find_int_tree(tablename, attributename) != NULL)return 0;//如果之前建立过索引，就不读了这个文件。
    //没建立过索引的record，相当于初始状态，需要扫描读取。
    int i, j, k;
    int treebuild = 0;
    int blocknum;
    In.table.get_table_info(tablename);
    Block* temppage;//存放这个block的地址的临时指针
    BYTE* tempdata;//存放这个block的data的地址的临时指针
    BYTE* temprecord;//存放当前record的地址的临时指针
    blocknum = buffer_manager.getBlockNum(tablename);
    for (k = 0; k < In.table.col_num; k++)
    {
        if (In.table.col[k].col_name == attributename)
        {
            if (In.table.col[k].col_type == 0)
            {
                t->create_tree_int(index_name, tablename, attributename, 'i');
            }
            else if (In.table.col[k].col_type == 1)
            {
                t->create_tree_float(index_name, tablename, attributename, 'f');
            }
            else if (In.table.col[k].col_type == 2)
            {
                t->create_tree_string(index_name, tablename, attributename, 's');
            }
            break;
        }
    }
    if (blocknum == 0)return 1;
    int key1;
    string key2;
    float key3;
    for (i = 0; i < blocknum; i++)
    {
        temppage = buffer_manager.getPage(tablename, i);
        tempdata = temppage->getData();
        for (j = 1; j <= tempdata[0]; j++)
        {
            address tempaddr = create_addr();
            tempaddr->block_id = i;
            tempaddr->record_id = j;
            temprecord = temppage->getRecord(j);//遍历生成新的地址，用于建立索引
            Tuple temptuple = record_manager.read2tuple(temprecord, In.table);
            for (k = 0; k < In.table.col_num; k++)
            {
                if (In.table.col[k].col_name == attributename)
                {
                    if (temptuple.getData()[k].type == -2)
                    {
                        key1 = temptuple.getData()[k].datai;
                        insert_index_int(tablename, attributename, key1, tempaddr);
                    }
                    else if (temptuple.getData()[k].type == -1)
                    {
                        key3 = temptuple.getData()[k].dataf;
                        insert_index_float(tablename, attributename, key3, tempaddr);
                    }
                    else if (temptuple.getData()[k].type >= 0)
                    {
                        key2 = temptuple.getData()[k].datas;
                        insert_index_string(tablename, attributename, key2, tempaddr);
                    }
                }
            }
        }
    }
    return 1;
    //读文件，得到block，遍历地址，然后得到每个地址对应的record中的key，就足够建立一个索引了。
}

int drop_index(string index_name)
{
    char type;
    string tablename;
    string attributename;
    int result = 0;
    ifstream fin((index_name + ".txt").c_str());
    if (!fin.is_open()) { cout << "can't find the index" << endl; return 0; }
    fin >> type;
    fin >> tablename;
    fin >> attributename;
    if (type == 'i')
    {
        result = t->drop_tree_int(index_name, tablename, attributename);
    }
    else if (type == 'f')
    {
        result = t->drop_tree_float(index_name, tablename, attributename);
    }
    else if (type == 's')
    {
        result = t->drop_tree_string(index_name, tablename, attributename);
    }
    return result;
}
//数据库程序结束的时候需要析构t指向的type_tablelist，从而保存所有的树。
//千万注意！！！！同一个addr，一定不能插入两次！！！不然会死掉。已经尽可能避免了。请在外部格外添加一个判断程序，如果这个地址插入过了，一定不要再插入了。

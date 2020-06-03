#ifndef CATALOG_H
#define CATALOG_H

#include "pch.h"
#include "bplustree.h"
#define rootstate 2
#define interstate 1
#define leafstate 0
class type_tablelist{
public:
    vector<bptree<int>*> int_treelist;
    vector<bptree<string>*> string_treelist;
    vector<bptree<float>*> float_treelist;
public:
    type_tablelist();
    bptree<int>* find_int_tree(string filename, string attributename);
    bptree<string>* find_string_tree(string filename, string attributename);
    bptree<float>* find_float_tree(string filename, string attributename);
    bptree<int>* create_tree_int(string table_name,string attributename, char type);
    bptree<string>* create_tree_string(string table_name,string attributename, char type);
    bptree<float>* create_tree_float(string table_name,string attributename, char type);
    void delete_tree_int(string table_name,string attributename);
    void delete_tree_float(string table_name,string attributename);
    void delete_tree_string(string table_name,string attributename);
};
type_tablelist::type_tablelist()
{
    this->int_treelist = vector<bptree<int>*>(0);
    this->string_treelist = vector<bptree<string>*>(0);
    this->float_treelist = vector<bptree<float>*>(0);
}

bptree<int>* type_tablelist::find_int_tree(string filename, string attributename)
{
    int i;
    for(i = 0;i<int_treelist.size();i++)
    {
        if(int_treelist[i]->index_filename == filename && (int_treelist[i]->index_attributename == attributename))
        {
            return int_treelist[i];
        }
    }
    return NULL;
}
bptree<string>* type_tablelist::find_string_tree(string filename, string attributename)
{
    int i;
    for(i = 0;i<string_treelist.size();i++)
    {
        if(string_treelist[i]->index_filename == filename&&(string_treelist[i]->index_attributename == attributename))
        {
            return string_treelist[i];
        }
    }
    return NULL;
}
bptree<float>* type_tablelist::find_float_tree(string filename, string attributename)
{
    int i;
    for(i = 0;i<float_treelist.size();i++)
    {
        if(float_treelist[i]->index_filename == filename&&(float_treelist[i]->index_attributename == attributename))
        {
            return float_treelist[i];
        }
    }
    return NULL;
}


bptree<int>* type_tablelist::create_tree_int(string table_name,string attributename, char type)
{
    this->int_treelist.push_back(0);
    static bptree<int> x(table_name,attributename,'i');
    this->int_treelist[int_treelist.size()-1] = &x;
    // cout<<"in the create_tree_int: x = "<<x.index_filename<<"-"<<x.index_attributename<<"-"<<x.rootnode->NodeState<<endl;
    // cout<<"in the create_tree_int: int_treelist["<<int_treelist.size()-1 <<"] = "<<int_treelist[int_treelist.size()-1]->index_filename;
    // cout<<"-"<<int_treelist[int_treelist.size()-1]->index_attributename<<"-"<<int_treelist[int_treelist.size()-1]->rootnode->NodeState<<endl;
    return int_treelist[int_treelist.size()-1];
}
bptree<string>* type_tablelist::create_tree_string(string table_name,string attributename, char type)
{
    this->string_treelist.push_back(0);
    static bptree<string> x(table_name,attributename,'i');
    this->string_treelist[string_treelist.size()-1] = &x;
    return string_treelist[string_treelist.size()-1];
}
bptree<float>* type_tablelist::create_tree_float(string table_name,string attributename, char type)
{
    this->float_treelist.push_back(0);
    static bptree<float> x(table_name,attributename,'i');
    this->float_treelist[float_treelist.size()-1] = &x;
    return float_treelist[float_treelist.size()-1];
}


type_tablelist a_table_list;
type_tablelist* t = &a_table_list;
//please use t as a extern global variable

void create_index_int(string table_name, string attributename, int key, address a)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        aimtree->insertindex(key,a);
    }
    else if(aimtree == NULL)
    {
        // cout<<"find_int_tree returns null"<<endl;
        aimtree = t->create_tree_int(table_name,attributename,'i');
        // cout<<"create_index_int"<<aimtree->index_filename<<aimtree->rootnode->NodeState<<endl;
        aimtree->insertindex(key,a);
    }
}

void create_index_string(string table_name, string attributename, string key, address a)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        aimtree->insertindex(key,a);
    }
    else if(aimtree == NULL)
    {
        aimtree = t->create_tree_string(table_name,attributename,'s');
        aimtree->insertindex(key,a);
    }
}

void create_index_float(string table_name, string attributename, float key, address a)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        aimtree->insertindex(key,a);
    }
    else if(aimtree == NULL)
    {
        aimtree = t->create_tree_float(table_name,attributename,'f');
        aimtree->insertindex(key,a);
    }
}

void delete_index_int(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        // cout<<"delete_index_int"<<endl;
        aimtree->deleteindex(key);
    }
    else if(aimtree == NULL)
    {
        // cout<<"not find this index"<<endl;
    }
}

void delete_index_string(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        aimtree->deleteindex(key);
    }
    else if(aimtree == NULL)
    {
        cout<<"not find this index"<<endl;
    }
}

void delete_index_float(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        aimtree->deleteindex(key);
    }
    else if(aimtree == NULL)
    {
        cout<<"not find this index"<<endl;
    }
}

address find_index_int(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->find_index_of_key(key);
    }
    else if(aimtree == NULL)
    {
        cout<<"not find"<<endl;
        return NULL;
    }
}

address find_index_string(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->find_index_of_key(key);
    }
    else if(aimtree == NULL)
    {
        cout<<"not find"<<endl;
        return NULL;
    }
}

address find_index_float(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->find_index_of_key(key);
    }
    else if(aimtree == NULL)
    {
        cout<<"not find"<<endl;
        return NULL;
    }
}

//先实现基本的插入，删除，查找单个。删除多个和查找多个之后再添加。在bplustree的修改中有一个addr的连接不再支持多个同值的搜索。

//scope
//范围查找函数：由于需要返回两个边界，上界和下界，因此直接考虑调用了两个函数。且注意，前闭后开，返回的low是包含在所需空间里的，返回的up是不包含的。
//范围删除函数，无返回值，直接一次输入，但是参数同样需要表名、属性名、上界和下界。且同样是前闭后开。
address find_scope_int_low(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->lowerbound_of_key(key);
    }
    else if(aimtree == NULL)
    {
        // cout<<"find_int_tree returns null"<<endl;
        aimtree = t->create_tree_int(table_name,attributename,'i');
        // cout<<"create_index_int"<<aimtree->index_filename<<aimtree->rootnode->NodeState<<endl;
        return aimtree->lowerbound_of_key(key);
    }
    
}

address find_scope_int_up(string table_name, string attributename, int key)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->upperbound_of_key(key);
    }
    else if(aimtree == NULL)
    {
        // cout<<"find_int_tree returns null"<<endl;
        aimtree = t->create_tree_int(table_name,attributename,'i');
        // cout<<"create_index_int"<<aimtree->index_filename<<aimtree->rootnode->NodeState<<endl;
        return aimtree->upperbound_of_key(key);
    }
    
}

address find_scope_string_low(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->lowerbound_of_key(key);
    }
    else if(aimtree == NULL)
    {
        // cout<<"find_int_tree returns null"<<endl;
        aimtree = t->create_tree_string(table_name,attributename,'i');
        // cout<<"create_index_int"<<aimtree->index_filename<<aimtree->rootnode->NodeState<<endl;
        return aimtree->lowerbound_of_key(key);
    }
}

address find_scope_string_up(string table_name, string attributename, string key)
{
    bptree<string>* aimtree;
    aimtree = t->find_string_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->upperbound_of_key(key);
    }
    else if(aimtree == NULL)
    {
        // cout<<"find_int_tree returns null"<<endl;
        aimtree = t->create_tree_string(table_name,attributename,'i');
        // cout<<"create_index_int"<<aimtree->index_filename<<aimtree->rootnode->NodeState<<endl;
        return aimtree->upperbound_of_key(key);
    }
}

address find_scope_float_low(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->lowerbound_of_key(key);
    }
    else if(aimtree == NULL)
    {
        // cout<<"find_int_tree returns null"<<endl;
        aimtree = t->create_tree_float(table_name,attributename,'i');
        // cout<<"create_index_int"<<aimtree->index_filename<<aimtree->rootnode->NodeState<<endl;
        return aimtree->lowerbound_of_key(key);
    }
}

address find_scope_float_up(string table_name, string attributename, float key)
{
    bptree<float>* aimtree;
    aimtree = t->find_float_tree(table_name,attributename);
    if(aimtree != NULL)
    {
        return aimtree->upperbound_of_key(key);
    }
    else if(aimtree == NULL)
    {
        // cout<<"find_int_tree returns null"<<endl;
        aimtree = t->create_tree_float(table_name,attributename,'i');
        // cout<<"create_index_int"<<aimtree->index_filename<<aimtree->rootnode->NodeState<<endl;
        return aimtree->upperbound_of_key(key);
    }//如果是空的，查找应该得到null
}

void delete_scope_int(string table_name,string attributename, int lowkey, int upkey)
{
    bptree<int>* aimtree;
    aimtree = t->find_int_tree(table_name,attributename);
    aimtree->deletescope(lowkey,upkey);
}

void delete_scope_string(string table_name, string attributename, string lowkey, string upkey)
{
    bptree<string>* aimtree;
    aimtree->deletescope(lowkey,upkey);
}

void delete_scope_float(string table_name,string attributename, float lowkey, float upkey)
{
    bptree<float>* aimtree;
    aimtree->deletescope(lowkey,upkey);
}


#endif // index.h

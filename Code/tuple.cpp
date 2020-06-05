#include "tuple.h"
void Tuple::addData(Data data_in)
{
	contents.push_back(data_in);
}
std::vector<Data> Tuple::getData()const
{
	return contents;
}
void Tuple::eraseData(int i)
{
    contents.erase(contents.begin() + i);
}
int Tuple::getSize()
{
	return contents.size();
}
bool Tuple::isDeleted()
{
    return deleted;
}
void Tuple::setDeleted()
{
    deleted = true;
}
void Tuple::showTuple()
{
    for (int i = 0; i < getSize(); i ++) {
        if (contents[i].type == -2)
            std::cout << setw(16) << setfill(' ') << left << contents[i].datai << '|';
        else if (contents[i].type == -1)
            std::cout << setw(16) << setfill(' ') << left << contents[i].dataf << '|';
        else
            std::cout << setw(16) << setfill(' ') << left << contents[i].datas << '|';
    }
}

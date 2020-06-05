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
            std::cout << contents[i].datai << '\t';
        else if (contents[i].type == 0)
            std::cout << contents[i].dataf << '\t';
        else
            std::cout << contents[i].datas << '\t';
    }
    std::cout << std::endl;
}

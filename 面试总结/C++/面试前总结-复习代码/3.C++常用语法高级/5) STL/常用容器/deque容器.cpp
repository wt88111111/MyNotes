#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<deque>
using namespace std;

void test()
{
	deque<int>::iterator it;
	it++;
	it--;
	it + 2;
	// 数据结构是：连续的（通过中控器的假连续的内存空间）
	// 双向，随机
}

void deqPrint(const deque<int> &deq)
{
	for (deque<int>::const_iterator it = deq.begin(); it != deq.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

/*  deque构造函数

deque<T> deqT;//默认构造形式
deque(beg, end);//构造函数将[beg, end)区间中的元素拷贝给本身。
deque(n, elem);//构造函数将n个elem拷贝给本身。
deque(const deque &deq);//拷贝构造函数。
*/
void test01()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	deque<int> d(arr, arr + sizeof(arr) / sizeof(int));
	deqPrint(d);

	deque<int> d2(5, 6);
	deqPrint(d2);
}

/*  deque赋值操作

assign(beg, end);//将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);//将n个elem拷贝赋值给本身。
deque&operator=(const deque &deq); //重载等号操作符
swap(deq);// 将deq与本身的元素互换
*/
void test02()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	deque<int> d;

	d.assign(arr, arr + sizeof(arr) / sizeof(int));
	deqPrint(d);

	deque<int> d2;
	d2.assign(5, 6);
	deqPrint(d2);

	cout << "-----------------" << endl;
	d2.swap(d);
	deqPrint(d);
	deqPrint(d2);
}

/*  deque大小操作

deque.size();//返回容器中元素的个数
deque.empty();//判断容器是否为空
deque.resize(num);//重新指定容器的长度为num,若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
deque.resize(num, elem); //重新指定容器的长度为num,若容器变长，则以elem值填充新位置,如果容器变短，则末尾超出容器长度的元素被删除。
*/
void test03()
{
	deque<int> d;
	cout << d.size() << endl;
	if (d.empty())
	{
		cout << "空" << endl;
	}

	d.resize(10, 6);
	deqPrint(d);
}

/*  deque双端插入和删除操作

push_back(elem);//在容器尾部添加一个数据
push_front(elem);//在容器头部插入一个数据
pop_back();//删除容器最后一个数据
pop_front();//删除容器第一个数据
*/
void test04()
{
	deque<int> d;
	d.push_back(10);
	d.push_front(20);
	d.push_front(30);
	deqPrint(d);

	d.pop_back();
	d.pop_front();
	deqPrint(d);
}

/*  deque数据存取操作

at(idx);//返回索引idx所指的数据，如果idx越界，抛出out_of_range。
operator[];//返回索引idx所指的数据，如果idx越界，不抛出异常，直接出错。
front();//返回第一个数据。
back();//返回最后一个数据

*/
void test05()
{
	int arr[] = { 1, 2, 3, 4, 5, 6 };
	deque<int> d(arr, arr + sizeof(arr) / sizeof(int));

	for (int i = d.size() - 1; i >= 0; i--)
	{
		cout << d.at(i) << " ";
	}
	cout << endl;

	d[2] = 200;
	deqPrint(d);

	cout << d.front() << " " << d.back() << endl;
}

/*  deque插入操作

insert(pos,elem);//在pos位置插入一个elem元素的拷贝，返回新数据的位置。
insert(pos,n,elem);//在pos位置插入n个elem数据，无返回值。
insert(pos,beg,end);//在pos位置插入[beg,end)区间的数据，无返回值。
*/
void test06()
{
	deque<int> d(8, 6);
	d.insert(d.begin() + 1, 100);
	deqPrint(d);

	d.insert(d.begin() + 1, 2, 200);
	deqPrint(d);

	deque<int> d2(2, 9);
	cout << "-------------" << endl;
	d2.insert(d2.begin() + 1, d.begin(), d.end());
	deqPrint(d2);
}

/*  deque删除操作

clear();//移除容器的所有数据
erase(beg,end);//删除[beg,end)区间的数据，返回下一个数据的位置。
erase(pos);//删除pos位置的数据，返回下一个数据的位置。
*/
void test07()
{
	deque<int> d(5, 8);
	d.erase(d.begin(), d.end());
	deqPrint(d);
}

int main(int argc, char **argv)
{
	test07();

	getchar();
	return 0;
}
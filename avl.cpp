#include <iostream>
#include "avl.h"
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>


using namespace std;
void show()
{
	cout<<endl;
	cout<<"MENU"<<endl;
	cout<<"1. Insert"<<endl;
	cout<<"2. Find"<<endl;
	cout<<"3. Remove"<<endl;
	cout<<"4. Check if empty"<<endl;
	cout<<"5. Size"<<endl;
	cout<<"6. Clear"<<endl;
	cout<<"7. Display"<<endl;
	cout<<"8. Exit"<<endl;
	cout<<"Enter your choice :";
}

int main(int argc, char const *argv[]) 
{
int c;
tree<int> t;
do
{
	show();
	cin>>c;
	int d;
	tree<int>::iterator h;
	tree<int>::iterator it;
	switch(c)
	{
		case 1:
			cout<<"Enter value to insert :";
			cin>>d;
			t.insert(d);
			cout<<endl;
			break;
		
		case 2:
			cout<<"Enter value to find :";
			cin>>d;
			h=t.find(d);
			if(h!=t.end())
				cout<<"\nFound"<<endl;
			else
				cout<<"\nNot Found";
			cout<<endl;	
			break;

		case 3:
			cout<<"Enter value to remove :";
			cin>>d;
			d=t.remove(d);
			if(d==0)
				cout<<"\nElement not found in tree";
			else
				cout<<"\nElement removed successfully";
			cout<<endl;
			break;
		
		case 4:
			cout<<boolalpha<<t.empty();
			cout<<endl;
			break;
		case 5:
			cout<<t.size();
			cout<<endl;
			break;
		case 6:
			t.clear_n();
			break;
		case 7:
			for (it = t.begin(); it != t.end(); ++it)
				std::cout << *it << ", ";
			cout<<endl;
			break;
				
	}
}while(c!=8);
cout<<"\nBye!!\n\n";
cout<<endl;
tree<int> t1;
t1.insert(42);
t1.insert(17);
t1.insert(19);
t1.insert(42);
t1.insert(45);
//cout<<"root: "<<t1.root->data<<endl;
tree<int> t2;
tree<int> t3;
t3=t1;
t2=std::move(t1);
tree<int>::iterator it;
cout<<"t2:\n\n";
for (it = t2.begin(); it != t2.end(); ++it)
				std::cout << *it << ", ";

cout<<endl<<endl<<endl;
cout<<"t3:\n\n";
for (it = t3.begin(); it != t3.end(); ++it)
				std::cout << *it << ", ";

cout<<endl<<endl<<endl;
cout<<"t1:\n\n";
for (it = t1.begin(); it != t1.end(); ++it)
				std::cout << *it << ", ";

cout<<endl<<endl<<endl;
tree<int> t4;
t4.insert(19);
t4.insert(42);
t4.insert(45);
cout<<"t4:\n\n";
for (it = t4.begin(); it != t4.end(); ++it)
				std::cout << *it << ", ";

cout<<endl<<endl<<endl;
bool h=t2==t3;
cout<<"Does t2 and t3 contain the same elements? :"<< boolalpha<<h;
cout<<endl<<endl<<endl;
h=t4==t2;
cout<<"Does t2 and t4 contain the same elements? :"<< boolalpha<<h;
cout<<endl<<endl<<endl;
//cout<<"root: "<<t4.root->left->data<<endl;
/*std::for_each(t4.begin(), t4.end(), [](auto e){ std::cout << e << "\t";}); 
cout<<endl<<endl<<endl;
auto x=std::is_sorted(t4.begin(),t4.end());
cout<<endl<<endl<<endl;
std::cout<<"is sorted : "<<std::boolalpha<<x<<"\n";
cout<<endl<<endl<<endl;
int num_items1 = std::count(t4.begin(), t4.end(),42);
std::cout<<"count()-----count of 42 is "<<num_items1<<"\n";
cout<<endl<<endl<<endl;*/
return 0;
}


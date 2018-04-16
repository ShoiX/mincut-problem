// implements karger's contraction algorithm
#include <iostream>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <vector>
using namespace std;
using namespace std::chrono;

class vertex;

typedef struct node
{
	vertex* id;	// label of the vertex it is referencing
	node* next = NULL;
}node;

class vertex
{
public:
	int n = 0;	// number of vertices adjacent to it
	node* head;
	int label;	// label of the vertex

	void dump()
	{
		cout<<"size: "<<this->n<<endl;
		node* crawler = this->head;
		while(crawler != NULL)
		{
			vertex* a = crawler->id;
			cout<<a->label<<" ";
		}

	}
};

void load(vector<vertex> &vertices)
{
}

int main()
{
	std::vector<vertex> vertices(5);

}
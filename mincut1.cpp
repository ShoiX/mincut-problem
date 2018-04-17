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
	int id;	// label of the vertex it is referencing
	node* next = NULL;
	// pointer to the vertex the node represents
}node;


class edge
{
public:
	vertex* a = NULL;
	vertex* b = NULL;
};


class vertex
{
public:
	int size = 0;	// number of vertices adjacent to it
	node* head;		// head of the ll of vertices adjacent to this
	int label;	// label of the vertex

	void dump()
	{
		cout<<"size: "<<this->size <<endl;
		node* crawler = this->head;
		while(crawler != NULL)
		{
			cout<<crawler->id<<" ";
			crawler = crawler->next;
		}
		cout<<endl;
	}

	void init(int arr[], int l, vector<edge> &edges, vector<vertex> &vertices)
	{
		node* tmp = this->head;


		for (int i = 0; i < l; i++)
		{
			// create an edge
			if (arr[i] > this->label)
			{
				edge edgy;
				edgy.a = this;
				edgy.b = &vertices[arr[i]-1];
				edges.push_back(edgy);
				//cout<<this->label<<"-"<<arr[i]<<endl;
			}
			
			tmp[i].id = arr[i];
			if (i != l - 1)
				tmp[i].next = &tmp[i + 1];
		}
	}

};

bool load(vector<vertex> &vertices, vector<edge> &edges, int n)
{
	for (int i = 0; i < n; i++)
	{
		cin>>vertices[i].label;
		int l;
		cin>>l;
		vertices[i].size = l;
		vertices[i].head = new (nothrow) node[l];
		if (vertices[i].head == nullptr)
			return false;

		// stores all the adjacent vertices 
		//to be initialized on thE current vertex
		int tmp[l];
		
		for (int j = 0; j<l; j++)
			cin>>tmp[j];
		vertices[i].init(tmp, l, edges, vertices);	// create a linked list
	}
	return true;
}

int main()
{
	int n;	// number of vertices
	cin>>n;
	std::vector<vertex> vertices(n);
	std::vector<edge> edges;
	edges.reserve(10 * n);

	if (!load(vertices, edges, n))
	{
		cout<<"Error\n";
		return 1;
	}

	for (int i = 0; i < n; i++)
		vertices[i].dump();

	// delete memory created on the heap
	for (int i = 0; i < n; i++)
	{
		delete[] vertices[i].head;
	}
	for (int i = 0, j = edges.size() ; i < j; i++)
	{
		vertex* x = edges[i].a;
		vertex* y = edges[i].b;
  		cout<<x->label<<"-"<<y->label;
		cout<<endl;
	}
	cout<<edges.size()<<endl;
}
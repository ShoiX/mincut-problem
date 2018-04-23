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
	vertex* present;
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
	vertex* point;	// pointer to the vertex merged (points to self at first)
	void dump()
	{
		cout<<"size: "<<this->size <<endl;
		node* crawler = this->head;
		while(crawler != NULL)
		{
			cout<<crawler->present->label<<" ";
			crawler = crawler->next;
		}
		cout<<endl;
	}

	void init(int arr[], int l, vector<edge> &edges, vector<vertex> &vertices)
	{
		node* tmp = this->head;
		this->point = this;

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
			tmp[i].present = &vertices[arr[i] - 1];
			if (i != l - 1)
				tmp[i].next = &tmp[i + 1];
		}
	}

	// deletes node in the linked-list
	void remove(vertex* adj)
	{
		if (this->head == NULL)
			return;
		node* crawler = this->head;
		node*prev = crawler;
		cout<<"pos1: ";
		if (adj == crawler->present->point)
		{
			if (crawler->next == NULL)
			{
				this->head = NULL;
				return;
			}
			else
			{
				this->head = crawler->next;
				crawler = crawler->next;
				prev->next = NULL;
			}
			this->size -= 1;
			cout<<"removed\n";
		}
		else
			crawler = crawler->next;
		int ctr = 1;
		while (crawler != NULL)
		{
			cout<<crawler->present->point->label<<endl;
			cout<<"pos"<<ctr<<": ";

			if (adj == crawler->present->point)
			{
				// node is head
				if (prev->next == NULL)
				{
					prev = crawler;
					this->head = crawler->next;
					crawler = crawler->next;
					prev->next = NULL;

				}
				// node is tail of ll
				else if (crawler->next == NULL)
				{
					prev->next = NULL;
					crawler = crawler->next;
				}

				// node is in the middle of ll
				else
				{
					node* tmp = crawler->next;
					crawler->next = NULL;
					crawler = tmp;
					prev->next = crawler;
				}
				this->size -= 1;
				cout<<"removed;\n";
			}
			else
			{
				prev = crawler;
				crawler = crawler->next;
			}
			ctr++;
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
		vertices[i].head = new node[l];
		if (vertices[i].head == NULL)
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

void merge(vertex* l, vertex* r)
{
	vertex* x;
	vertex* y;
	if (l->head == NULL)
	{
		x = r;
		y = l;
	}
	else
	{
		x = l;
		y = r;
	}
	node* crawler = x->head;

	// point tail of ll of l vertex to the start of ll of r vertex
	/*tmp1[(x->size) - 1].next = y->head;*/
	while(crawler->next != NULL)
		crawler = crawler->next;
	crawler->next = y->head;

	// point the point field of r to point field of l
	y->head = x->head;
	y->point = x->point;
	int new_size = y->size + x->size;
	y->size = new_size;
	x->size = new_size;



}

int main()
{
	int n;	// number of vertices
	cin>>n;
	if (n == 2)
	{
		cout<<"Graph is already a mincut"<<endl;
		return 2;
	}
	std::vector<vertex> vertices(n);
	std::vector<edge> edges;
	edges.reserve(10 * n);

	if (!load(vertices, edges, n))
	{
		cout<<"Error\n";
		return 1;
	}
	// store are ll handles;
	node* handles[n];
	for (int i = 0; i < n; i++)
	{
		handles[i] = vertices[i].head;
	}
	cout<<"edges1: "<<edges.size()<<endl;
	cout<<"Cs: "<<endl;
	/*Start of the Karger's Contraction Algorithm subroutine*/
	srand(time(NULL));	// seed the pseudo-random generator
	for (int i = 0; i < n - 2; i++)
	{
		// choose a random edge
		cout<<"edges: "<<edges.size()<<endl;
		int c = rand() % edges.size();

		cout<<c<<endl;
		vertex* l = edges[c].a->point;
		vertex* r = edges[c].b->point;

		// delete the nodes in the list that represents self-loop to the contracted vertices
		cout<<"removing "<<r->label<<" of size "<<r->size<<" from "<<l->label<<" of size "<<l->size<<endl;
		l->remove(r);
		cout<<"removing "<<l->label<<" of size "<<l->size<<" from "<<r->label<<" of size "<<r->size<<endl;
		r->remove(l);
		cout<<"merging.."<<endl;
		// append remaining nodes of vertex r to vertexl
		merge(l, r);
		cout<<"deleting.. "<<endl;
		// delete self-loop edges
		for (int p = 0, erased = 0, l = edges.size(); p < l; p++)
		{
			vertex* x = edges[p - erased].a;
			vertex* y = edges[p - erased].b;

			if (x->point == y->point)
			{
				edges.erase(edges.begin() + (p - erased));
				erased ++;
			}
		}
		cout<<i<<"th loop\n";
	}
	
	/*End of the Contraction Algorithm*/

/*	for (int i = 0; i < n; i++)
		vertices[i].point->dump();
*/
	// delete memory created on the heap
	for (int i = 0; i < n; i++)
	{
		delete[] handles[i];
	}
	cout<<"finished deleting heap memory"<<endl;
/*	for (int i = 0, j = edges.size() ; i < j; i++)
	{
		vertex* x = edges[i].a;
		vertex* y = edges[i].b;
  		cout<<x->label<<"-"<<y->label;
		cout<<endl;
	}*/
	cout<<"edges2: "<<edges.size()<<endl;
}
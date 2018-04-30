// implements karger's contraction algorithm
#include <iostream>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <tgmath.h>

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
	vertex* point = NULL;	// pointer to the vertex merged (points to self at first)
	void dump()
	{
		node* crawler = this->head;
		while(crawler != NULL)
		{
			crawler = crawler->next;
		}
	}

	void init(int arr[], int l, vector<edge> &edges, vector<vertex> &vertices)
	{
		node* tmp = this->head;
		// this->point = this;

		for (int i = 0; i < l; i++)
		{
			// create an edge
			if (arr[i] > this->label)
			{
				edge edgy;
				edgy.a = this;
				edgy.b = &vertices[arr[i]-1];
				edges.push_back(edgy);
			}
			
			tmp[i].id = arr[i];
			tmp[i].present = &vertices[arr[i] - 1];
			if (i != l - 1)
				tmp[i].next = &tmp[i + 1];
		}
	}

	// deletes node in the linked-list
	int remove(vertex* adj)
	{
		if (this->head == NULL)
			return 0;
		node* crawler = this->head;
		node*prev = crawler;
		if (adj == crawler->present->getAddr())
		{
			if (crawler->next == NULL)
			{
				this->head = NULL;
				return 0;
			}
			else
			{
				this->head = crawler->next;
				crawler = crawler->next;
				prev->next = NULL;
			}
			this->size -= 1;
		}
		else
			crawler = crawler->next;
		int ctr = 1;
		while (crawler != NULL)
		{
			if (adj == crawler->present->getAddr())
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
			}
			else
			{
				prev = crawler;
				crawler = crawler->next;
			}
			ctr++;
		}
		return 0;
	}

	// returns the latest address of the vertex
	vertex* getAddr()
	{
		if (this->point == NULL)
			return this;

		vertex* crawler = this->point;
		while (crawler->point != NULL)
			crawler = crawler->point;

		return crawler;
	}

};

bool load(vector<vertex> &vertices, vector<edge> &edges, int n, std::fstream& mf)
{
	for (int i = 0; i < n; i++)
	{
		// get current line
		string linee;
		getline(mf, linee);
		std::stringstream linestream(linee);
		string tmpstr;

		// get label
		getline(linestream, tmpstr, ' ');
		vertices[i].label = stoi(tmpstr);
		
		// get size
		int l;
		getline(linestream, tmpstr, ' ');
		l = stoi(tmpstr);
		vertices[i].size = l;
		vertices[i].head = new node[l];
		if (vertices[i].head == NULL)
			return false;

		// stores all the adjacent vertices 
		//to be initialized on thE current vertex
		int tmp[l];
		
		for (int j = 0; j<l; j++)
		{
			/*cin>>tmp[j];*/
			getline(linestream, tmpstr, ' ');
			tmp[j] = stoi(tmpstr);
		}	
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
	while(crawler->next != NULL)
		crawler = crawler->next;
	crawler->next = y->head;

	// point the point field of r to point field of l
	y->head = x->head;
	y->point = x->getAddr();
	int new_size = y->size + x->size;
	y->size = new_size;
	x->size = new_size;

}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout<<"usage: ./mincut1 filename\n";
		return 1;
	}
	char* name = argv[1];

	// open the file
	fstream myfile(name);
	if(!myfile.is_open())
	{
		cout<<"Unable to open file\n";
		return 1;
	}

	int num;
	const double e = 2.71828;
	cin>>num;
	int loop = (num * num) * (int)log(num);
	int mincut = 0;
	int ne = 0;	// number of edges
	int nv = num;	// number of vertices
	cout<<"iterations: "<<loop<<endl;

	// timer start
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (int t = 0; t < num * 2; t++)
	{
		string line;
		// get the number of vertices
		myfile.clear();
		myfile.seekg(0, ios::beg);
		getline(myfile, line);
		int n = stoi(line);
		if (n == 2)
		{
			cout<<"Graph is already a mincut"<<endl;
			return 0;
		}

		std::vector<vertex> vertices(n);
		std::vector<edge> edges;
		edges.reserve(10 * n);

		if (!load(vertices, edges, n, myfile))
		{
			cout<<"Error\n";
			return 1;
		}
		ne = edges.size();
		// store are ll handles;
		node* handles[n];
		for (int i = 0; i < n; i++)
		{
			handles[i] = vertices[i].head;
		}
		/*Start of the Karger's Contraction Algorithm subroutine*/
		srand(time(NULL) * t);	// seed the pseudo-random generator
		for (int i = 0; i < n - 2; i++)
		{
			// choose a random edge
			int c = rand() % edges.size();
			vertex* l = edges[c].a->getAddr();
			vertex* r = edges[c].b->getAddr();

			// delete the nodes in the list that represents self-loop to the contracted vertices
			if (l->remove(r) == 1)
				return 1;
	
			if (r->remove(l) == 1)
				return 1;

			// append remaining nodes of vertex r to vertexl
			merge(l, r);

			// delete self-loop edges
			for (int p = 0, erased = 0, l = edges.size(); p < l; p++)
			{	
				vertex* x = edges[p - erased].a;
				vertex* y = edges[p - erased].b;

				if (x->getAddr() == y->getAddr())
				{
					edges.erase(edges.begin() + (p - erased));
					erased ++;
				}
			}
		}
		cout<<vertices[0].size<<endl;
		// get the number of remaining edges
		int re = edges.size();
		mincut = (re < mincut || mincut == 0)	?	re	: mincut;
		
		/*End of the Contraction Algorithm*/

	/*	for (int i = 0; i < n; i++)
			vertices[i].point->dump();
	*/
		// delete memory created on the heap
		for (int i = 0; i < n; i++)
		{
			delete[] handles[i];
		}
	}

	// timer stop
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();

	myfile.close();
	cout<<"vertices: "<<nv<<endl;
	cout<<"edges: "<<ne<<endl;
	cout<<"mincut: "<<mincut<<endl;
	cout<<"time: "<<duration<<endl;
	return 0;
}
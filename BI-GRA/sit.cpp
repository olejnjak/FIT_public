// BI-GRA second excercise
// Author: Jakub Olejnik <olejnjak@fit.cvut.cz>

#include <vector>
#include <iostream>
using namespace std;

#define VERT_NO_START 0
#define NO_SOLUTION()  { \
	delete g; \
	cout << "No solution." << endl; \
	return 0; \
}

struct Vertice {
	int opened;
	int closed;
	int depth;
	int min_depth;
	vector<int> successors;

public:
	Vertice(void) {
		opened = -1;
		closed = -1;
		depth = -1;
		min_depth = -1;
	}

	friend ostream& operator << (ostream& os, const Vertice& v) {
		os << "opened=" << v.opened << " closed=" << v.closed << " depth=" << v.depth << " min_depth=" << v.min_depth;

		if (v.successors.size()) os << endl << "\tsuccessors: ";

		for (unsigned i = 0; i < v.successors.size(); i++)
			os << v.successors[i]+VERT_NO_START << " ";

		return os;
	}
};

class Graph {
public:	
	Graph * dfs_g;

private:
	vector<Vertice> * vertices;
	void DFS (int v, int& stamp, int depth, int predecessor = -1) {
		//cout << "DFS("<<v<<","<<predecessor<<")" << endl;
		Vertice& vertice = vertices->at(v);

		if (predecessor != -1 && vertice.closed == -1)
			dfs_g->addEdge(predecessor,v);
		if (vertice.opened != -1) return;

		vertice.opened = ++stamp;
		vertice.depth = depth;
		vertice.min_depth = depth;

		dfs_g->vertices->at(v).min_depth = vertice.min_depth;
		dfs_g->vertices->at(v).depth = vertice.depth;

//		for (unsigned i = 0; i < vertice.successors.size(); i++) {
		for (vector<int>::iterator it = vertice.successors.begin(); it != vertice.successors.end(); it++) {
			//if (vertice.successors[i] != predecessor) 
			if (*it != predecessor)
				DFS(*it, stamp, depth+1, v);
			else continue;
		}

		Vertice& vertice2 = dfs_g->vertices->at(v);
		//for (unsigned i = 0; i < vertice2.successors.size(); i++) {
		for (vector<int>::iterator it = vertice2.successors.begin(); it != vertice2.successors.end(); it++) {
			//Vertice& succ = dfs_g->vertices->at(vertice2.successors.at(i));
			Vertice& succ = dfs_g->vertices->at(*it);
			if(succ.min_depth < vertice2.min_depth)
				vertice2.min_depth = succ.min_depth;
		}

		vertice.closed = ++stamp;
	}

	Graph (int v) {
		vertices = new vector<Vertice>(v);
		dfs_g = NULL;
	}

public:
	Graph (void) {
		dfs_g = NULL;
		vertices = new vector<Vertice>();
	}

	~Graph (void) {
		delete dfs_g;
		delete vertices;
	}

	bool bridgeExists (void) {
		//for (unsigned i = 0; i < dfs_g->vertices->size(); i++) {
		for (vector<Vertice>::iterator it = dfs_g->vertices->begin(); it != dfs_g->vertices->end(); it++) {
			//Vertice& v = dfs_g->vertices->at(i);
			Vertice& v = *it;

			if (v.depth == v.min_depth && v.depth != 0) {
				return true;
			}
		}

		return false;
	}

	void addVertice (void) {
		Vertice v;
		vertices->push_back(v);
	}

	void DFS(int start_vertice = 0) {
		int stamp = 0;
		delete dfs_g;
		dfs_g = new Graph(vertices->size());

		if (vertices->size()) DFS (start_vertice, stamp, 0);
	}

	void addEdge (int number_start, int number_end) {
		vertices->at(number_start).successors.push_back(number_end);
	}

	/*friend ostream& operator<< (ostream& os, const Graph& g) {
		//for (unsigned i = 0; i < g.vertices->size(); i++)
		for (vector<Vertice>::iterator it = g.vertices->begin(); it != g.vertices->end(); it++)
			os << i+VERT_NO_START << ": " << *it << endl;

		return os;
	}*/

	bool printDFS_G (ostream & os) {
		if (dfs_g == NULL || bridgeExists()) return false;

		for (vector<Vertice>::iterator it = dfs_g->vertices->begin(); it != dfs_g->vertices->end(); it++) {
			Vertice& v = *it;
			
			os << v.successors.size() << " ";

			for (vector<int>::iterator it2 = v.successors.begin(); it2 != v.successors.end(); it2++) {
			//for (unsigned j = 0; j < v.successors.size(); j++) {
				os << *it2 << " ";
			}

			os << endl;
		}

		return true;
	}
};


int main (int argc, char * argv[]) {
	Graph * g = new Graph();
	int N;

	cin >> N;

	for (int i = 0; i < N; i++) {
		int z_i;

		g->addVertice();

		cin >> z_i;

		if (!z_i) NO_SOLUTION()

		for (int j = 0; j < z_i; j++) {
			int end;

			cin >> end;

			g->addEdge(i, end);
		}
	}

	g->DFS();
	
	if (!g->printDFS_G(cout)) NO_SOLUTION();

	delete g;

	return 0;
}

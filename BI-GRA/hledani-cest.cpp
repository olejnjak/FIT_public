#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
using namespace std;

#define GRAPH_T vector<Node>
#define STATUS_T char
#define WAYS_T unsigned
#define SUCCESSORS_LIST_T list<int>
#define MOD 536870909

#define FRESH 0
#define OPENED 1
#define CLOSED 2

class Node {
    int way_length;
    WAYS_T way_count;
    STATUS_T status;
    SUCCESSORS_LIST_T successors;
    
public:
    Node (void) {
        way_length = -1;
        way_count = 0;
        status = FRESH;
    }
    
    bool isFresh(void) {
        return (status == FRESH);
    }
    
    void incWays(int length, int by = 1) {
        if (way_length == -1) {
            way_length = length;
            way_count = by;
        }
        else if (way_length == length) {
            way_count += by;
            way_count %= MOD;
        }
    }
    
    int getLength(void) {
        return way_length;
    }
    
    void addSuccessor (int successor) {
        successors.push_back(successor);
    }
    
    WAYS_T getMinWays (void) {
        return way_count;
    }
    
    SUCCESSORS_LIST_T& getSuccessors (void) {
        return successors;
    }
    
    void setStatus (STATUS_T status) {
        this->status = status;
    }
};

GRAPH_T* readGraph (int& start, int& end) {
    int n, m;
    GRAPH_T *graph;
    
    scanf("%d %d %d %d", &n, &m, &start, &end);
    graph = new GRAPH_T(n);
    
    for (int i = 0; i < n; i++) {
        int count;
        scanf("%d", &count);
        
        for (int j = 0; j < count; j++) {
            int successor;
            scanf ("%d", &successor);
            
            graph->at(i).addSuccessor(successor);
        }
    }
    
    return graph;
}

void enqueueSuccessors (GRAPH_T* graph, queue<int>& q, int node) {
    SUCCESSORS_LIST_T& successors = graph->at(node).getSuccessors();
    Node& served = graph->at(node);
    
    for(SUCCESSORS_LIST_T::iterator it = successors.begin(); it != successors.end(); it++) {
        Node& n = graph->at(*it);
        n.incWays(served.getLength()+1, served.getMinWays());
        //if (*it == 3) cout << " end " << endl;
        
        if (n.isFresh()) {
            n.setStatus(OPENED);
            q.push(*it);
        }
    }
}

void BFS (GRAPH_T* graph, int start) {
    queue<int> serve_queue;
    serve_queue.push(start);
    graph->at(start).setStatus(OPENED);
    graph->at(start).incWays(0, 1);
    
    while (!serve_queue.empty()) {
        int served = serve_queue.front(); serve_queue.pop();
        graph->at(served).setStatus(CLOSED);
        
        enqueueSuccessors(graph, serve_queue, served);
    }
}

int main (int argc, char * argv[]) {
    int start, end;
    GRAPH_T* graph = readGraph(start,end);
    BFS (graph, start);
    cout << graph->at(end).getMinWays() << endl;
    
    return EXIT_SUCCESS;
}
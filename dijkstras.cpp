#include <iostream>
#include <cstdlib>		// qsort
#include <cstring>		// NULL
#include <climits>		// INT_MAX
#include <vector>

using namespace std;

const int INFINITY = INT_MAX;	// Used for prims
const int NINFINITY = INT_MIN;	


// ****************************************************************
//
//			      Edge
//
// ****************************************************************



struct Node{
    int src;
    int dest;
    int distance;
    Node* next;
};



// ****************************************************************
//
//                            List
//
// ****************************************************************



struct List{
  int size;
  Node* head;  
};



// ****************************************************************
//
//                           Graph
//
// ****************************************************************



class Graph{
  public:
    int vertices;
    int edges;
  
    List* array;

    Node* add_edge(const int node, int src, int dest, int w){
      Node* newEdge = new Node;
      newEdge->src = src;
      newEdge->dest = dest;
      newEdge->distance = w;
      newEdge->next = NULL;

      return newEdge;
    }

    void add_node(const int node, int src, int dest, int w){
      Node* newEdge = add_edge(node, src, dest, w);
      
      newEdge->next = array[src].head;
      array[src].head = newEdge;
      
      newEdge = add_edge(node, dest, src, w);
      
      newEdge->next = array[dest].head;
      array[dest].head = newEdge;
    }
    

    int get_vertices(){ return vertices; }
    int get_edges() { return edges; }
};



// ****************************************************************



struct Graph* create_graph(int v, int e){
  Graph* graph = (Graph*)malloc(sizeof(Graph));

  graph->vertices = v;
  graph->edges = e;
  
  graph->array = (List*)malloc(graph->edges * sizeof(List));
  
  for(int i = 0; i < graph->vertices; i++){
    graph->array[i].head = NULL;
  }
  
  return graph;
}



// ****************************************************************
//
//                        Other Helpers
//
// ****************************************************************

// ****************************************************************



void Dijkstras(Graph* graph);

int GetMin(Graph g, int unvisited[5]);
void UpdateKeys(Graph g, int unvisited[5], int visited[5], int value);
bool IsAvailable(int unvisited[5], int value);



// ****************************************************************
//
//			     Main
//
// ****************************************************************



int main(){
  int numV = 5;
  int numE = 7;
  Graph *g = create_graph(numV, numE);

/*
       (2)
    5 /   \ 4
     /     \
   (1)--9--(3)
    |      /|
    |    /  |
    7   2   2
    |  /    |
    |/      |
   (0)--0--(4)
        

*/


  g->add_node(0, 0, 1, 7);
  g->add_node(1, 0, 3, 2);
  g->add_node(2, 0, 4, 1);
  g->add_node(3, 1, 2, 5);
  g->add_node(4, 1, 3, 9);
  g->add_node(5, 2, 3, 4);
  g->add_node(6, 3, 4, 2);
  
  Dijkstras(g);

  return 0;
}



// ****************************************************************
//
//			 Dijkstra's Algorithm
//
// ****************************************************************



// Dijkstra's Algorithm
// Pre: Graph is made and filled
// Post: Dijkstra's algorithm is implemented and the result is output
void Dijkstras(Graph* graph){
  Graph g = *graph;
  int V = g.vertices;
  int unvisited[V];
  int visited[V];		// Holds the visited nodes
  int lastNode = 0;		// Holds the integer value of the last node visited
  
  // Set up all unvisited nodes's distances to infinity &
  // Set each node's number and the distance to 0
  for(int i = 0; i<V; i++){
    unvisited[i] = INFINITY;
    visited[i] = 0;
  }
  
  // Pick starting node 0 and set it's key to 0
  unvisited[0] = 0;


  for(int i = 0; i < V; i++){  
    // Get Minimum
    lastNode = GetMin(g, unvisited);
    
    visited[lastNode] = unvisited[lastNode];

    // Update keys nearby
    UpdateKeys(g, unvisited, visited, lastNode);
    
    unvisited[lastNode] = NINFINITY;
  }


  cout << "Dijkstras:" << endl << endl;
  
  cout << " Node\tDistance From Source" << endl;
  for(int i = 0; i < V; i++){
    cout << "  " << i << "\t\t" << visited[i] << endl;
  }
  cout << endl;

}



// ****************************************************************
//
//                     Dijkstras Helper Functions
//
// ****************************************************************


// Finds the minimum key value in t and returns the node's number
// Pre: Graph is made and keys are updated
// Post: Minimum key is found in t. The parent's number is returned
int GetMin(Graph g, int unvisited[5]){
  int min = -4;
  int parent;
  int minVal = INFINITY;
  
  
  // Find the minimum value
  for(int i = 0; i < g.vertices; i++){
    if(unvisited[i] < minVal && IsAvailable(unvisited, i) == true){
      min = i;
      minVal = unvisited[i];
    }
  }
  
  return min;  
}



// ****************************************************************



// t is updated with the weights adjacent to node 'value'
// Pre: Graph is made and the list of available elements (q) is updated
// Post: Updates t to hold the weights of all adjacent elements to 'value'
void UpdateKeys(Graph g, int unvisited[5], int visited[5], int value){
  Node* temp = new Node;
  int nodeNeeded;
    
  temp = g.array[value].head;

  while(temp != NULL){
    // Find the source/destination
    nodeNeeded = temp->dest;

    // Check if a visisted node + adjacent node is a smaller distance than the current distance
    if(visited[value] + temp->distance < unvisited[nodeNeeded]){
      unvisited[nodeNeeded] = visited[value] + temp->distance;
    }

    temp = temp->next;
  }      
}



// ****************************************************************



// Checks if an element is available (to avoid a cycle)
// Pre: q is filled with elements available or set to NEGINFINITY if
//	not available
// Post: Returns whether the specified value is available
bool IsAvailable(int unvisited[5], int value){
  if(unvisited[value] != NINFINITY){
    return true;
  }
  else {
    return false;
  }
}

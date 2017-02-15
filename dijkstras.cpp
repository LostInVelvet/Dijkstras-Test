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



struct DijkstrasHelper{
  int myNum;
  int distance;
};



// ****************************************************************

// ****************************************************************



void Dijkstras(Graph* graph);

int GetMin(Graph g, DijkstrasHelper visited[5], int unvisited[5]);
void UpdateKeys(Graph g, int unvisited[5], DijkstrasHelper visited[5], int value);
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
  DijkstrasHelper visited[V];	// Holds the visited nodes
  int lastNode = 0;		// Holds the integer value of the last node visited
  
  // Set up all unvisited nodes's distances to infinity &
  // Set each node's number and the distance to 0
  for(int i = 0; i<V; i++){
    unvisited[i] = INFINITY;
    visited[i].myNum = i;
    visited[i].distance = 0;
  }
  
  // Pick starting node 0 and set it's key to 0
  unvisited[0] = 0;


  for(int i = 0; i < V-1; i++){  
    // Get Minimum
    lastNode = GetMin(g, visited, unvisited);
    
    visited[lastNode].distance = unvisited[lastNode];
    cout << endl << "Node: " << lastNode << endl;
    cout << "Unvisisted: " << unvisited[lastNode] << endl
         << "Dist: " << g.array[lastNode].head->distance << endl;
    cout << "Visited: " <<  visited[lastNode].distance << endl << endl;

    // Update keys nearby
    UpdateKeys(g, unvisited, visited, lastNode);
    
    unvisited[lastNode] = NINFINITY;
  }


  cout << "Dijkstras:" << endl << endl;
  
  for(int i = 0; i < V; i++){
    cout << visited[i].myNum << " - " << visited[i].distance << endl;
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
int GetMin(Graph g, DijkstrasHelper visited[5], int unvisited[5]){
  int min = -4;
  int parent;
  int minVal = NINFINITY;
  
  // Get Minimum
  for(int i = 0; i < g.vertices; i++){
    cout << "Checking: " << i << " - " << unvisited[i] << "\n\tCurrent Min: " << min << "\n\tMinVal: " << minVal << endl;
    if(unvisited[i] > minVal && IsAvailable(unvisited, i) == true){
      min = i;
      cout << "Min: " << min << endl;
      minVal = visited[i].distance;
      cout << "Updated to: " << minVal << endl << endl;
    }
  }
  return min;
}



// ****************************************************************



// t is updated with the weights adjacent to node 'value'
// Pre: Graph is made and the list of available elements (q) is updated
// Post: Updates t to hold the weights of all adjacent elements to 'value'
void UpdateKeys(Graph g, int unvisited[5], DijkstrasHelper visited[5], int value){
  Node* temp = new Node;
  int nodeNeeded;
    
  temp = g.array[value].head;

  while(temp != NULL){
    // Find the source/destination
    nodeNeeded = temp->dest;

    // Check if a visisted node + adjacent node is a smaller distance than the current distance
    if((visited[value].distance + temp->distance) < unvisited[nodeNeeded] && IsAvailable(unvisited, temp->dest)){
      // If so, update the key
      
/*      if(unvisited[nodeNeeded] = INFINITY){
      cout << "if " << value << " to " << nodeNeeded <<": " << unvisited[nodeNeeded] << " to " << temp->distance << endl;
        visited[nodeNeeded].distance = 0;
      }*/
      cout << "Updating " << nodeNeeded << " (" << unvisited[nodeNeeded] << ") to " << visited[value].distance << " + " << temp->distance << endl << endl;
      unvisited[nodeNeeded] = visited[value].distance+temp->distance;
      
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
//  cout << value <<" is available: ";
  if(unvisited[value] != NINFINITY){
//    cout << "true" << endl;
    return true;
  }
  else {
//    cout << "false" << endl;
    return false;
  }

}

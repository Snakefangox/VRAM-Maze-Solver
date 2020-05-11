#include <iostream>

#include <sstream>
#include <fstream>

#include <algorithm>
#include <vector>
#include <list>

#include <limits>
#include <math.h>

using namespace std;

class Node{
	public:
	//Node's position on the map
	int x;
	int y;
	
	float fCost; //Total path cost so far
	float gCost; //Cost of taking this node as route to end 
	
	bool isWall; //Is the node a wall
	bool visited; //Has the node been visisted
	
	vector<Node*> neighbors; //vector to store the neighbors 
	
	Node* previousNode = nullptr; //stores the previous node to track the route through the maze
};

float distance(Node* nodeOne, Node* nodeTwo){
	return abs((nodeOne->x - nodeTwo->x)) + abs((nodeOne->y - nodeTwo->y)); //Manhattan distance hueristic, most effient hueristic for grids with 4 directions of movement 
}

int main(){
	//Open the text files
	ifstream mazeInfo("maze_prop.txt");
	ifstream mazeInput("maze.txt");
	ofstream mazeOutput("out.txt", ios::out | ios::trunc);
	
	string line;
	string mazeInfoString = "";
	while(getline(mazeInfo, line)){ //Loop through each line of mazeInfo and convert text file into one continuous string 
		mazeInfoString += line + " "; 
	}
	
	string mazeInfoArr[9]; //Make array to store contents of the text file
	int mazeInfoindex = 0;
	istringstream iss(mazeInfoString);
	while(iss){ //Seperate the continous string into each word and store them in an array
		iss >> mazeInfoArr[mazeInfoindex]; //Shift the iss value into the mazeInfoArr at the current index position
		mazeInfoindex++; //Update the index position
	}
	
	//Assign the maze's size values by accessing the array which stores the mazeInfo, using the index position of cols and rows
	int cols = std::stoi(mazeInfoArr[1]);
	int rows = std::stoi(mazeInfoArr[2]);
	
	Node* maze[cols][rows]; //Make the 2d maze array with the size specified by the mazeInfo file
	
	//Assign the maze's start X, Y values by accessing the array which stores the mazeInfo, using the index position of startX and startY
	int startX = std::stoi(mazeInfoArr[4]);
	int startY = std::stoi(mazeInfoArr[5]);
	
	//Assign the maze's end X, Y values by accessing the array which stores the mazeInfo, using the index position of endX and endY
	int endX = std::stoi(mazeInfoArr[7]);
	int endY = std::stoi(mazeInfoArr[8]);	
	
	
	for(int y = 0; y < rows; y++){ //Loop through all the rows
		for(int x = 0; x < cols; x++){ //Loop through all the columns 
			int isWall = (int)mazeInput.get() - 48; //Convert char to int and subtract 48 (to convert to the proper int value)
			if(isWall == -38){ //If the tempNum is -38 this mean the character was a newline character and should not be put into the array
				isWall = (int)mazeInput.get() - 48; 
			}
			
			//Make a new node in the maze and assign values
			maze[x][y] = new Node(); 
			
			maze[x][y]->x = x;
			maze[x][y]->y = y;
			
			//To start f and g are infinity as when f and g are calculated they are going to be less than infinity so they replace these values
			maze[x][y]->fCost = std::numeric_limits<float>::infinity(); 
			maze[x][y]->gCost = std::numeric_limits<float>::infinity();
			
			maze[x][y]->isWall = isWall; //Is the noe a wall
			maze[x][y]->visited = false; //The node has not been visited yet
			
			//Node does not currently have a previousNode
			maze[x][y]->previousNode = nullptr;
		}
	}
	
	Node* startNode = maze[startX][startY];
	Node* endNode = maze[endX][endY];
	
	startNode->fCost = 0.0; //Total path cost so far is 0
	startNode->gCost = distance(startNode, endNode); //Minimum possible distance between the startNode and the endNode

	list<Node*> openSet;
	openSet.push_back(startNode); //Only the startNode is being evaluated so far
	Node* currentNode = startNode;

	bool pathFound = false; 
	int numOfNodes = 0;
	while(!openSet.empty() && currentNode != endNode){ //A* based off of OneLoneCoder's A* algorithm
		openSet.sort([](const Node* nodeOne, const Node* nodeTwo) { return nodeOne->fCost < nodeTwo->fCost; }); //Sort the open set by lowest f score
		
		while(!openSet.empty() && openSet.front()->visited){ //If the node is visited remove it from the open set
			openSet.pop_front();
		}
		
		if(openSet.empty()){ //If the open set is empty, there are no more nodes to look through
			break;
		}
		
		currentNode = openSet.front(); //Select the top node from the open set (node with the lowest f cost
		currentNode->visited = true; //Set that node to be visited
		numOfNodes++;
		
		if(currentNode == endNode){ //Check if the currentNode is the endNode, if it is a path has been found, the loop does not break here as there may be a better path still
			pathFound = true;
		}
		
		//Only calculate the neighbors of a node(not a wall) if it is required, making the program more efficient
		if(currentNode->x > 0){
			if(!maze[currentNode->x - 1][currentNode->y]->isWall){
				currentNode->neighbors.push_back(maze[currentNode->x - 1][currentNode->y]);
			}
		}
		if(currentNode->x < (cols - 1)){
			if(!maze[currentNode->x + 1][currentNode->y]->isWall){
				currentNode->neighbors.push_back(maze[currentNode->x + 1][currentNode->y]);
			}
		}
		if(currentNode->y > 0){
			if(!maze[currentNode->x][currentNode->y - 1]->isWall){
				currentNode->neighbors.push_back(maze[currentNode->x][currentNode->y - 1]);
			}
		}
		if(currentNode->y < (rows - 1)){
			if(!maze[currentNode->x][currentNode->y + 1]->isWall){
				currentNode->neighbors.push_back(maze[currentNode->x][currentNode->y + 1]);
			}
		}
		
		for(auto nodeNeighbor : currentNode->neighbors){ //Loop through all neighbors
			if(!nodeNeighbor->visited){ //If this neighbor is unvisited, add it to the openSet to be searched 
				openSet.push_back(nodeNeighbor);
			}
			
			float gCostNeighbor = currentNode->fCost + distance(currentNode, nodeNeighbor); //Calculate the g cost of going to this node
			
			if(gCostNeighbor < nodeNeighbor->gCost){ //Check if the g cost of this neighbour is lower than the already exisiting g cost of the neighbor
				nodeNeighbor->previousNode = currentNode; //Set the previous node of the neighbor to the current node
				nodeNeighbor->gCost = gCostNeighbor; //Update the g cost of this node
				
				nodeNeighbor->fCost = nodeNeighbor->gCost + distance(nodeNeighbor, endNode); //Update the total f cost so far
			}
		}
	}
	
	
	
	vector<Node*> path;
	int nodeDist = 1; //Total nodes covered in the path 
	path.push_back(endNode);
	Node* currentPathNode = endNode;
	if(pathFound){ //If there is a path recostruct the path
		while(currentPathNode->previousNode != nullptr){ //While the currentNode is not the startNode (therefore the path is complete)
			currentPathNode = currentPathNode->previousNode; //Get the previous node the the current node and set it as currentNode
			path.push_back(currentPathNode); //Add the current node to the path
			nodeDist++;
		}
		cout << "There were " << rows*cols << " nodes, optimal solution found by searching " << numOfNodes << " nodes with a distance of " << nodeDist << endl;
	}
	std::reverse(path.begin(), path.end()); //The path was reconstructed backwards so reverse the path
	
	if(!path.empty()){ //Check if the path is empty (empty means there is no path)
		for(auto node : path){ //Loop through the path
			mazeOutput << std::to_string(node->x) + ", " + std::to_string(node->y) + " \n"; //Output the x, y point of the node input mazeOutput file 
		}
	}
	else{ 
		cout << "All nodes were searched and no path was found" << endl;
		mazeOutput << "There is no path through the maze";
	}
}

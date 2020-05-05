#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <limits>

using namespace std;

float distance(float x1, float y1, float x2, float y2){
	return sqrt((x1-x2) * (x1-x2) + (y1 - y2) * (y1 - y2));
}

class Node{
	public:
	//Node's position on the map
	int x;
	int y;
	
	float f; //Total path cost so far
	float g; //Cost of taking this node as route to end 
	
	bool isWall; //Is the node a wall
	bool isVisited; //Has the node been visisted
	
	vector<Node*> neighbors; //vector to store the neighbors 
	
	Node* previousNode = nullptr; //stores the previous node to track the route through the maze
};

int main(){
	//Open the text files
	ifstream mazeInfo("maze_prop.txt");
	ifstream mazeInput("maze.txt");
	ofstream mazeOutput("solved_maze.txt");
	
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
			maze[x][y]->f = std::numeric_limits<float>::infinity(); 
			maze[x][y]->g = std::numeric_limits<float>::infinity();
			
			maze[x][y]->isWall = isWall; //Is the noe a wall
			maze[x][y]->isVisited = false; //The node has not been visited yet
			
			//Node does not currently have a previousNode
			maze[x][y]->previousNode = nullptr;
		}
	}
	
	//Setup node neighbours that are not walls		
	for(int x = 0; x < cols; x++){
		for(int y = 0; y < rows; y++){
			if(x > 0 && !maze[x][y]->isWall){
				maze[x][y]->neighbors.push_back(maze[y][x - 1]);
			}
			if( x < (cols - 1) && !maze[x][y]->isWall){
				maze[x][y]->neighbors.push_back(maze[y][x + 1]);
			}
			if(y > 0 && !maze[x][y]->isWall){
				maze[x][y]->neighbors.push_back(maze[y - 1][x]);
			}
			if( y < (rows - 1) && !maze[x][y]->isWall){
				maze[x][y]->neighbors.push_back(maze[y + 1][x]);
			}
		}
	}
	
	Node* startNode = maze[startX][startY];
	Node* endNode = maze[endX][endY];
	
	startNode->f = 0.0; //Total path cost so far is 0
	startNode->g = distance(startNode->x, startNode->y, endNode->x, endNode->y); //Minimum possible distance between the startNode and the endNode
}

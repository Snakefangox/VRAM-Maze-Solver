#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

class Node{
	public:
	int x;
	int y;
	
	float f;
	float g;
	
	bool isWall;
	bool isVisited;
	
	vector<Node> neighbours;
	
	Node* previousNode;
	
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
			
			//To start f and g are infinity
			maze[x][y]->f = std::numeric_limits<float>::infinity(); 
			maze[x][y]->g = std::numeric_limits<float>::infinity();
			
			maze[x][y]->isWall = isWall;
			maze[x][y]->isVisited = false;
			
			//Node does not currently have a previousNode
			maze[x][y]->previousNode = nullptr;
		}
	}
	
	Node* startNode = maze[startX][startY];
	Node* endNode = maze[endX][endY];
}

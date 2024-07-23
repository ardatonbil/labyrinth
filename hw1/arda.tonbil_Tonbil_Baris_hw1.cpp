#include <iostream>
#include "Stack.h"
#include <random>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct Cell {
    int right = 1;   //1 means there is a wall in the respective direction
    int left = 1;
    int up = 1;
    int down = 1;
    int x_coord, y_coord;  //cell should also know its own coordinates (will be useful)
    bool visited = false;
};

int getRandomNumber(int min, int max) {  //function to choose a random number from a range
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

int main() { 
    int K, M, N;     //K different mazes with M rows and N columns will be generated.
    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;

    Stack<Cell> visited_cells;   //Cells which are visited along the path
    int visited = 0;

    for (int i = 1; i <= K; i++) {
        vector<vector<Cell>> maze(M, vector<Cell>(N));
        while (visited < M * N) {

            if (visited == 0) {  //origin generation
                maze[0][0].x_coord = 0;
                maze[0][0].y_coord = 0;
                maze[0][0].visited = true;
                visited_cells.push(maze[0][0]);
                visited++;
            }
            else {
                Cell currentCell = visited_cells.top();

                vector<string> possible_paths = {};  
                //since it is generation phase, it is not necessary to check the walls of neighbours
                //they will already be destroyed


                //maze[row][column]
                

                //if a cell is already visited, it should not be visited again
                if (currentCell.y_coord + 1 < M && maze[currentCell.y_coord + 1][currentCell.x_coord].visited == false) {
                    possible_paths.push_back("up");
                }

                if (currentCell.y_coord - 1 >= 0 && maze[currentCell.y_coord - 1][currentCell.x_coord].visited == false) {
                    possible_paths.push_back("down");
                }

                if (currentCell.x_coord + 1 < N && maze[currentCell.y_coord][currentCell.x_coord + 1].visited == false) {
                    possible_paths.push_back("right");
                }

                if (currentCell.x_coord - 1 >= 0 && maze[currentCell.y_coord][currentCell.x_coord - 1].visited == false) {
                    possible_paths.push_back("left");
                }

                if (!possible_paths.empty()) {
                    int randomIndex = getRandomNumber(0, possible_paths.size() - 1);
                    string direction = possible_paths[randomIndex];  //one of the directions randomly selected

                    if (direction == "up") {
                        maze[currentCell.y_coord + 1][currentCell.x_coord].down = 0;  //opening up the walls of neighbouring cells
                        maze[currentCell.y_coord][currentCell.x_coord].up = 0;
                        maze[currentCell.y_coord + 1][currentCell.x_coord].visited = true;
                        maze[currentCell.y_coord + 1][currentCell.x_coord].x_coord = currentCell.x_coord;  
                        maze[currentCell.y_coord + 1][currentCell.x_coord].y_coord = currentCell.y_coord + 1;
                        visited_cells.push(maze[currentCell.y_coord + 1][currentCell.x_coord]);  //newest visited cell
                        visited++;
                    }

                    //same for the other directions

                    else if (direction == "down") {
                        maze[currentCell.y_coord - 1][currentCell.x_coord].up = 0;
                        maze[currentCell.y_coord][currentCell.x_coord].down = 0;
                        maze[currentCell.y_coord - 1][currentCell.x_coord].visited = true;
                        maze[currentCell.y_coord - 1][currentCell.x_coord].x_coord = currentCell.x_coord;
                        maze[currentCell.y_coord - 1][currentCell.x_coord].y_coord = currentCell.y_coord - 1;
                        visited_cells.push(maze[currentCell.y_coord - 1][currentCell.x_coord]);
                        visited++;
                    }

                    else if (direction == "right") {
                        maze[currentCell.y_coord][currentCell.x_coord + 1].left = 0;
                        maze[currentCell.y_coord][currentCell.x_coord].right = 0;
                        maze[currentCell.y_coord][currentCell.x_coord + 1].visited = true;
                        maze[currentCell.y_coord][currentCell.x_coord + 1].x_coord = currentCell.x_coord + 1;
                        maze[currentCell.y_coord][currentCell.x_coord + 1].y_coord = currentCell.y_coord;
                        visited_cells.push(maze[currentCell.y_coord][currentCell.x_coord + 1]);
                        visited++;
                    }

                    else if (direction == "left") {
                        maze[currentCell.y_coord][currentCell.x_coord - 1].right = 0;
                        maze[currentCell.y_coord][currentCell.x_coord].left = 0;
                        maze[currentCell.y_coord][currentCell.x_coord - 1].visited = true;
                        maze[currentCell.y_coord][currentCell.x_coord - 1].x_coord = currentCell.x_coord - 1;
                        maze[currentCell.y_coord][currentCell.x_coord - 1].y_coord = currentCell.y_coord;
                        visited_cells.push(maze[currentCell.y_coord][currentCell.x_coord - 1]);
                        visited++;
                    }
                }

                else { //if there is no available path remaining, it's time for backtracking
                    visited_cells.pop();
                }
            }
        }
        visited = 0;
        string filename = "maze_" + to_string(i) + ".txt";
        ofstream file(filename);

        if (file.is_open()) {   //writing into the maze_mazeID file
            file << M << " " << N << "\n";
            for (int row = 0; row < M; row++) {
                for (int column = 0; column < N; column++) {
                    file << "x=" << column << " y=" << row << " l=" << maze[row][column].left <<
                        " r=" << maze[row][column].right << " u=" << maze[row][column].up << " d=" << maze[row][column].down << "\n";
                }
            }
            //PLEASE CHECK THE DEBUG FOLDER FOR MAZE FILES
            //MAZE FILES SHOULD BE SAVED INTO THAT FOLDER
        }

        file.close();
        visited_cells.makeEmpty();  //will be used second time for path-finding, so it should be empty

    }
    int mazeID, entryX, entryY, exitX, exitY;

    cout << "All mazes are generated.\n";
    if (K > 1) {
        cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path : ";
        cin >> mazeID;
    }
    else { mazeID = 1; }
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entryX >> entryY;
    cout << "\nEnter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;

    string filename_selected = "maze_" + to_string(mazeID) + ".txt";
    ifstream maze_file(filename_selected);
    vector<vector<Cell>> maze(M, vector<Cell>(N));  
    //there are multiple mazes and one of them will be selected
    //so the selected one should be constructed again but this time reading from the file will do the job

    if (maze_file.is_open()) {
        string line;
        bool first_line = true;
        while (getline(maze_file, line)) {
            if (first_line == true) {
                first_line = false;  //I bypassed first line as it is used for dimensions of the maze
                continue;
            }
            else {
                int x_index, y_index, l_index, r_index, d_index, u_index, end_index;  
                //useful to parse the line, since the line format is known
                x_index = 0;
                y_index = line.find("y");
                l_index = line.find("l");
                r_index = line.find("r");
                d_index = line.find("d");
                u_index = line.find("u");
                end_index = line.find("\n");

                Cell new_cell_read;

                //I made some calculations and these arguments are spot-on for obtaining coordinates and wall values
                new_cell_read.x_coord = stoi(line.substr(x_index + 2, y_index - x_index - 3));  
                new_cell_read.y_coord = stoi(line.substr(y_index + 2, l_index - y_index - 3));
                new_cell_read.left = stoi(line.substr(l_index + 2, r_index - l_index - 3));
                new_cell_read.right = stoi(line.substr(r_index + 2, u_index - r_index - 3));
                new_cell_read.up = stoi(line.substr(u_index + 2, d_index - u_index - 3));
                new_cell_read.down = stoi(line.substr(d_index + 2, end_index - d_index - 3));

                maze[new_cell_read.y_coord][new_cell_read.x_coord] = new_cell_read;

                if (new_cell_read.x_coord == entryX && new_cell_read.y_coord == entryY) {  
                    //initial cell can be pushed into the stack without another loop
                    visited_cells.push(new_cell_read);
                }
            }
        }

        maze_file.close();

        while ((visited_cells.top().x_coord != exitX) || (visited_cells.top().y_coord != exitY)) { 
            //while top cell is not the exit cell
            Cell currentCell = visited_cells.top();

            vector<string> possible_paths = {};
            //checking if the neighbouring cell is valid in respective direction, walls are down in the path to neighbour
            //and the neighbour should not be visited already
            if (currentCell.y_coord + 1 < M && maze[currentCell.y_coord + 1][currentCell.x_coord].down == 0 
                && maze[currentCell.y_coord + 1][currentCell.x_coord].visited == false) {
               
                possible_paths.push_back("up");
            }

            //same for all directions
            if (currentCell.y_coord - 1 >= 0 && maze[currentCell.y_coord - 1][currentCell.x_coord].up == 0
                && maze[currentCell.y_coord - 1][currentCell.x_coord].visited == false) {
                possible_paths.push_back("down");
            }

            if (currentCell.x_coord + 1 < N && maze[currentCell.y_coord][currentCell.x_coord + 1].left == 0
                && maze[currentCell.y_coord][currentCell.x_coord + 1].visited == false) {
                possible_paths.push_back("right");
            }

            if (currentCell.x_coord - 1 >= 0 && maze[currentCell.y_coord][currentCell.x_coord - 1].right == 0
                && maze[currentCell.y_coord][currentCell.x_coord - 1].visited == false) {
                possible_paths.push_back("left");
            }





            if (!possible_paths.empty()) {  //if there are some available paths
                int randomIndex = getRandomNumber(0, possible_paths.size() - 1);
                string direction = possible_paths[randomIndex];  //choosing a random direction


                //neighbour is visited now and pushed to the stack in if/else if blocks
                if (direction == "up") {
                    
                    maze[currentCell.y_coord + 1][currentCell.x_coord].visited = true;  
                    visited_cells.push(maze[currentCell.y_coord + 1][currentCell.x_coord]);
                }

                else if (direction == "down") {
                    maze[currentCell.y_coord - 1][currentCell.x_coord].visited = true;
                    visited_cells.push(maze[currentCell.y_coord - 1][currentCell.x_coord]);
                }

                else if (direction == "right") {
                    maze[currentCell.y_coord][currentCell.x_coord + 1].visited = true;
                    visited_cells.push(maze[currentCell.y_coord][currentCell.x_coord + 1]);
                }

                else if (direction == "left") {
                    maze[currentCell.y_coord][currentCell.x_coord - 1].visited = true;
                    visited_cells.push(maze[currentCell.y_coord][currentCell.x_coord - 1]);
                }
            }

            else { //no available path, backtrack
                visited_cells.pop();
            }
        }

        string path_filename = "maze_" + to_string(mazeID) + "_path_" + to_string(entryX) + "_" +
            to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";

        ofstream path_file(path_filename);

        Stack<Cell> temp_stack;

        //I transferred elements from visited_cells to temp_stack to reverse the order
        while (!visited_cells.isEmpty()) {
            temp_stack.push(visited_cells.topAndPop());
        }

        //Now it's time to write the coordinates of cells from temp_stack to the path file
        while (!temp_stack.isEmpty()) {
            Cell top_cell = temp_stack.topAndPop();
            int x_coordinate = top_cell.x_coord;
            int y_coordinate = top_cell.y_coord;
            path_file << x_coordinate << " " << y_coordinate << "\n";
        }
    }
    else {
        cout << "Unable to open the maze file" << endl;
    }

    return 0;
}

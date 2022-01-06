#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <climits>

#define CALGARY "YYC"
#define CAN1 "YYZ"
#define CAN2 "YUL"
#define CAN3 "YVR"

using namespace std;

unordered_map<string, int> node_map;

bool containsKey(unordered_map<string, int> map, string str){
    return map.find(str) != map.end();
}

string getKey(unordered_map<string, int> map, int value) {
    for (auto it = map.cbegin(); it != map.cend(); ++it) {
        if ((*it).second == value) {
            return (*it).first;
        }
    }
    return "";
}

vector<string> split(string str, string delimiter){
    vector<string> list;
    size_t index = 0;
    string token;

    while ((index = str.find(delimiter)) != string::npos) {
        token = str.substr(0, index);
        list.push_back(token);
        str.erase(0, index + delimiter.length());
    }
    list.push_back(str);

    return list;
}

void printMatrix(int **matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
            cout << *(*(matrix + i) + j) << " ";
        }
		cout << endl;
	}
}

int** initMatrix(int rows, int cols) {
    int **matrix = (int**) malloc(rows*sizeof(int*));
    
    for (int i = 0; i < rows; i++) {
        matrix[i]= (int*) malloc(cols*sizeof(int));
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(*(matrix + i) + j) = 0;
        }
    }
    return matrix;
}

void printPathInfo(int paths[], int costs[], string src, string dest) {
    vector<int> path;
    int src_i = node_map[src];
    int dest_i = node_map[dest];
    int node_i = dest_i;
    int hops = 0;

    while (node_i != src_i) {
        path.insert(path.end(), node_i);
        hops++;
        node_i = paths[node_i];
    }
    path.insert(path.end(), node_i);

    cout << dest << "\t\t" << costs[dest_i] << "\t" << hops << "\t";
    for (int i = path.size()-1; i >= 1; i--) {
        cout << getKey(node_map, path.at(i)) << "->";
    }
    cout << getKey(node_map, path.at(0)) << endl;
}

int minCost(int costs[], bool visited[], int size) {
    int min_value = INT_MAX;
    int min_index;

    for (int i = 0; i < size; i++) {
        if (!visited[i] && costs[i] <= min_value) {
            min_value = costs[i];
            min_index = i;
        }
    }
    return min_index;
}

int* dijkstra(int **matrix, int costs[], int src, int size) {
    bool *visited = (bool*) malloc(size*sizeof(bool));
    int *paths = (int*)  malloc(size*sizeof(int));
    
    for (int i = 0; i < 21; i++) {
        costs[i] = INT_MAX;
        visited[i] = false;    
    }
    costs[src] = 0;
    paths[src] = src;
    
    for (int i = 0; i < size; i++) {
        int m = minCost(costs, visited, size); 
        visited[m] = true;

        for (int j = 0; j < size; j++) {
            if (!visited[j]
                && *(*(matrix + m) + j)
                && costs[m] != INT_MAX
                && costs[m] + *(*(matrix + m) + j) < costs[j]) {
                costs[j] = costs[m] + *(*(matrix + m) + j);
                paths[j] = m;
            }
        }
    }
    free(visited);
    
    return paths;
}

int initNodeMap(const char *file_name) {
    string line;
    ifstream file;
    file.open(file_name);
    
    if (!file.is_open()) {
        perror("open");
        exit(1);
    }
    vector<string> line_content;
    string n1_name, n2_name;
    int node_index = 0;
    
    while(getline(file, line)) {
        line_content = split(line, " ");

        n1_name = line_content.at(0);
        n2_name = line_content.at(1);

        if (!containsKey(node_map, n1_name)) {
            node_map[n1_name] = node_index++;
        }
        if (!containsKey(node_map, n2_name)) {
            node_map[n2_name] = node_index++;
        }
    }
    file.close();

    return node_index;
}

void fillAdjacencyMatrix(int **matrix, int size, const char *file_name) {
    string line;
    ifstream file;
    file.open(file_name);
    
    if (!file.is_open()) {
        perror("open");
        exit(1);
    }
    vector<string> line_content;
    int index1, index2, distance;
    
    while(getline(file, line)) {
        line_content = split(line, " ");

        index1 = node_map[line_content.at(0)];
        index2 = node_map[line_content.at(1)];
        distance = stoi(line_content.at(2));

        *(*(matrix + index1) + index2) = distance;
    }
    file.close();
}

int main(int argc, const char *argv[]) {
    int size = initNodeMap(argv[1]);
    int **matrix = initMatrix(size, size);

    fillAdjacencyMatrix(matrix, size, argv[1]);

    int* costs = (int*)  malloc(size*sizeof(int));
    int* paths = dijkstra(matrix, costs, node_map[CALGARY], size);

    cout << "Destination\tCost\tHops\tShortest Path" << endl;
    printPathInfo(paths, costs, CALGARY, CAN1);
    printPathInfo(paths, costs, CALGARY, CAN2);
    printPathInfo(paths, costs, CALGARY, CAN3);

    free(matrix);
    free(costs);
    free(paths);

    return 0;
}

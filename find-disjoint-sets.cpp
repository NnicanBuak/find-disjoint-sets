#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <queue>
#include <algorithm>

using namespace std;

// Чтение CSV файла и построение графа
unordered_map<int, unordered_set<int>> buildGraph(const string& filename) {
    unordered_map<int, unordered_set<int>> graph;
    ifstream file(filename);
    string line;
    getline(file, line); // Пропуск заголовка

    while (getline(file, line)) {
        stringstream ss(line);
        int src_id, dst_id;
        char comma;

        ss >> src_id >> comma >> dst_id;
        graph[src_id].insert(dst_id);
        graph[dst_id].insert(src_id); // Если граф неориентированный
    }

    return graph;
}

// Поиск компонент связности
void bfs(int start, const unordered_map<int, unordered_set<int>>& graph, unordered_set<int>& visited, vector<int>& component) {
    queue<int> q;
    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        component.push_back(node);

        for (int neighbor : graph.at(node)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
}

// Запись результата в CSV файл
void writeResult(const vector<vector<int>>& components, const string& filename) {
    ofstream file(filename);

    for (const auto& component : components) {
        for (size_t i = 0; i < component.size(); ++i) {
            if (i != 0) file << ',';
            file << component[i];
        }
        file << '\n';
    }
}

int main() {
    string inputFilename = "src_dst_1.csv";
    string outputFilename = "output.csv";

    // Построение графа
    unordered_map<int, unordered_set<int>> graph = buildGraph(inputFilename);

    // Поиск компонент связности
    unordered_set<int> visited;
    vector<vector<int>> components;

    for (const auto& pair : graph) {
        int node = pair.first;
        if (visited.find(node) == visited.end()) {
            vector<int> component;
            bfs(node, graph, visited, component);
            components.push_back(component);
        }
    }

    // Запись результата
    writeResult(components, outputFilename);

    // Вычисление дополнительной информации
    int numGroups = components.size();
    int maxGroupSize = 0;
    int totalFiles = 0;

    for (const auto& component : components) {
        int groupSize = component.size();
        maxGroupSize = max(maxGroupSize, groupSize);
        totalFiles += groupSize;
    }

    double avgGroupSize = static_cast<double>(totalFiles) / numGroups;

    cout << "Количество групп: " << numGroups << endl;
    cout << "Максимальное количество входящих файлов в группе: " << maxGroupSize << endl;
    cout << "Среднее количество файлов в группе: " << avgGroupSize << endl;

    return 0;
}

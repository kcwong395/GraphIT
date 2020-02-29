#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

string IntToString(int a)
{
	ostringstream temp;
	temp << a;
	return temp.str();
}

struct node {
	int id;
	vector<node> neighbor; // nodes connected to this node

	void addNeighbor(node d) {
		// break if the node is already a neighbor 
		for (int i = 0; i < neighbor.size(); i++) {
			if (d.id == neighbor[i].id) {
				break;
			}
		}
		this->neighbor.push_back(d);
	}

	void print() {
		cout << left << setw(20) << "Node ID" << setw(20) << IntToString(this->id) << '\n';
		cout << left << setw(20) << "Neighbor" << '\n';
		if (!this->neighbor.empty()) {
			for (int i = 0; i < this->neighbor.size(); i++) {
				cout << left << setw(20) << IntToString(i + 1) << setw(20) << IntToString(this->neighbor[i].id) << '\n';
			}
		}
		cout << endl;
	}

	bool compareTo(node n) { return this->id - n.id; }
};

struct edge {
	node src;
	node des;
	int cost;

	bool equal(bool directed, edge e) {
		if (directed) {
			return this->src.id == e.src.id && this->des.id == e.des.id;
		}
		return (this->src.id == e.src.id && this->des.id == e.des.id) || (this->src.id == e.des.id && this->des.id == e.src.id);
	}

	string toString() {
		return "(" + IntToString(this->src.id) + " -> " + IntToString(this->des.id) + ") : " + IntToString(this->cost) + '\n' + '\n';
	}
};

struct Graph {
	vector<node> nodeSet;
	vector<edge> edgeSet;
	bool directed;

	void addNode(node n);
	void addEdge(edge e);

	int** genIncMatrix();
	bool** genAdjMatrix();
	void dijkstra(int sp);
};

bool sortEdge(edge a, edge b) {
	if (a.src.id == b.src.id) {
		return a.des.id < b.des.id;
	}
	return a.src.id < b.src.id;
}

bool sorting(pair<node, pair<int, int>> a, pair<node, pair<int, int>> b) {
	return a.second.first < b.second.first;
}

void Graph::dijkstra(int sp) {

	cout << "Dijkstra's Algorithm starting from point: " << sp << endl << endl;

	map<int, pair<node, pair<int, int>>> result;
	map<int, pair<node, pair<int, int>>>::iterator it;
	vector<node> nArr = this->nodeSet;
	vector<edge> eArr = this->edgeSet;

	cout << "Initialization" << endl;
	for (int i = 0; i < nArr.size(); i++) {
		if (nArr[i].id != sp) {
			result[nArr[i].id] = { nArr[i], { INT_MAX / 2, -1 } };
		}
		else {
			result[nArr[i].id] = { nArr[i], { 0, -1 } };
		}
	}
	cout << left << setw(15) << "Node: ";
	for (it = result.begin(); it != result.end(); it++) {
		cout << setw(5) << it->first;
	}
	cout << endl << setw(15) << "Dist: ";
	for (it = result.begin(); it != result.end(); it++) {
		if (it->second.second.first == INT_MAX / 2) {
			cout << setw(5) << "\u221E";
		}
		else {
			cout << setw(5) << it->second.second.first << " ";
		}
	}
	cout << endl << setw(15) << "Prev: ";
	for (it = result.begin(); it != result.end(); it++) {
		if (it->second.second.second == -1) {
			cout << setw(5) << "Nil";
		}
		else {
			cout << setw(5) << it->second.second.second << " ";
		}
	}
	cout << endl << endl;

	cout << "Running the Algorithm" << endl;
	map<int, pair<node, pair<int, int>>> queue = result; // node, { dist, prev }
	while (!queue.empty()) {
		int idxToRemove = queue.begin()->first;
		for (it = queue.begin(); it != queue.end(); it++) {
			if (it->second.second.first < queue[idxToRemove].second.first) {
				idxToRemove = it->first;
			}
		}
		node u = queue[idxToRemove].first;
		int distU = queue[idxToRemove].second.first;
		queue.erase(idxToRemove);

		for (int i = 0; i < u.neighbor.size(); i++) {
			node v = u.neighbor[i];
			int edgeLength = -1;
			for (int j = 0; j < eArr.size(); j++) {
				if (eArr[j].src.compareTo(u) == 0 && eArr[j].des.compareTo(v) == 0) {
					edgeLength = eArr[j].cost;
				}
			}
			int mini = distU + edgeLength;
			if (queue.find(v.id) != queue.end() && mini < queue[v.id].second.first) {
				result[v.id].second = { mini, u.id };
				queue[v.id].second = { mini, u.id };
			}
		}

		cout << setw(15) << "Node: ";
		for (it = result.begin(); it != result.end(); it++) {
			cout << setw(5) << it->first;
		}
		cout << endl << setw(15) << "Dist: ";
		for (it = result.begin(); it != result.end(); it++) {
			if (it->second.second.first == INT_MAX / 2) {
				cout << setw(5) << "\u221E";
			}
			else {
				cout << setw(5) << it->second.second.first << " ";
			}
		}
		cout << endl << setw(15) << "Prev: ";
		for (it = result.begin(); it != result.end(); it++) {
			if (it->second.second.second == -1) {
				cout << setw(5) << "Nil";
			}
			else {
				cout << setw(5) << it->second.second.second << " ";
			}
		}
		cout << endl << endl;
	}

	cout << setw(15) << "Node: ";
	for (it = result.begin(); it != result.end(); it++) {
		cout << setw(5) << it->first;
	}
	cout << endl << setw(15) << "Dist: ";
	for (it = result.begin(); it != result.end(); it++) {
		if (it->second.second.first == INT_MAX / 2) {
			cout << setw(5) << "\u221E";
		}
		else {
			cout << setw(5) << it->second.second.first << " ";
		}
	}
	cout << endl << setw(15) << "Prev: ";
	for (it = result.begin(); it != result.end(); it++) {
		if (it->second.second.second == -1) {
			cout << setw(5) << "Nil";
		}
		else {
			cout << setw(5) << it->second.second.second << " ";
		}
	}
	cout << endl << endl;
}

bool** Graph::genAdjMatrix() {

	// initialization
	int n = this->nodeSet.size();
	bool** adjMatrix = new bool* [n];
	for (int i = 0; i < n; i++) {
		adjMatrix[i] = new bool[n];
		for (int j = 0; j < n; j++) {
			adjMatrix[i][j] = false;
		}
	}

	cout << left << setw(30) << "Adjacency Matrix: " << endl;
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			if (i == 0 && j == 0) {
				cout << left << setw(20) << "Node ID | Node ID";
			}
			else if (i == 0) {
				cout << setw(5) << this->nodeSet[j - 1].id;
			}
			else if (j == 0) {
				cout << left << setw(20) << this->nodeSet[i - 1].id;
			}
			else {
				for (int k = 0; k < this->nodeSet[i - 1].neighbor.size(); k++) {
					if (this->nodeSet[j - 1].id == this->nodeSet[i - 1].neighbor[k].id) {
						adjMatrix[i - 1][j - 1] = this->nodeSet[j - 1].id == this->nodeSet[i - 1].neighbor[k].id;
					}
				}
				cout << setw(5) << adjMatrix[i - 1][j - 1];
			}
		}
		cout << endl;
	}

	return adjMatrix;
}

int** Graph::genIncMatrix() {

	// initialization
	int m = this->nodeSet.size(), n = this->edgeSet.size();
	int** incMatrix = new int* [m];
	for (int i = 0; i < m; i++) {
		incMatrix[i] = new int[n];
		for (int j = 0; j < n; j++) {
			incMatrix[i][j] = 0;
		}
	}

	cout << left << setw(30) << "Incidence Matrix: " << endl;
	for (int i = 0; i < m + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			if (i == 0 && j == 0) {
				cout << setw(20) << "Node ID | Edge";
			}
			else if (i == 0) {
				string tmp = IntToString(this->edgeSet[j - 1].src.id) + "," + IntToString(this->edgeSet[j - 1].des.id);
				cout << setw(5) << tmp;
			}
			else if (j == 0) {
				cout << setw(20) << this->nodeSet[i - 1].id;
			}
			else {
				if (this->directed) {
					if (this->nodeSet[i - 1].id == this->edgeSet[j - 1].src.id) {
						incMatrix[i - 1][j - 1] = -1;
					}
					else if (this->nodeSet[i - 1].id == this->edgeSet[j - 1].des.id) {
						incMatrix[i - 1][j - 1] = 1;
					}
				}
				else {
					incMatrix[i - 1][j - 1] = this->nodeSet[i - 1].id == this->edgeSet[j - 1].src.id || this->nodeSet[i - 1].id == this->edgeSet[j - 1].des.id;
				}
				cout << setw(5) << incMatrix[i - 1][j - 1];
			}
		}
		cout << endl;
	}

	return incMatrix;
}

int main() {

	string input;
	cin >> input;
	if (input.length() != 16) return 0;

	int n = input.length();

	int* question = new int[n];
	for (int i = 0; i < n; i++) {
		question[i] = input[i] - '0';
	}
	cout << "The unique code is: " << input << endl << endl;

	Graph dG, uG;
	dG.directed = true, uG.directed = false;

	vector<node> nArr;
	for (int i = 0; i < 10; i++) {
		nArr.push_back({ i });
	}
	dG.nodeSet = nArr, uG.nodeSet = nArr;

	vector<edge> dgEArr, ugEArr;
	for (int i = 1; i < n; i++) {
		node src = nArr[question[i - 1]], des = nArr[question[i]];
		int cost = abs(src.id - des.id);
		edge tmp = { src, des, cost };
		// handle directed edge
		dG.nodeSet[question[i - 1]].addNeighbor(des);
		bool contain = false;
		for (int j = 0; j < dgEArr.size(); j++) {
			if (tmp.equal(dG.directed, dgEArr[j])) {
				contain = true;
			}
		}
		if (!contain) {
			dgEArr.push_back(tmp);
		}
		// handle undirected edge
		contain = false;
		uG.nodeSet[question[i - 1]].addNeighbor(des);
		uG.nodeSet[question[i]].addNeighbor(src);
		for (int j = 0; j < ugEArr.size(); j++) {
			if (tmp.equal(uG.directed, ugEArr[j])) {
				contain = true;
			}
		}
		if (!contain) {
			if (src.compareTo(des) <= 0) {
				ugEArr.push_back({ src, des, cost });
			}
			else {
				ugEArr.push_back({ des, src, cost });
			}
		}
	}
	sort(dgEArr.begin(), dgEArr.end(), sortEdge);
	sort(ugEArr.begin(), ugEArr.end(), sortEdge);
	dG.edgeSet = dgEArr, uG.edgeSet = ugEArr;

	cout << "Directed Graph: " << endl << endl;
	dG.dijkstra(0);
	dG.genAdjMatrix(); cout << endl;
	dG.genIncMatrix(); cout << endl;

	cout << "Undirected Graph: " << endl << endl;
	uG.genAdjMatrix(); cout << endl;
	uG.genIncMatrix(); cout << endl;

	cin >> n;
	return 0;
}
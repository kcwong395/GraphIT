#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <set>

using namespace std;

string IntToString(int a)
{
	ostringstream temp;
	temp << a;
	return temp.str();
}

struct node {
	int id;
	vector<pair<int, int>> neighbor; // nodes connected to this node and the cost

	void addNeighbor(int d, int c) {
		this->neighbor.push_back({ d, c });
	}

	void print() {
		cout << left << setw(20) << "Node ID | Neighbor" << setw(20) << IntToString(this->id) << '\n';
		cout << left << setw(20) << "(#, id, cost)" << '\n';
		if (!this->neighbor.empty()) {
			for (int i = 0; i < this->neighbor.size(); i++) {
				cout << left << setw(20) << IntToString(i + 1) << setw(20) << IntToString(this->neighbor[i].first) << " " << IntToString(this->neighbor[i].second) << endl;
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
	node getNode(int id);

	int** genIncMatrix();
	bool** genAdjMatrix();
	void dijkstra(int id);
	void mst(int id);
};

ostream& operator<<(ostream& os, const Graph& g)
{
	os << "This is a ";
	if (g.directed) os << "directed ";
	else os << "undirected ";
	os << "graph." << endl;
	os << "It contains " << g.nodeSet.size() << " nodes" << endl;
	for (int i = 0; i < g.nodeSet.size(); i++) {
		node tmp = g.nodeSet[i];
		tmp.print();
	}
	return os;
}

node Graph::getNode(int id) {
	node tmp;
	for (int i = 0; i < this->nodeSet.size(); i++) {
		if (nodeSet[i].id == id) {
			tmp = nodeSet[i];
			break;
		}
	}
	return tmp;
}

bool compareNeighbor(pair<int, int> a, pair<int, int> b) {
	return a.first > b.first;
}

void Graph::addNode(node n) {
	// sort neighbor first
	sort(n.neighbor.begin(), n.neighbor.end(), compareNeighbor);
	
	// find the insertion point where the node will be added in front of e
	int s = -1, e = this->nodeSet.size();
	while (s < e - 1) {
		int mid = (s + e) / 2;
		this->nodeSet[mid].id > n.id ? e = mid : s = mid;
	}
	this->nodeSet.insert(this->nodeSet.begin() + e, n);
}

bool sortEdge(edge a, edge b) {
	if (a.src.id == b.src.id) {
		return a.des.id < b.des.id;
	}
	return a.src.id < b.src.id;
}

bool sorting(pair<node, pair<int, int>> a, pair<node, pair<int, int>> b) {
	return a.second.first < b.second.first;
}

// id donate the id of the starting node
void Graph::dijkstra(int id) {

	cout << "Dijkstra's Algorithm starting from node: " << id << endl << endl;

	map<int, pair<node, pair<int, int>>> result;			// id, { node, { cost, prev } }
	map<int, pair<node, pair<int, int>>>::iterator it;
	vector<node> nArr = this->nodeSet;
	vector<int> visitedSet;

	cout << "Initialization:" << endl << endl;
	for (int i = 0; i < nArr.size(); i++) {
		if (nArr[i].id != id) {
			result[nArr[i].id] = { nArr[i], { INT_MAX / 2, -1 } };
		}
		else {
			result[nArr[i].id] = { nArr[i], { 0, -1 } };
		}
	}
	cout << left << setw(15) << "Node: ";
	for (it = result.begin(); it != result.end(); it++) {
		cout << setw(7) << it->first;
	}
	cout << endl << setw(15) << "Dist: ";
	for (it = result.begin(); it != result.end(); it++) {
		if (it->second.second.first == INT_MAX / 2) {
			cout << setw(7) << "Inf";
		}
		else {
			cout << setw(7) << it->second.second.first;
		}
	}
	cout << endl << setw(15) << "Prev: ";
	for (it = result.begin(); it != result.end(); it++) {
		if (it->second.second.second == -1) {
			cout << setw(7) << "Nil";
		}
		else {
			cout << setw(7) << it->second.second.second;
		}
	}
	cout << endl;
	cout << setw(15) << "Vistied Nodes: { }";
	cout << endl << endl;

	cout << "Running the Algorithm" << endl << endl;
	map<int, pair<node, pair<int, int>>> queue = result; // node, { dist, prev }
	int step = 1;
	while (!queue.empty()) {

		// extract index of node with minimum distance
		int idxToRemove = queue.begin()->first;
		for (it = queue.begin(); it != queue.end(); it++) {
			if (it->second.second.first < queue[idxToRemove].second.first) {
				idxToRemove = it->first;
			}
		}
		node u = queue[idxToRemove].first;
		int distU = queue[idxToRemove].second.first;
		visitedSet.push_back(idxToRemove);
		queue.erase(idxToRemove);
		// end extract

		for (int i = 0; i < u.neighbor.size(); i++) {
			pair<int, int> v = u.neighbor[i];
			int edgeLength = v.second;
			int mini = distU + edgeLength;

			// if the neighbor is still in the queue and relaxation is required
			if (queue.find(v.first) != queue.end() && mini < queue[v.first].second.first) {
				result[v.first].second = { mini, u.id };
				queue[v.first].second = { mini, u.id };
			}
		}

		cout << "Step " << IntToString(step) << ": " << endl;
		step++;
		cout << setw(15) << "Node: ";
		for (it = result.begin(); it != result.end(); it++) {
			cout << setw(7) << it->first;
		}
		cout << endl;
		cout << setw(15) << "Dist: ";
		for (it = result.begin(); it != result.end(); it++) {
			if (it->second.second.first == INT_MAX / 2) {
				cout << setw(7) << "Inf";
			}
			else {
				cout << setw(7) << it->second.second.first;
			}
		}
		cout << endl;
		cout << setw(15) << "Prev: ";
		for (it = result.begin(); it != result.end(); it++) {
			if (it->second.second.second == -1) {
				cout << setw(7) << "Nil";
			}
			else {
				cout << setw(7) << it->second.second.second;
			}
		}
		cout << endl;
		cout << setw(15) << "Visited nodes: { ";
		for (int i = 0; i < visitedSet.size(); i++) {
			cout << visitedSet[i] << " ";
		}
		cout << "}" << endl << endl;
	}

	cout << "Backtracking: " << endl;
	for (int i = 0; i < nArr.size(); i++) {
		if (result[nArr[i].id].second.second == -1 && nArr[i].id != id) {
			cout << "The path from node " << IntToString(id) << " to " << IntToString(nArr[i].id) << " does not exist as the node is unreachable." << endl;
		}
		else {
			cout << "The path from node " << IntToString(id) << " to " << IntToString(nArr[i].id) << " is: ";
			int j = nArr[i].id;
			while (j != id) {
				cout << IntToString(j) << " <- ";
				j = result[j].second.second;
			}
			cout << IntToString(id) << " with a cost of " << result[nArr[i].id].second.first << "." << endl;
		}
	}
	cout << endl;
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
					if (this->nodeSet[j - 1].id == this->nodeSet[i - 1].neighbor[k].first) {
						adjMatrix[i - 1][j - 1] = this->nodeSet[j - 1].id == this->nodeSet[i - 1].neighbor[k].first;
					}
				}
				cout << setw(5) << adjMatrix[i - 1][j - 1];
			}
		}
		cout << endl;
	}
	cout << endl;

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

void Graph::mst(int id) {

	cout << "Prim's Algorithm starting from node: " << id << endl << endl;
	
	set<int> connected;
	connected.insert(id); // start with any node in the graph
	cout << "Start from Node: " << id << endl;
	int origin = id;

	while (connected.size() != this->nodeSet.size()) { // end the program when all nodes are considered
		// assert: all edge value must be lower than INT_MAX
		// find the minimum edge to connect a node outside the current mst
		pair<int, int> mini = { INT_MAX, INT_MAX };
		for (set<int>::iterator it = connected.begin(); it != connected.end(); ++it) {
			node curNode = this->getNode(*it);
			for (int i = 0; i < curNode.neighbor.size(); i++) { 
				// ensure no cycle is produced, if the node is connected, do not consider it
				set<int>::iterator it2 = connected.find(curNode.neighbor[i].first);
				if (it2 == connected.end()) { // if the node is not connected
					if (curNode.neighbor[i].second < mini.second) {
						mini = curNode.neighbor[i];
						origin = curNode.id;
					}
				}
			}
		}
		// no valid solution found
		if (mini.first == INT_MAX) {
			cout << "No edge available found" << endl;
			return;
		}
		if (origin > mini.first) {
			cout << "Add edge: (" << origin << ", " << mini.first << ") with a cost of " << mini.second << endl;
		}
		else {
			cout << "Add edge: (" << mini.first << ", " << origin << ") with a cost of " << mini.second << endl;
		}
		connected.insert(mini.first);
	}

	cout << endl;
}


void moderator(Graph g) {
	int opt;
	while (true) {
		cout << "Choose an operation: -1: end, 0: print graph info, 1: adjMatrix, 2: incMatrix, 3: Dijk_Algor, 4: MST" << endl;
		std::cin >> opt;
		if (opt == -1) break;

		int id = 0;
		switch (opt) {
		case 0:
			cout << g;
			break;
		case 1:
			g.genAdjMatrix();
			break;
		case 2:
			g.genIncMatrix();
			break;
		case 3:
			cout << "Input id of the starting node: ";
			std::cin >> id;
			cout << endl;
			g.dijkstra(id);
			break;
		case 4:
			cout << "Input id of the starting node: ";
			std::cin >> id;
			cout << endl;
			g.mst(id);
			break;
		}
	}
}

int main() {
	
	char mode;
	cout << "Please enter input mode: u for unique code, otherwise would be self-defined graph: " << endl;
	std::cin >> mode;

	if (mode == 'u') {
		string input;
		std::cin >> input;
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
			dG.nodeSet[question[i - 1]].addNeighbor(des.id, cost);
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
			uG.nodeSet[question[i - 1]].addNeighbor(des.id, cost);
			uG.nodeSet[question[i]].addNeighbor(src.id, cost);
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
		dG.mst(0); cout << endl;

		cout << "Undirected Graph: " << endl << endl;
		dG.dijkstra(0);
		uG.genAdjMatrix(); cout << endl;
		uG.genIncMatrix(); cout << endl;
		uG.mst(0); cout << endl;

	}
	else {
		Graph g;		
		int src_id, des_id, cost, directed;
		cout << "0 for directed graph, otherwise undirected" << endl;
		std::cin >> directed;
		g.directed = directed == 0;
		while (true) {
			cout << "Input src_id (-1 donates the end)" << endl;
			std::cin >> src_id;
			if (src_id == -1) break;
			node tmp;
			tmp.id = src_id;
			cout << "input des_id, cost, (-1 -1 donates the end)" << endl;
			while (true) {
				std::cin >> des_id >> cost;
				if (des_id == -1 && cost == -1) {
					g.addNode(tmp);
					cout << endl;
					break;
				}
				else {
					tmp.addNeighbor(des_id, cost);
				}
			}
		}
		cout << endl;

		// operation
		moderator(g);
	}

	std::cin >> mode;
	return 0;
}
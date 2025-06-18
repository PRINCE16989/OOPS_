/*
    Problem Statement : https://docs.google.com/document/d/1HTkkBcOyunG4DIktCcfNwYGLV0g4ejwBiJvKyXjLtMc/edit?usp=sharing
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
using namespace std;

template<typename T,typename comparator>
class min_heap
{
    private:
        vector<T> heap;
        comparator comp;
        int Size;

        void perculateUp(int x) {
            if(x >= Size) return;
            T val = heap[x];

            while(x >= 1 && comp(heap[(x-1)/2], heap[x])) {
                heap[x] = heap[(x-1)/2];
                x = (x-1)/2;
            }
            heap[x] = val;
        }

        void perculateDown(int x) {
            if(empty()) return;
            while(true) {
                int left = 2*x + 1;
                int right = 2*x + 2;
                int smallest = x;
                if(left < Size && comp(heap[smallest], heap[left])) smallest = left;
                if(right < Size && comp(heap[smallest], heap[right])) smallest = right;
                if(smallest == x) break;
                
                swap(heap[smallest],heap[x]);
                x = smallest;
            }    
        }

    public:
        min_heap(int size = 0) { heap.resize(size); Size = size; }
        void push(T &a) {
            if(Size == heap.size()) 
                heap.resize(2*Size +1);

            heap[Size] = a;
            ++Size;
            perculateUp(Size-1);
        }

        void pop() {
            if(empty()) return;

            heap[0] = heap[Size-1];
            --Size;
            perculateDown(0);
        }

        const T& top() {
            return heap[0];
        }

        const bool empty() {
            return Size == 0;
        }

        const size_t size() {
            return Size;
        }
};

class Node
{
    private:
        string id;
        int t;
    public:
        map<string,int> edge;
        Node(string ID, int time = __INT_MAX__) : id(ID), t(time) {}

        //getters
        string getId() { return id; }
        int getDistanceFromS() { return t; }

        //setters
        void setDistanceFromS(int x) { t = x; }

        void addEdge(string s, int time) {
            edge[s] = time;
        }

        bool operator>(Node & other) {
            return t > other.t;
        }

        bool operator<(Node & other) {
            return t < other.t;
        }
};

struct comparator {
    bool operator()(Node* a, Node* b) {
        return a->getDistanceFromS() > b->getDistanceFromS();
    } 
};

void dijkstra(string source, map<string,Node*> &rooms) {
    map<string,bool> inA;
    min_heap<Node*, comparator> A;

    for(auto x : rooms) inA[x.first] = false;

    A.push(rooms[source]);
    inA[source] = true;
    rooms[source]->setDistanceFromS(0);

    while(!A.empty()) {
        Node* curr = A.top(); A.pop();
        int t = curr->getDistanceFromS();
        for(auto x : curr->edge) {
            if(t + x.second < rooms[x.first]->getDistanceFromS()) {
                rooms[x.first]->setDistanceFromS(t + x.second);
                A.push(rooms[x.first]);
            }
        }
    }
}


int main()
{
    int n, m;
    cin >> n >> m;

    map<string,Node*> rooms, new_rooms;
    for(int i=0; i<n; ++i) {
        string room_id;
        cin >> room_id;
        rooms[room_id] = new Node(room_id);
    }

    for(int i=0; i<m; ++i) {    
        string a,b;
        int time;
        cin >> a >> b >> time;
        rooms[a]->addEdge(b,time);
        rooms[b]->addEdge(a,time);
    }

    string source, dest;
    cin >> source >> dest;

    for(auto x : rooms) {
        new_rooms[x.first + 'e'] = new Node(x.first + 'e');
        new_rooms[x.first + 'o'] = new Node(x.first + 'o');
    }

    for(auto x : rooms) {
        for(auto y : x.second->edge) {
            new_rooms[x.first + 'e']->addEdge(y.first + 'o',y.second);
            new_rooms[x.first + 'o']->addEdge(y.first + 'e',y.second);
        }
    }

    dijkstra(source + 'e',new_rooms);
    int ans = new_rooms[dest + 'e']->getDistanceFromS();
    cout << (ans == __INT_MAX__ ? -1 : ans) << endl;
}
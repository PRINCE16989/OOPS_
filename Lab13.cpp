/*
    Problem Statement : https://drive.google.com/file/d/1lz2JzqtOTFYPc6yoYe9yjYCU143Gvnds/view?usp=sharing
*/

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
using namespace std;

class sentinel
{
    private:
        string name;
        int id, rank;
    public:
        sentinel(string Name, int Id, int Rank) : name(Name), id(Id), rank(Rank) {}
        virtual int countHigherRanked()=0;
        int getRank() { return rank; }
        int getId() { return id; }
};

class Senapati : public sentinel
{
    public:
        Senapati(string name, int id) : sentinel(name, id, 1) {}
        int countHigherRanked() {
            return 0;
        }
};

class Dandanayaka : public sentinel
{
    public:
        Dandanayaka(string name, int id) : sentinel(name, id, 2) {}
        int countHigherRanked() {
            return 0;
        }
};

class Chaturangini : public sentinel
{
    public:
        Chaturangini(string name, int id) : sentinel(name, id, 3) {}
        int countHigherRanked() {
            return 0;
        }
};

class comparator {
    public:
        bool operator()(sentinel* a, sentinel* b) {
            if(a->getRank() == b->getRank()) 
                return a->getId() < b->getId();
            return a->getRank() < b->getRank();
        }
};

template<typename T>
class Kingdom
{
    private:
        int vertex_cover;
        vector< vector<int> > roads;
        vector< vector<int> > vc;
        vector<T*> sentinels;
        vector<T*> sorted_sentinels;
        int no_of_senapati, no_of_dandanayaka;

        void computeStates(int i, int parent) {
            if(vc[i][0] != -1 && vc[i][1] != -1) return;

            int included = 1, excluded = 0;
            for(auto x : roads[i]) {
                if(x == parent) continue;
                computeStates(x,i);
                excluded += vc[x][1];
                included += min(vc[x][1], vc[x][0]);
            }
            
            vc[i][0] = excluded;
            vc[i][1] = included;
        }

    public:
        Kingdom(int n) {
            vc.resize(n, vector<int>(2,-1));
            roads.resize(n);
            vertex_cover = 0;
            no_of_dandanayaka = 0;
            no_of_senapati = 0;
        }

        void addRoad(int u, int v) {
            roads[u].push_back(v);
            roads[v].push_back(u);
        }

        void addSentinel(T* s) {
            sentinels.push_back(s);
        }

        void increment_senapati() { ++ no_of_senapati; }
        void increment_dandanayaka() { ++no_of_dandanayaka; }

        void query1() {
            if(vertex_cover == 0) {
                computeStates(0,-1);
                vertex_cover = min(vc[0][1], vc[0][0]);
            }
            
            cout << vertex_cover << '\n';
        }

        void query2() {
            if(sorted_sentinels.empty()) {
                sorted_sentinels = sentinels;
                sort(sorted_sentinels.begin(),sorted_sentinels.end(),comparator());
            }
            
            for(auto x : sorted_sentinels) {
                    cout << x->getId() << " ";
                }
                cout << '\n';
            return;
        }

        void query3(int id) {
            int r = sentinels[id]->getRank();
            if(r == 1) cout << 0 << '\n';
            else if(r == 2) cout << no_of_senapati << '\n';
            else cout << no_of_dandanayaka + no_of_senapati << '\n';
        }

    friend istream& operator>>(istream& s, Kingdom<sentinel>& K);
};

istream& operator>>(istream& s, Kingdom<sentinel>& K) {
    
    return s;
}

int main() {
    int n, u, v;
    cin >> n;
    Kingdom<sentinel> K(n);

    for(int i=0; i<n-1; ++i) {
        cin >> u >> v;
        K.addRoad(u,v);
    }
    string name, rank;
    for(int i=0; i<n; ++i) {
        cin >> name >> rank;
        if(rank == "SENAPATI") {
            K.addSentinel(new Senapati(name,i));
            K.increment_senapati();
        }
        else if(rank == "DANDANAYAKA") {
            K.addSentinel(new Dandanayaka(name,i));
            K.increment_dandanayaka();
        }
        else if(rank == "CHATURANGINI") {
            K.addSentinel(new Chaturangini(name,i));
        }
    }

    int q, query;
    cin >> q;
    while(q--) {
        cin >> query;
        if(query == 1) K.query1();
        else if(query == 2) K.query2();
        else if(query == 3) {
            int id;
            cin >> id;
            K.query3(id);
        }
    }   
}
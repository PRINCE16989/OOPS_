/*
    Problem Statement : https://docs.google.com/document/d/1I6uZdAEQRjhYX8tAiNKr9z6r3dsk1M9MW2izsFs3ePo/edit?usp=sharing
*/

#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Graph
{
    public:
        //constructors
        Graph(int n) {
            matrix.resize(n);
        }
        Graph() {}

        void add_Edge(int u,int v) {
            matrix[u].insert(v);
            matrix[v].insert(u);
        }

        void remove_edge(int u,int v) {
            matrix[u].erase(v);
            matrix[v].erase(u);
        }
        
        
        Graph& operator+(Graph& other) {
            int n1 = matrix.size(), n2 = other.matrix.size();
            if(n1 < n2) matrix.resize(n2);
            
            for(int i=0; i< n2; ++i) {
                matrix[i].insert(other.matrix[i].begin(),other.matrix[i].end());
            }
            return *this;
        }

        Graph& operator-(Graph& other) {
            int n1 = matrix.size(), n2 = other.matrix.size();
            if(n1 < n2) matrix.resize(n2);
            
            for(int i=0; i<n2; ++i) {
                if(matrix[i] == other.matrix[i]) continue;
                vector<int> toRemove;
                
                for(auto x : matrix[i]) {
                    if(other.matrix[i].find(x) == other.matrix[i].end()) toRemove.push_back(x);
                }
                
                for(auto x : toRemove)
                    matrix[i].erase(x);
            }

            for(int i=n2; i<n1; ++i) {
                    matrix[i].clear();
            }
            return *this;
        }

        Graph& operator!() {
            int n = matrix.size();
            vector<pair<int,int>> toadd,toremove;
            
            for(int i=0; i<n; ++i) {
                for(int j=i+1; j<n; ++j) {
                    if(matrix[i].find(j) == matrix[i].end()) toadd.push_back({i,j});
                    else toremove.push_back({i,j});
                }
            }
            
            for(auto& p : toremove) remove_edge(p.first,p.second);
            for(auto& p : toadd) add_Edge(p.first,p.second);

            return *this;
        }

        bool is_reachable(int v1,int v2) {
            reached.resize(matrix.size());
            reached.assign(matrix.size(),false);
            return search(v1,v2);
        }

        bool search(int v1,int v2) {
            reached[v1] = true;
            if(v1 == v2) return true;
            for(auto x : matrix[v1]) {
                if(!reached[x] && search(x,v2)) return true;
            }
            return false;
        }

    private:
        vector<set<int>> matrix; 
        vector<bool> reached;
   
    friend istream& operator>>(istream& stream, Graph &g);
    friend ostream& operator<<(ostream& stream, Graph &g);
};

istream& operator>>(istream& stream, Graph &g) {
    string s;
    cin >> s;
    int n,m;
    stream >> n >> m;
   
    g.matrix.resize(n);
    for(int i=0; i<m; ++i) {
        int v1,v2;
        stream >> v1 >> v2;
        g.add_Edge(v1,v2);
    }
   
    return stream;
}

ostream& operator<<(ostream& stream, Graph &g) {
    int n = g.matrix.size();
    
    for(int i=0; i<n; ++i) {
        stream << "Vertex "  << i << ": ";
        for(auto x : g.matrix[i]) {
            stream << x << " ";
        }
        stream << endl;
    }
   
    return stream;
}

int main()
{
    Graph G;
    cin >> G;
    string s=" ";
    while(s != "end") {
        cin >> s;
        if(s == "union") {
            Graph G2;
            cin >> G2;
            G + G2;
        }
        else if(s == "complement") {
            !G;
        }
        else if(s == "intersection") {
            Graph G2;
            cin >> G2;
            G - G2;
        }
        else if(s == "isReachable") {
            int v1,v2;
            cin >> v1 >> v2;
            cout << (G.is_reachable(v1,v2) ? "Yes" : "No") << endl;
        }
        else if(s == "printGraph") {
            cout << G;
        }
        else if(s == "remove_edge") {
            int v1,v2;
            cin >> v1 >> v2;
            G.remove_edge(v1,v2);
        }
        else if(s == "add_edge") {
            int v1,v2;
            cin >> v1 >> v2;
            G.add_Edge(v1,v2);
        }
    }
}
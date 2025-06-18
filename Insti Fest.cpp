/*
    Problem Statement : https://docs.google.com/document/d/1usTyI23L_WgEglSfcRCOM21Xeopm_Q3lj18ZQMH0H8w/edit?usp=sharing
*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#define MIN_HEAP priority_queue<int,vector<int>,greater<int>>
#define GRAPH vector<vector<int>>
using namespace std;

class GraphAlgorithm
{
    public:
        vector<vector<int>> graph;
        vector<int> hype;
        GraphAlgorithm(vector<vector<int>> &g, vector<int> &h) {
            graph = g; hype = h;
        }
        virtual void query()=0;
};

class isCycle : public GraphAlgorithm
{
    private:
        bool has_cycle = false;
    public:
        isCycle(vector<vector<int>> &g, vector<int> &h) : GraphAlgorithm(g,h) {
            DFS();
        }

        void DFS() {
            int n = graph.size();
            vector<int> visited(n,-1);
            for(int i=1; i<n; ++i) {
                if(has_cycle) return;
                if(visited[i] == -1) DFS_visit(i,visited);
            }
        }

        void DFS_visit(int vertex, vector<int> &visited) {
            visited[vertex] = 0;
            for(auto x : graph[vertex]) {
                if(visited[x] == 0) { has_cycle = true; return; }
                else if(visited[x] == -1) DFS_visit(x,visited);
            }
            visited[vertex] = 1;
        }

        void query() {
            cout << (has_cycle ? "YES" : "NO") << endl;
        }
};

class indepComponent : public GraphAlgorithm
{
    private:
        int no_of_SCC;
        int cardinality;
    public:
        indepComponent(vector<vector<int>> &g, vector<int> &h) : GraphAlgorithm(g,h) {
            int n = g.size();
            stack<int> top_sort;
            
            vector<int> visited(n,-1);
            for(int i=1; i<n; ++i) {
                if(visited[i] == -1)
                    DFS_visit(i,visited,top_sort);
            }
            visited[0]=-1;
            GRAPH gt(n);
            for(int i=1; i<n; ++i) {
                visited[i] = -1;
                for(auto x : g[i]) {
                    gt[x].push_back(i);
                }
            }

            vector<int> component_no(n);
            int count = 0;
            while(!top_sort.empty()) {
                int vertex = top_sort.top(); top_sort.pop();
                if(visited[vertex] == -1) {
                    ++count;
                    DFS_visit(gt, vertex, visited, component_no, count);
                }
            }

            no_of_SCC = count;
            vector<int> max_car(count+1,0);
            for(int i=1; i<n; ++i) {
                max_car[component_no[i]]++;
            }

            int ans = max_car[1];
            for(int i=1; i<=count; ++i) {
                ans = max(ans,max_car[i]);
            }
            
            cardinality = ans;
        }
        
        void DFS_visit(GRAPH &g, int vertex, vector<int> &visited, vector<int> &component_no, int count) {
            visited[vertex] = 0;
            component_no[vertex] = count;
            for(auto x : g[vertex]) {
                if(visited[x] == -1) DFS_visit(g,x,visited,component_no,count);
            }
            visited[vertex] = 1;
        }

        void DFS_visit(int vertex, vector<int> &visited, stack<int> &top_sort) {
            visited[vertex] = 0;
            for(auto x : graph[vertex]) {
                if(visited[x] == -1) 
                DFS_visit(x,visited,top_sort);
            }
            visited[vertex] = 1;
            top_sort.push(vertex);
        }

        void query() {
            cout << no_of_SCC << " " << cardinality << endl;
        }   
};

class validOrder : public GraphAlgorithm
{
    private:
        vector<int> top_sort;
        bool has_cycle = false;

        void print_top_sort() {
            for(auto x : top_sort) {
                    cout << x << " ";
            }
            cout << endl;
        }

    public:
        validOrder(vector<vector<int>> &g, vector<int> &h) : GraphAlgorithm(g,h) {
            int n = g.size();
            vector<int> inDegree(n,0);
            for(int i=1; i<n; ++i) {
                for(auto x : g[i]) {
                    inDegree[x]++;
                }
            }
            MIN_HEAP pq;
            sol(inDegree,pq);

            if(top_sort.size() != n-1) has_cycle = true;
        }

        void sol(vector<int> &inDegree,MIN_HEAP &pq) {
            for(int i=1; i<inDegree.size(); ++i) {
                if(inDegree[i] == 0) pq.push(i);
            }

            while (!pq.empty()) {
                int source = pq.top(); pq.pop();
                top_sort.push_back(source);
                for(auto x : graph[source]) {
                    inDegree[x]--;
                    if(!inDegree[x]) pq.push(x);
                }
            }
        }

        void query() {
            if(has_cycle) cout << "NO" << endl;
            else print_top_sort();
        }
};

class maxHype : public GraphAlgorithm
{
    private:
        int max_hype;
    public:
        maxHype(vector<vector<int>> &g, vector<int> &h) : GraphAlgorithm(g,h) {
            int n = g.size();
            stack<int> top_sort;
            
            vector<int> visited(n,-1);
            for(int i=1; i<n; ++i) {
                if(visited[i] == -1)
                    DFS_visit(i,visited,top_sort);
            }

            visited[0]=-1;
            GRAPH gt(n);
            for(int i=1; i<n; ++i) {
                visited[i] = -1;
                for(auto x : g[i]) {
                    gt[x].push_back(i);
                }
            }

            vector<int> component_no(n);
            int count = 0;
            while(!top_sort.empty()) {
                int vertex = top_sort.top(); top_sort.pop();
                if(visited[vertex] == -1) {
                    ++count;
                    DFS_visit(gt, vertex, visited, component_no, count);
                }
            }

            vector<int> scc_hypes(count+1,0);
            vector<vector<int>> gscc(count+1);
            for(int i=1; i<n; ++i) {
                scc_hypes[component_no[i]] += h[i]; 
                for(auto x : g[i]) {
                    if(component_no[i] != component_no[x]) {
                        gscc[component_no[i]].push_back(component_no[x]);
                    }
                }
            }

            vector<int> indegree(count+1,0);
            for(int i=1; i<=count; ++i) {
                for(auto x : gscc[i]) {
                    indegree[x]++;
                }
            }

            vector<int> new_hypes(count+1,0);
            queue<int> q;
            for(int i=1; i<=count; ++i) {
                if(indegree[i] == 0) {
                    new_hypes[i] = scc_hypes[i];
                    q.push(i);
                }
            }
            
            while(!q.empty()) {
                int vertex = q.front(); q.pop();
                for(auto x : gscc[vertex]) {
                    new_hypes[x] = max(new_hypes[x], scc_hypes[x] + new_hypes[vertex]);
                    indegree[x]--;
                    if(indegree[x] == 0) q.push(x);
                }
            }
            
            max_hype=new_hypes[1];
            for(int i=1; i<=count; ++i) {
                max_hype = max(max_hype, new_hypes[i]);
            }
        }

        void DFS_visit(GRAPH &g, int vertex, vector<int> &visited, vector<int> &component_no, int count) {
            visited[vertex] = 0;
            component_no[vertex] = count;
            for(auto x : g[vertex]) {
                if(visited[x] == -1) DFS_visit(g,x,visited,component_no,count);
            }
            visited[vertex] = 1;
        }

        void DFS_visit(int vertex, vector<int> &visited, stack<int> &top_sort) {
            visited[vertex] = 0;
            for(auto x : graph[vertex]) {
                if(visited[x] == -1) 
                DFS_visit(x,visited,top_sort);
            }
            visited[vertex] = 1;
            top_sort.push(vertex);
        }

        void query() {
            cout << max_hype << endl;
        }
};

int main()
{
    int n,m;
    cin >> n >> m;

    vector<int> hype(n+1);
    vector<vector<int>> graph(n+1);
    
    for(int i=1; i<=n; ++i) cin >> hype[i];  
    for(int i=1; i<=m; ++i) {
        int u, v;
        cin >> u >> v;

        graph[u].push_back(v);
    }

    GraphAlgorithm* Has_Cycle_query_1 = new isCycle(graph,hype);
    GraphAlgorithm* SSCs_query_2 = new indepComponent(graph,hype);
    GraphAlgorithm* topSort_query_3 = new validOrder(graph,hype);
    GraphAlgorithm* MaxHype_query_4 = new maxHype(graph,hype);

    int Q,query;
    cin >> Q;
    while(Q--) {
        cin >> query;

        if(query == 1) {
            Has_Cycle_query_1->query();
        }
        else if(query == 2) {
            SSCs_query_2->query();
        }
        else if(query == 3) {
            topSort_query_3->query();
        }
        else if(query == 4) {
           MaxHype_query_4->query();
        }
    }
}
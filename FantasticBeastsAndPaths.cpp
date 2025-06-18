/*
    Problem Statement : https://drive.google.com/file/d/1j8TEmeSVSkWdpU5Ky2fQSjitAuLt27Ek/view?usp=sharing
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <stack>
#include <bitset>
using namespace std;
typedef pair<int,int> Vertex;
typedef pair<int,int> Centre;

class Island 
{
    protected:
        string ID;
        Centre COI; // centre of island
        double max_dist;

        virtual void modify_data(istream& s)=0;
        virtual void display_data(ostream& s)=0;
    public:
        Island(string id = "") : ID(id) {}

        string getID() { return ID; }
        double getMaxDIst() { return max_dist; }
        double DistFromI(const Island& A) {
            int xdiff = COI.first - A.COI.first;
            int ydiff = COI.second - A.COI.second;
            return sqrt(xdiff*xdiff + ydiff*ydiff);
        }

    friend istream& operator>>(istream& s, Island &A) {
        A.modify_data(s);
        return s;
    }

    friend ostream& operator<<(ostream& s, Island &A) {
        A.display_data(s);
        return s;
    }
};

class Rectangle : public Island {
    private:
        Vertex v[4];
    public:
        Rectangle() {}

        void modify_data(istream& s) {
            s >> ID >> v[0].first >> v[0].second;
            s >> v[1].first >> v[1].second;
            s >> v[2].first >> v[2].second;
            s >> v[3].first >> v[3].second;

            COI.first = (v[0].first + v[2].first) / 2;
            COI.second = (v[0].second + v[2].second) / 2;

            int xdiff = v[0].first - COI.first;
            int ydiff = v[0].second - COI.second;
            max_dist = sqrt(xdiff*xdiff + ydiff*ydiff);
        }

        void display_data(ostream& s) {
            s << ID << " " << v[0].first << " " << v[0].second << " ";
            s << v[1].first << " " << v[1].second;
            s << v[2].first << " " << v[2].second;
            s << v[3].first << " " << v[3].second;
        }
};


class Triangle : public Island {
    private:
        Vertex v[3];
    public:
        Triangle() {}

        void modify_data(istream& s) {
            s >> ID >> v[0].first >> v[0].second;
            s >> v[1].first >> v[1].second;
            s >> v[2].first >> v[2].second;

            COI.first = (v[0].first + v[1].first + v[2].first) / 3;
            COI.second = (v[0].second + v[1].second + v[2].second) / 3;

            int xdiff = v[0].first - COI.first;
            int ydiff = v[0].second - COI.second;
            double r1 = sqrt(xdiff*xdiff + ydiff*ydiff);
            
            xdiff = v[1].first - COI.first;
            ydiff = v[1].second - COI.second;
            double r2 = sqrt(xdiff*xdiff + ydiff*ydiff);
            
            xdiff = v[2].first - COI.first;
            ydiff = v[2].second - COI.second;
            double r3 = sqrt(xdiff*xdiff + ydiff*ydiff);
            
            r1 = max(r1, r2);
            max_dist = max(r1, r3);
        }

        void display_data(ostream& s) {
            s << ID << " " << v[0].first << " " << v[0].second << " ";
            s << v[1].first << " " << v[1].second << " ";
            s << v[2].first << " " << v[2].second << endl;
        }
};

class Circle : public Island {
    private:
        Centre c;
        int radius;
    public:
        Circle() {}

        void modify_data(istream& s) {
            s >> ID >> c.first >> c.second >> radius;

            COI = c;
            max_dist = radius;
        }

        void display_data(ostream& s) {
            s << ID << " " << c.first << " " << c.second << " " << radius << endl;
        }
};

class Sea {
    private:
        vector<Island*> Islands;
        vector<vector<int>> graph;
        int N;

        bool InVicinity(Island* A, Island* B) {
            return A->DistFromI(*B) <= A->getMaxDIst() + B->getMaxDIst();
        }

        void Print_graph() {
            int i=0;
            for(auto x : graph) {
                cout << i << " : ";
                for(auto y : x) cout << y << " ";
                cout << endl;
                ++i;
            }
        }
    public:
        Sea(vector<Island*> I) : Islands(I) {
            N = Islands.size();
            graph.resize(N);

            for(int i = 0; i<N; ++i) {
                for(int j = 0; j<N; ++j) {
                    if(i!=j && InVicinity(Islands[i],Islands[j])) {
                        graph[i].push_back(j);
                    }
                }
            }
        }

        vector<Island*> max_path() {
            // Print_graph();
            // cout << "inside max_path" << endl;
            vector<vector<int>> DP(1 << N,vector<int>(N,-1));

            for(int i = 0; i < N; ++i) {
                DP[1 << i][i] = i;
            }

            int m = 0, end, prev, len = 0;
            for(int mask = 1; mask < (1 << N); ++mask) {
                for(int n = 0; n < N; ++n) {
                    if(DP[mask][n] != -1) {
                        for(auto x : graph[n]) {
                            if((mask & (1 << x)) == 0)
                                DP[mask | (1 << x)][x] = n;
                        }
                    }
                    if(DP[mask][n] != -1 && bitset<32>(mask).count() > len) {
                        m = mask;
                        end = n;
                        prev = DP[mask][n];
                        len = bitset<32>(mask).count();
                    } 
                }
            }
            // cout << "finished dp" << endl;
            int mask = m;
            // cout << "found largest path" << endl;

            vector<Island*> ans;
            ans.push_back(Islands[end]);
            while(end != prev) {
                int temp = prev;
                mask = mask ^ (1 << end);
                prev = DP[mask][temp];
                end = temp;
                ans.push_back(Islands[end]);
            }

            return ans;
        }        
};  

int main() {
    int n;
    cin >> n;

    vector<Island*> Islands(n);
    for(int i=0; i<n; ++i) {
        string shape;
        cin >> shape;

        if(shape == "RECTANGLE") {
            Islands[i] = new Rectangle();
        }
        else if(shape == "TRIANGLE") {
            Islands[i] = new Triangle();
        }
        else Islands[i] = new Circle();

        cin >> *Islands[i];
    }
    Sea S(Islands);
    vector<Island*> IslandsInMaxPath = S.max_path();
    
    cout << (IslandsInMaxPath.size() == n ? "YES" : "NO") << endl;
    if(IslandsInMaxPath.size() != n) cout << IslandsInMaxPath.size() << endl;

    for(auto I : IslandsInMaxPath) {
        cout << I->getID() << " ";
    }
    cout << endl;
}
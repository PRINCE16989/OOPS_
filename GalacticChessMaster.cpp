/*
    Problem Statement : https://docs.google.com/document/d/1xCYa_vIygDbf3yYgPmlVBBrx3M_VJkrSFkCTQ4ETM6I/edit?usp=sharing
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#define int long long int
#define pair_points pair<pair<int,int>,pair<int,int>>
using namespace std;

class sorter
{
    private:
        int order;

        void merge_sort(vector<int> &arr,int low,int high) {
            if(low == high) return;
            int mid = (low + high)/2;

            merge_sort(arr,low,mid);
            merge_sort(arr,mid+1,high);

            vector<int> temp;
            int i=low,j = mid+1;
            int max, min;
            while(i <= mid && j <= high) {
                if(arr[i] > arr[j]) {
                    if(order == 1) { temp.push_back(arr[j]); ++j; }
                    else { temp.push_back(arr[i]); ++i; }
                }
                else if(arr[i] <= arr[j]){
                    if(order == 1) { temp.push_back(arr[i]); ++i; }
                    else { temp.push_back(arr[j]); ++j; }
                }
            }
            while(i <= mid) temp.push_back(arr[i++]);
            while(j <= high) temp.push_back(arr[j++]); 

            for(int i=low; i<=high; ++i) {
                arr[i] = temp[i-low];
            }
        }
    public:
        sorter(int x) : order(x) {}
        void operator()(vector<int> &arr) {
            int n = arr.size();
            merge_sort(arr,0,n-1);
        }
};

class Board
{
    private:
        vector<vector<int>> matrix;

        int countInversions(vector<vector<int>> &arr) {
            vector<int> temp;
            int n = arr.size();
            for(int i=0; i<n; ++i) {
                for(int j=0; j<n; ++j) {
                    temp.push_back(arr[i][j]);
                }
            }
            int high = n*n-1;
            return countInversions(temp, 0, high);
        }

        int countInversions(vector<int> &arr,int low, int high) {
            if(low >= high) return 0;
            int mid = (low + high)/2;
            
            // inversions in left subarray and right subarray
            int inversions = countInversions(arr, low, mid) + countInversions(arr, mid+1, high);
            
            // count cross inversions i.e, inverions involving elements from left and right subarray while merge sorting the array
            vector<int> temp;
            int i=low,j=mid+1;
            while(i <= mid && j <= high) {
                if(arr[i] > arr[j]) {
                    temp.push_back(arr[j]);
                    inversions += mid - i + 1; // increment no.of inversions
                    ++j;
                }
                else if(arr[i] <= arr[j]){
                    temp.push_back(arr[i]);
                    ++i;
                }
            }
            // complete the merging of arrays
            while(i <= mid) temp.push_back(arr[i++]);
            while(j <= high) temp.push_back(arr[j++]); 
            
            // assign the sorted array to the original array
            for(int i=low; i<=high; ++i) {
                arr[i] = temp[i-low];
            }
            // cout << "inverions : " << inversions << endl;
            return inversions;
        }

    public:
        void operator()(int n) {
            matrix.resize(n);
            int p;
            for(int i=0; i<n; ++i) {
                for(int j=0; j<n; ++j) {
                    cin >> p;
                    matrix[i].push_back(p);
                }
            }
        }

        void display() {
            int n = matrix.size();
            for(int i=0; i<n; ++i) {
                for(int j =0; j<n; ++j) {
                    cout << matrix[i][j] << " ";
                }
                cout << endl;
            }
        }

        void sortRows(int y) {
            // y indicates ascending for 1 and descending for 0
            for_each(matrix.begin(),matrix.end(),sorter(y));
        }

        int countInversions() {
            return countInversions(matrix);
        }
};

class Closest_points 
{
    private:
        vector<pair<int,int>> P;
        map<pair<int,int>,int> index;
        pair_points ans;

        int sq_distance(pair_points P) {
            int x = P.first.first - P.second.first;
            int y = P.first.second - P.second.second;

            return x*x + y*y;
        }

        pair_points Closest_Pair(vector<pair<int,int>> &P) {  //P indicates x sorted array of points
            int n = P.size();
            
            
            //basecases
            if (n == 2) { 
                return  (index[P[0]] < index[P[1]] ? make_pair(P[0],P[1]) : make_pair(P[1],P[0]));
            }

            if(n == 3) {
                pair_points ans = (index[P[0]] < index[P[1]] ? make_pair(P[0],P[1]) : make_pair(P[1],P[0]));
                int del = sq_distance({P[0],P[1]});
                if(sq_distance({P[0],P[2]}) < del) {
                    ans = (index[P[0]] < index[P[2]] ? make_pair(P[0],P[2]) : make_pair(P[2],P[0]));
                    del = sq_distance(ans);
                }
                else if(sq_distance({P[0],P[2]}) == del) {
                    if(index[P[0]] > index[P[2]]) swap(P[0],P[2]);
                    ans = better_pair({P[0],P[2]},ans);
                    swap(P[0],P[2]);
                }

                if(sq_distance({P[1],P[2]}) < del) {
                    ans = (index[P[1]] < index[P[2]] ? make_pair(P[1],P[2]) : make_pair(P[2],P[1]));
                    del = sq_distance(ans);
                }
                else if(sq_distance({P[1],P[2]}) == del) {
                    if(index[P[1]] > index[P[2]]) swap(P[1],P[2]);
                    ans = better_pair({P[1],P[2]},ans);
                }
                return ans;
            }
            vector<pair<int,int>> P_left(P.begin(),P.begin()+n/2);
            vector<pair<int,int>> P_right(P.begin()+n/2,P.end());
            
            int L = P_left.back().first;    // max x coordinate in left subarray

            pair_points Qp = Closest_Pair(P_left);      
            pair_points Rp = Closest_Pair(P_right);
            
            int del = min(sq_distance(Qp),sq_distance(Rp));
            pair_points ans = (sq_distance(Qp) <= sq_distance(Rp)) ? Qp : Rp;
            
            vector<pair<int,int>> S;
            for(int i=0; i<n; ++i) {
                // push elements which are at a distance of atmost sqrt(del) from L
                int d = abs(P[i].first - L);
                if(d*d < del) 
                    S.push_back(P[i]);
            }

            //sort S using Y coordinate
            sort(S.begin(),S.end(),[](pair<int,int> &a, pair<int,int> &b) {
                if(a.second != b.second) return a.second < b.second;
                return a.second < b.second;
            });

            for(int i=0; i<S.size(); ++i) {     
                int j = i+1;
                while(j < S.size() && j-i < 16) {
                    if(sq_distance({S[i],S[j]}) < del) {
                        ans = (index[S[i]] < index[S[j]] ? make_pair(S[i],S[j]) : make_pair(S[j],S[i]));
                        del = sq_distance(ans);
                    }
                    else if(sq_distance({S[i],S[j]}) == del) {
                        if(index[S[i]] > index[S[j]]) ans = better_pair({S[j],S[i]},ans);
                        else ans = better_pair({S[i],S[j]},ans);
                    }
                    ++j;
                } 
            }
            return ans;
            
        }

        pair_points better_pair(pair_points a, pair_points b) {
            if(a.first.first < b.first.first) { return a; }
            else if(a.first.first > b.first.first) { return b; }
            else if(a.first.second < b.first.second) { return a; }
            else if(a.first.second > b.first.second) { return b; }
            else if(a.second.first < b.second.first) { return a; }
            else if(a.second.first > b.second.first) { return b; }
            else if(a.second.second < a.second.second) { return a; }
            else return b;
        }

    public:
        Closest_points(int n) {
            P.resize(n);

            for(int i=0; i<n; ++i) {
                cin >> P[i].first >> P[i].second;
                index[P[i]] = i;
            }
            sort(P.begin(),P.end(),[](pair<int,int>& a, pair<int,int>&b) {
                return a.first < b.first;
            });
            
            ans = Closest_Pair(P);
            cout << ans.first.first << " " << ans.first.second << " ";
            cout << ans.second.first << " " << ans.second.second << endl;
        } 
};

int32_t main()
{
    Board GChess;
    string s;
    int n;
    cin >> s;
    
    while(s != "END") {
        if(s == "CREATE_2D") {
            cin >> n;
            GChess(n);
        }
        else if(s == "SORT_2D") {
            cin >> s;
            if(s == "ascending") {
                GChess.sortRows(1);
            }
            else if(s == "descending") {
                GChess.sortRows(0);
            }
        }
        else if(s == "INVERSION_2D") {
            cout << GChess.countInversions() << endl;
        }
        else if(s == "DISPLAY_2D") {
            GChess.display();
        }
        else if(s == "CLOSEST_2D") {
            cin >> n;
            Closest_points CP(n);
        }
        cin >> s;

    }
}
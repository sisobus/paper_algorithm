#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <map>
#include <set>
using namespace std;

#ifndef false
#define false 0
#endif
#ifndef true
#define true !(false)
#endif
#define sz(a) (int)a.size()

//#define HasDataSet 

typedef vector<char>            DataVector;
typedef pair<int,int>           ii;
typedef pair<int,DataVector>    iDV;
default_random_engine generator;
auto pqCompareFunction = [](const ii& e1,const ii& e2) {return e1.second<e2.second;};
typedef priority_queue<ii,vector<ii>,decltype(pqCompareFunction)> PQ;

void printDataVector(DataVector& d) {
    for ( char &c: d ) {
        printf("%c",c);
    }
    puts("");
}
void printVantagePoints(vector<DataVector>& vps) {
    for ( DataVector &vp: vps ) {
        printDataVector(vp);
    }
}
DataVector generateRandomPoint(int N,int A=10) {
    DataVector ret;
    uniform_int_distribution<int> distribution(0,A-1);
    for ( int i = 0 ; i < N ; i++ ) {
        ret.push_back('A'+distribution(generator));
    }
    return ret;
}
PQ initPriorityQueue(int N) {
    PQ pq(pqCompareFunction);
    for ( int i = 0 ; i < N ; i++ ) {
        pq.push(ii(i,0));
    }
    return pq;
}
vector<iDV> generateCandidateSet(DataVector& vp,int N,int lVl,int K,PQ& pq) {
    vector<iDV> newCandidateSet;
    set<int> dist;
    while ( sz(dist) < N*min(K,lVl)/lVl ) {

    }
    return newCandidateSet;
}
vector<DataVector> generateVantagePoints(int N,int M,int K,int A=10) {
    vector<DataVector> V;
    map<int,int> dist;
    vector<iDV> candidateSet;
    PQ pq = initPriorityQueue(N);
#ifdef HasDataSet
#else
    V.push_back(generateRandomPoint(N,A));
#endif

    while ( (int)V.size() < M ) {
        for ( DataVector &vp: V ) {
            candidateSet.push_back(generateCandidateSet(vp,N,sz(V),K,pq));
        }
        V.push_back(generateRandomPoint(N,A));
    }
    return V;
}
int main() {
    vector<DataVector> vps = generateVantagePoints(6,6,1);
    printVantagePoints(vps);
    
    return 0;
}

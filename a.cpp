#include <cstdio>
#include <cstdlib>
#include <cassert>
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
auto pqCompareFunction = [](const ii& e1,const ii& e2) {return e1.second>e2.second;};
typedef priority_queue<ii,vector<ii>,decltype(pqCompareFunction)> PQ;

int hammingDistance(DataVector& d1,DataVector& d2) {
    assert((int)d1.size()==(int)d2.size());
    int ret = 0;
    for ( int i = 0 ; i < (int)d1.size() ; i++ ) 
        ret += (d1[i] != d2[i]);
    return ret;
}

void _printDataVector(DataVector& d) {
    for ( char &c: d ) {
        printf("%c",c);
    }
}
void printDataVector(DataVector& d) {
    _printDataVector(d);
    puts("");
}
void printCandidateSet(vector<iDV>& candidateSet) {
    printf("candidate set\n");
    for ( iDV& candidate: candidateSet ) {
        printf("[");
        printf("%d,",candidate.first);
        _printDataVector(candidate.second);
        printf("] ");
    }
    puts("");
}
void printVantagePoints(vector<DataVector>& vps) {
    for ( DataVector &vp: vps ) {
        printDataVector(vp);
    }
}
void printVantagePointScore(vector<DataVector>& vps) {
    set<int> dist;
    for ( DataVector& vp1: vps ) {
        for ( DataVector& vp2: vps ) {
            if ( vp1 == vp2 ) continue;
            dist.insert(hammingDistance(vp1,vp2));
        }
    }
    printf("%d\n",sz(dist));
}
char generateAlphabet(int A=10) {
    uniform_int_distribution<int> distribution(0,A-1);
    return 'A'+distribution(generator);
}
DataVector generateRandomPoint(int N,int A=10) {
    DataVector ret;
    for ( int i = 0 ; i < N ; i++ ) {
        ret.push_back(generateAlphabet(A));
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
DataVector generatePoint(DataVector& vp,int N,int newDist,PQ& pq,int A=10) {
    DataVector newPoint = vp;
    set<int> changePosition;
    while ( sz(changePosition) < newDist ) {
        int dim = pq.top().first;
        int count = pq.top().second;
        pq.pop();
        changePosition.insert(dim);
        pq.push(ii(dim,count+1));
    }
    for ( auto& position: changePosition ) {
        char c = generateAlphabet(A);
        while ( newPoint[position] == c ) {
            c = generateAlphabet(A);
        }
        newPoint[position] = c;
    }
    return newPoint;
}
vector<iDV> generateCandidateSet(DataVector& vp,int N,int lVl,int K,PQ& pq,int A=10) {
    vector<iDV> newCandidateSet;
    set<int> dist;
    uniform_int_distribution<int> distribution(1,N);
    while ( sz(dist) < N*min(K,lVl)/lVl ) {
        int newDist = distribution(generator);
        if ( dist.count(newDist) == 0 ) {
            dist.insert(newDist);
            newCandidateSet.push_back(iDV(newDist,generatePoint(vp,N,newDist,pq,A)));
        }
    }
    return newCandidateSet;
}
vector<iDV> eraseCandidateSet(vector<iDV>& candidateSet,set<int> dist) {
    vector<iDV> ret;
    for ( iDV& candidate: candidateSet ) {
        if ( dist.count(candidate.first) == 0 ) 
            ret.push_back(candidate);
    }
    return ret;
}
vector<DataVector> generateVantagePoints(int N,int M,int K,int A=10) {
    vector<DataVector> V;
    set<int> dist;
    vector<iDV> candidateSet;
    PQ pq = initPriorityQueue(N);
#ifdef HasDataSet
#else
    V.push_back(generateRandomPoint(N,A));
#endif

    bool first = true;
    while ( (int)V.size() < M ) {
        for ( DataVector &vp: V ) {
            vector<iDV> newCandidateSet = generateCandidateSet(vp,N,sz(V),K,pq,A);
            for ( iDV& newCandidate: newCandidateSet ) {
                candidateSet.push_back(newCandidate);
            }
        }
//        printCandidateSet(candidateSet);
        int maxDistCardinality = 0;
        DataVector nextPoint;
        set<int> nextDist;
        for ( iDV& candidate: candidateSet ) {
            set<int> newDist;
            for ( DataVector& vp: V ) {
                newDist.insert(hammingDistance(vp,candidate.second));
            }
            if ( sz(newDist) > maxDistCardinality ) {
                maxDistCardinality = sz(newDist);
                nextPoint = candidate.second;
                nextDist = newDist;
            }
        }
        V.push_back(nextPoint);
        for ( auto& d: nextDist ) {
            dist.insert(d);
        }
        candidateSet = eraseCandidateSet(candidateSet,dist);
        if ( first && sz(dist) == N ) {
            printf("aleady arrive\n");
            printVantagePoints(V);
            printf("%d ",sz(V));
            printVantagePointScore(V);
            puts("");
            first = false;
        }
    }
    return V;
}
int main() {
    vector<DataVector> vps = generateVantagePoints(100,15,128);
    printVantagePoints(vps);
    printVantagePointScore(vps);
    
    return 0;
}

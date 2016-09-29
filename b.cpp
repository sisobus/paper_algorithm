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
    for ( int i = 0 ; i < (int)vps.size() ; i++ ) {
        for ( int j = 0 ; j < (int)vps.size() ; j++ ) {
            if ( i == j ) continue;
            dist.insert(hammingDistance(vps[i],vps[j]));
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
DataVector generatePoint(DataVector& vp,int N,int newDist,int A=10) {
    DataVector newPoint = vp;
    uniform_int_distribution<int> distribution(0,N-1);
    set<int> changePosition;
    while ( sz(changePosition) < newDist ) {
        changePosition.insert(distribution(generator));
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
vector<iDV> generateCandidateSet(DataVector& vp,int N,int lVl,int K,int A=10) {
    vector<iDV> newCandidateSet;
    set<int> dist;
    uniform_int_distribution<int> distribution(1,N);
    while ( sz(dist) < N*min(K,lVl)/lVl ) {
        int newDist = distribution(generator);
        if ( dist.count(newDist) == 0 ) {
            dist.insert(newDist);
            newCandidateSet.push_back(iDV(newDist,generatePoint(vp,N,newDist,A)));
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
#ifdef HasDataSet
#else
    DataVector t;for ( int i = 0 ; i < N ; i++ )t.push_back('A');
    V.push_back(t);
//    V.push_back(generateRandomPoint(N,A));
#endif

    bool first = true;
    while ( (int)V.size() < M ) {
        for ( DataVector &vp: V ) {
            vector<iDV> newCandidateSet = generateCandidateSet(vp,N,sz(V),K,A);
            for ( iDV& newCandidate: newCandidateSet ) {
                candidateSet.push_back(newCandidate);
            }
        }
//        printCandidateSet(candidateSet);
//        printVantagePoints(V);
//        printVantagePointScore(V);
        int maxDistCardinality = 0;
        DataVector nextPoint;
        set<int> nextDist;
        for ( iDV& candidate: candidateSet ) {
            set<int> newDist = dist;
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
//            printVantagePoints(V);
            printf("%d ",sz(V));
            printVantagePointScore(V);
            puts("");
            first = false;
        }
    }
    return V;
}
int main() {
    /*
    vector<DataVector> vps = generateVantagePoints(6,6,1);
    printVantagePoints(vps);
    printVantagePointScore(vps);
    */
    for ( int i = 1; i < 100 ; i++ ) {
        printf("i : %d\n",i);
        generateVantagePoints(i,i,i);
    }
    
    return 0;
}

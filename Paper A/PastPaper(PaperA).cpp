// PastPaper(PaperA).cpp

#include <iostream>
#include "PolySeq2.h"
using namespace std; 
int main() {
    int n1, n2, x1, x2;
    cin >> n1;
    int* c1 = new int[n1];
    int* e1 = new int[n1];
    for (int i = 0; i < n1; i++) {
        cin >> c1[i];
    }
    for (int i = 0; i < n1; i++) {
        cin >> e1[i];
    }
    cin >> n2;
    int* c2 = new int[n2];
    int* e2 = new int[n2];
    for (int i = 0; i < n2; i++) {
        cin >> c2[i];
    }
    for (int i = 0; i < n2; i++) {
        cin >> e2[i];
    }
    cin >> x1;
    cin >> x2;

    PolySeq P1(c1, e1, n1);
    PolySeq P2(c2, e2, n2);
    PolySeq P3 = P1.Add(P2);
    cout << P3.getValue(x1)
        << endl;
    PolySeq P4 = P1.Multiply(P2);
    cout << P4.getValue(x1)
        << endl;
    PolySeq P5 = P1.Derivative();
    cout << P5.getValue(x1)
        << endl;

    cout << P2.Integral(2, 3);
    delete[]c1;
    delete[]e1;
    delete[]c2;
    delete[]e2;



    return 0; 
}


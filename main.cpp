#include <iostream>
#include "BPlusTree.h"
#include "Pair.h"
int main()
{

    BPlusTree<Pair<int, char> > test;

    Pair<int,char> input(6,'t');
    Pair<int,char> input2(17,'s');
    Pair<int,char> input3(4,'a');
    Pair<int,char> input4(12,'a');
    Pair<int,char> input5(19,'s');
    Pair<int,char> input6(22,'a');
    Pair<int,char> input7(2,'a');
    Pair<int,char> input8(3,'a');
    Pair<int,char> input9(5,'a');
    Pair<int,char> input10(10,'a');
    Pair<int,char> input11(16,'a');
    Pair<int,char> input12(18,'a');
    Pair<int,char> input13(20,'a');
    Pair<int,char> input14(25,'a');

    test.insert(input);
    test.insert(input2);
    test.insert(input3);
    test.insert(input4);
    test.insert(input5);
    test.insert(input6);
    test.insert(input7);
    test.insert(input8);
    test.insert(input9);
    test.insert(input10);
    test.insert(input11);
    test.insert(input12);
    test.insert(input13);
    test.insert(input14);
//    test.remove(input13);
//    test.remove(input);
//    test.remove(input10);
//    test.remove(input4);

//    BPlusTree<int> tester;

//    tester.insert(6);
//    tester.insert(17);
//    tester.insert(4);
//    tester.insert(12);
//    tester.insert(19);
//    tester.insert(22);
//    tester.insert(2);
//    tester.insert(3);
//    tester.insert(5);
//    tester.insert(10);
//    tester.insert(16);
//    tester.insert(18);
//    tester.insert(20);
//    tester.insert(25);
//
//    tester.remove(20);
//    tester.remove(6);
//    tester.remove(10);
//    tester.remove(12);
//    tester.remove(16);
//    tester.remove(22);
//    tester.remove(25);


//    cout << tester<<endl<<endl;
    cout <<test<<endl<<endl;


//    tester.contains(25);
    return 0;
}
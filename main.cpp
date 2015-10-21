/*******************************************************************
*  Copyright(c) 2015     wu.2724@osu.edu
*  All rights reserved.
*
*  File name:   main
*  Description: this is the main entry file of LISP Parser
*
*  Date: 10/01/2015
*  Author: Longhua Wu
******************************************************************/
#include <iostream>
#include <cstdio>
#include <locale>
#include "lispparser.h"

using namespace std;

int main()
{
    string strExpr="(REMAINDER .(-5.(-3.NIL)))";
    LispParser lp;
    char t;

    // put all texts from the inputfile to the string strExpr
//    while(true)
//    {
//        t = getchar();
//        if(t==char_traits<char>::eof())
//        {
//            break;
//        }
//        strExpr.push_back(t);
//    }

    size_t i=0;
    size_t strLen = strExpr.length();
    while(i<strLen)
    {
        TreeNode *root = new TreeNode();
        lp.resetStatus();
        lp.buildBinaryTree(strExpr,i,root);
        lp.printExpr(root);
        if(i<strLen)
            cout << endl;
        lp.deleteBinaryTree(root);
    }
    return 0;
}


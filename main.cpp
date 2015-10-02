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
    string strExpr;
    LispParser lp;
    char t;

    // put all texts from the inputfile to the string strExpr
    while(true)
    {
        t = getchar();
        if(t==std::char_traits<char>::eof())
        {
            break;
        }
        if(!isspace(t))
            strExpr.push_back(t);
    }

    size_t i=0;
    size_t strLen = strExpr.length();
    while(i<strLen)
    {
        TreeNode *root = new TreeNode();
        lp.resetStatus();
        lp.buildBinaryTree(strExpr,i,root);
        lp.checkInnerNodesList(root);
        if(lp.getIsAllList())
            lp.printListExpr(root);
        else
            lp.printNodeExpr(root);
        cout << endl;
        lp.deleteBinaryTree(root);
    }
    return 0;
}


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
    while(true)
    {
        t = getchar();
        if(t==std::char_traits<char>::eof())
            break;
        if(t=='\n')
        {
            if(strExpr.length()>0)
            {

                size_t i=0;
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
            strExpr.clear();
            continue;
        }
        if(!isspace(t))
            strExpr.push_back(t);
    }

    return 0;
}


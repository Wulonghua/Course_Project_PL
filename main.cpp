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
#include "lispevaluater.h"

using namespace std;

int main()
{
    //(REMAINDER .(-5.(-3.NIL)))
    //(ATOM.((QUOTE.(X.NIL)).NIL))
    //(EQ.(T.(T.NIL)))
    //(INT.(5.NIL))
    //(NULL.(5.NIL))
    //(PLUS.((PLUS.(1.(2.NIL))).(3.NIL)))
    //(CAR.((QUOTE.((7.(8.NIL)).NIL)).NIL))
    //(CDR.((QUOTE.((7.(8.NIL)).NIL)).NIL))
    //(CONS.(7.(10.NIL)))
    //(REMAINDER.(5.(-2.NIL)))
    //(PLUS.((TIMES.((QUOTIENT.(5.(3.NIL))).(3.NIL))).((REMAINDER.(5.(3.NIL))).NIL)))
    //(LESS.((PLUS.(2.(3.NIL))).(6.NIL)))
    string strExpr="(GREATER.((PLUS.(2.(3.NIL))).(6.NIL)))";
    LispParser *lp = new LispParser();
    LispEvaluater *le = new LispEvaluater(lp);

    //char t;
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
        lp->resetStatus();
        lp->buildBinaryTree(strExpr,i,root);
        le->evaluateExpr(root);
        lp->updateIsList(root);
        lp->printExpr(root);
        if(i<strLen)
            cout << endl;
        lp->deleteBinaryTree(root);
    }
    return 0;
}


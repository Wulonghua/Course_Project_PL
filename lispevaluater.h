/***********************************************************************************
*  Copyright(c) 2015     wu.2724@osu.edu
*  All rights reserved.
*
*  File name:   lispevaluater
*  Description: This is the class LispEvaluater, which aims to evaluate the input Expression
*
*  Date: 10/27/2015
*  Author: Longhua Wu
*************************************************************************************/
#ifndef LISPEVALUATER_H
#define LISPEVALUATER_H

#include "lispparser.h"
#include <vector>
#include <iterator>

using namespace std;

class LispEvaluater
{
public:
    LispEvaluater(LispParser *lisp_parser);
    virtual ~LispEvaluater();
    void evaluateExpr(TreeNode *node);

private:
    bool checkIsAtom(TreeNode *node) {return !lp->checkIsInnerNode(node);}

    TreeNode* applyFunction(TreeNode *node,TreeNode *node_para);
    TreeNode* generateEvList(TreeNode *node);
    void computeCond(TreeNode *root);
    LispParser *lp;

};

#endif // LISPEVALUATER_H

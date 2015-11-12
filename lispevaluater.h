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
#include <set>

using namespace std;

class LispEvaluater
{
public:
    LispEvaluater(LispParser *lisp_parser);
    virtual ~LispEvaluater();
    void evaluateExpr(TreeNode *node, map<string,TreeNode*> alistMap );

private:
    bool checkIsAtom(TreeNode *node) {return !lp->checkIsInnerNode(node);}
	void checkParaNum(string func_name, TreeNode *node_para);
    TreeNode* applyFunction(TreeNode *node,TreeNode *node_para);
	TreeNode* generateEvList(TreeNode *node, map<string, TreeNode*> alistMap);
	void computeCond(TreeNode *root, map<string, TreeNode*> alistMap);
    void addValuePair(TreeNode *formal, TreeNode *actual, map<string, TreeNode*>& listpair);

private:
    LispParser *lp;

};

#endif // LISPEVALUATER_H

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
    void testPrint(TreeNode *node, vector<int> orders);

private:
    bool checkIsAtom(TreeNode *node) {return !lp->checkIsInnerNode(node);}

    TreeNode* applyFunction(TreeNode *node,TreeNode *node_para);
    TreeNode* generateEvList(TreeNode *node);
    LispParser *lp;

};

#endif // LISPEVALUATER_H

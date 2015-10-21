/***********************************************************************************
*  Copyright(c) 2015     wu.2724@osu.edu
*  All rights reserved.
*
*  File name:   lispparser
*  Description: This is the class LispParser, which mainly realizes the funtion of
*  lexical and syntatic analysis, and output the results in required format.
*
*  Date: 10/01/2015
*  Author: Longhua Wu
*************************************************************************************/
#ifndef LISPPARSER_H
#define LISPPARSER_H

#include <iostream>
#include <string>
#include <locale>
#include <cstdlib>
#include <stdexcept>

using namespace std;

enum valueType { BOOL_TYPE, INT_TYPE, NO_TYPE};

struct NodeValue
{
    int intValue;
    bool boolValue;
    valueType vType;
    NodeValue() : vType(NO_TYPE) {}
};

struct TreeNode
{
    string expr;
    TreeNode *left;
    TreeNode *right;
    TreeNode *rSibling;
    bool isList;
    NodeValue nodeValue;
    TreeNode() : left(NULL),right(NULL),rSibling(NULL),isList(false) {}
    TreeNode(string x) : expr(x), left(NULL), right(NULL),rSibling(NULL),isList(false) {}
};

class LispParser
{
//FUNCTION
public:
    LispParser();
    void buildBinaryTree(string textline, size_t &curIdx, TreeNode *node);
    void checkInnerNodesList(TreeNode *node);
    // Always call function checkInnerNodesList before get IsAllList
    bool getIsAllList() {return isAllList;}
    void resetStatus();
    void deleteBinaryTree(TreeNode *node);
    void printExpr(TreeNode *node);
    NodeValue evaluateExpr(TreeNode *node);

private:
    bool checkIsInnerNode(TreeNode *node) {return (node->left != NULL && node->right != NULL);}
    string getNextToken(string textline, size_t &curIdx);
    void printNodeExpr(TreeNode *node);
    void printListExpr(TreeNode *node);
//CLASS MEMBER
private:
    bool hasLetter;           // determine whether an atom has letter.
    bool hasMinusSign;
    bool isAllList;           // determine whether inner nodes' isList attributes are all true
                              // if it is true, print out in list notation,otherwise dot notation
};

#endif // LISPPARSER_H

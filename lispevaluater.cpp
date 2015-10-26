#include "lispevaluater.h"

LispEvaluater::LispEvaluater(LispParser *lisp_parser)
{
    lp = lisp_parser;
}

LispEvaluater::~LispEvaluater()
{

}

void LispEvaluater::evaluateExpr(TreeNode *node)
{
    if(checkIsAtom(node)) // exp is an atom
    {
        if(node->expr == string("T"))
        {
            node->nodeValue.vType = BOOL_TYPE;
            node->nodeValue.boolValue = true;
        }
        else if(node->expr == string("NIL"))
        {
            node->nodeValue.vType = BOOL_TYPE;
            node->nodeValue.boolValue = false;
        }
        else if(node->expr.find_first_not_of("-0123456789")==string::npos) // number
        {
            node->nodeValue.vType = INT_TYPE;
            node->nodeValue.intValue = stoi(node->expr);
        }
        else
            throw runtime_error("EUBL"); // ERROR: unbound literal
    }
    else // exp is a list
    {
        if(node->left->expr == string("QUOTE"))
        {
            TreeNode *temp1= node->left;
            TreeNode *temp2 = node->right;
            *node = *(node->right->left);
            lp->deleteBinaryTree(temp1);
            temp2->left = NULL;
            lp->deleteBinaryTree(temp2);
        }
        else if(node->expr == string("COND"))
        {
            // to add
        }
        else
        {
            TreeNode *temp1= node->left;
            TreeNode *temp2 = node->right;
            //          vector<int> flags;
            //          testPrint(node,flags);
            //          cout << endl;
            *node =*applyFunction(node,generateEvList(node->right));
            //          flags.clear();
            //          testPrint(node,flags);
            //          cout << endl;
            if(temp2 != NULL)
            {
                temp2->left = NULL;
                if(temp1->expr==string("CONS"))
                {
                    temp2->right = NULL;
                }
                lp->deleteBinaryTree(temp2);
                temp2 = NULL;
            }
            if(temp1 != NULL)
            {
                lp->deleteBinaryTree(temp1);
                temp1 = NULL;
            }
            //           flags.clear();
            //           testPrint(node,flags);
        }
    }
}

TreeNode* LispEvaluater::generateEvList(TreeNode *node)
{
    if(!(checkIsAtom(node)&&node->expr==string("NIL")))
    {
        evaluateExpr(node->left);
        //cout << node->left->expr << endl;
        generateEvList(node->right);
        //cout << node->right->expr << endl;
    }

    return node;
}

TreeNode* LispEvaluater::applyFunction(TreeNode *node, TreeNode *node_para)
{
    TreeNode *node_func = node->left;
    //cout << node_para->left->expr << " " << node_para->right->expr << endl;
    if(node_func->expr == string("ATOM"))
    {
        node_para->nodeValue.vType = BOOL_TYPE;
        if(checkIsAtom(node_para->left))
        {
            node_para->nodeValue.boolValue = true;
            node_para->expr = string("T");
        }
        else
        {
            node_para->nodeValue.boolValue = false;
            node_para->expr = string("NIL");
        }
        lp->deleteBinaryTree(node_para->left);
        lp->deleteBinaryTree(node_para->right);
        node_para->left = NULL;
        node_para->right = NULL;
    }
    else if(node_func->expr == string("EQ")||
            node_func->expr == string("LESS")||
            node_func->expr == string("GREATER"))
    {
        bool flag;
        if(node_func->expr == string("EQ"))
        {
            flag = (node_para->left->expr == node_para->right->left->expr) ? true : false;
        }
        else if(node_func->expr == string("LESS"))
        {
            flag = (node_para->left->nodeValue.intValue < node_para->right->left->nodeValue.intValue) ? true : false;
        }
        else if(node_func->expr == string("GREATER"))
        {
            flag = (node_para->left->nodeValue.intValue > node_para->right->left->nodeValue.intValue) ? true : false;
        }
        node_para->nodeValue.vType = BOOL_TYPE;
        if(flag)
        {
            node_para->nodeValue.boolValue = true;
            node_para->expr = "T";
        }
        else
        {
            node_para->nodeValue.boolValue = false;
            node_para->expr = string("NIL");
        }
        lp->deleteBinaryTree(node_para->left);
        lp->deleteBinaryTree(node_para->right);
        node_para->left = NULL;
        node_para->right = NULL;
    }
    else if(node_func->expr == string("INT"))
    {
        node_para->nodeValue.vType = BOOL_TYPE;
        if(node_para->left->nodeValue.vType == INT_TYPE)
        {
            node_para->nodeValue.boolValue = true;
            node_para->expr = "T";
        }
        else
        {
            node_para->nodeValue.boolValue = false;
            node_para->expr = string("NIL");
        }
        lp->deleteBinaryTree(node_para->left);
        lp->deleteBinaryTree(node_para->right);
        node_para->left = NULL;
        node_para->right = NULL;
    }
    else if(node_func->expr == string("NULL"))
    {
        node_para->nodeValue.vType = BOOL_TYPE;
        if(checkIsAtom(node_para->left) && node_para->left->expr == string("NIL"))
        {
            node_para->nodeValue.boolValue = true;
            node_para->expr = "T";
        }
        else
        {
            node_para->nodeValue.boolValue = false;
            node_para->expr = string("NIL");
        }
        lp->deleteBinaryTree(node_para->left);
        lp->deleteBinaryTree(node_para->right);
        node_para->left = NULL;
        node_para->right = NULL;
    }
    //PLUS, MINUS, TIMES, QUOTIENT, REMAINDER
    else if(node_func->expr == string("PLUS")||
            node_func->expr == string("MINUS")||
            node_func->expr == string("TIMES")||
            node_func->expr == string("QUOTIENT")||
            node_func->expr == string("REMAINDER"))
    {
        node_para->nodeValue.vType = INT_TYPE;
        if(node_func->expr == string("PLUS"))
            node_para->nodeValue.intValue = node_para->left->nodeValue.intValue +
                                            node_para->right->left->nodeValue.intValue;
        else if(node_func->expr == string("MINUS"))
            node_para->nodeValue.intValue = node_para->left->nodeValue.intValue -
                                            node_para->right->left->nodeValue.intValue;
        else if(node_func->expr == string("TIMES"))
            node_para->nodeValue.intValue = node_para->left->nodeValue.intValue *
                                            node_para->right->left->nodeValue.intValue;
        else if(node_func->expr == string("QUOTIENT"))
            node_para->nodeValue.intValue = node_para->left->nodeValue.intValue /
                                            node_para->right->left->nodeValue.intValue;
        else if(node_func->expr == string("REMAINDER"))
            node_para->nodeValue.intValue = node_para->left->nodeValue.intValue %
                                            node_para->right->left->nodeValue.intValue;
        node_para->expr = to_string(node_para->nodeValue.intValue);
        lp->deleteBinaryTree(node_para->left);
        lp->deleteBinaryTree(node_para->right);
        node_para->left = NULL;
        node_para->right = NULL;
    }
    else if(node_func->expr == string("CAR"))
    {
        node->right = node_para->left->left;
        node_para->left->left = NULL;
        lp->deleteBinaryTree(node_para);
        node_para = node->right;
    }
    else if(node_func->expr == string("CDR"))
    {
        node->right = node_para->left->right;
        node_para->left->right = NULL;
        lp->deleteBinaryTree(node_para);
        node_para = node->right;
    }
    else if(node_func->expr == string("CONS"))
    {
        //        vector<int> flags;
        //        testPrint(node_para,flags);
        //        cout << endl;
        TreeNode *tmp = node_para->right;
        node_para->right = node_para->right->left;
        tmp->left = NULL;
        lp->deleteBinaryTree(tmp);
        tmp = NULL;
        //        flags.clear();
        //        testPrint(node_para,flags);
    }
    return node_para;
}

void LispEvaluater::testPrint(TreeNode *node, vector<int> orders)
{
    if(node!=NULL)
    {
        if(checkIsAtom(node))
        {
            std::copy(orders.begin(),orders.end(),std::ostream_iterator<int>(std::cout<< "->" ));
            cout << ":" << node->expr << " "<<endl;
        }
        else
        {
            if(node->left!=NULL)
            {
                orders.push_back(0);
                testPrint(node->left,orders);
                orders.pop_back();
            }

            if(node->right!=NULL)
            {
                orders.push_back(1);
                testPrint(node->right,orders);
                orders.pop_back();
            }
        }
    }
}


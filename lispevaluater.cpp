#include "lispevaluater.h"

LispEvaluater::LispEvaluater(LispParser *lisp_parser)
{
    lp = lisp_parser;
}

LispEvaluater::~LispEvaluater()
{

}

void LispEvaluater::evaluateExpr(TreeNode *node, map<string,TreeNode*> alistMap)
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
            node->nodeValue.intValue = atoi(node->expr.c_str());
        }
        else if(alistMap.find(node->expr) != alistMap.end()) // find the actual value correspondent to the formal parameter
        {
            TreeNode *para_cpy = new TreeNode();
            lp->copyTree(alistMap[node->expr],para_cpy);
            *node = *para_cpy;
        }
		else
		{
			cout << "unbound liter: " << node->expr << endl;
			throw runtime_error("UBL"); // ERROR: unbound literal
		}
	}
    else // exp is a list
    {
        if(node->left->expr == string("QUOTE"))
        {
			if (node->right->left == NULL)
				throw runtime_error("WA_QUO");
			if (node->right->right->expr != string("NIL"))
				throw runtime_error("NL_QUO");
			//lp->updateIsList(node->right->left);
			//lp->checkInnerNodesList(node->right->left);
			//if (!lp->getIsAllList())
			//	throw runtime_error("NLA_QUO");
            TreeNode *temp1= node->left;
            TreeNode *temp2 = node->right;
            *node = *(node->right->left);
            lp->deleteBinaryTree(temp1);
            temp1 = NULL;
            temp2->left = NULL;
            lp->deleteBinaryTree(temp2);
            temp2 = NULL;
        }
        else if(node->left->expr == string("COND"))
        {
             computeCond(node,alistMap);
             TreeNode *temp1 = node->left;
             TreeNode *temp2 = node->right;
             *node = *(node->right);
             delete temp1;
             delete temp2;
        }
        else if(node->left->expr == string("DEFUN"))
        {
            if(node->right->right == NULL ||
               node->right->right->right == NULL ||
               node->right->right->right->right == NULL ||
               node->right->right->right->right->expr != string("NIL"))
                throw runtime_error("WA_DF");

			// funtion name is a literal atom(should not be number)
			if (!checkIsAtom(node->right->left))
				throw runtime_error("WT_DF_F");
			else
			{
				if (node->right->left->expr.find_first_not_of("0123456789") == string::npos)
					throw runtime_error("WT_DF_F");
			}

			// should not have conflic same name with inner function
            string func_name = node->right->left->expr;
            if(func_name == string("CAR")||func_name == string("CDR")||
               func_name == string("CONS")||func_name == string("ATOM")||
               func_name == string("EQ")||func_name == string("NULL")||
               func_name == string("INT")||func_name == string("PLUS")||
               func_name == string("MINUS")||func_name == string("TIMES")||
               func_name == string("QUOTIENT")||func_name == string("REMAINDER")||
               func_name == string("LESS")||func_name == string("GREATER")||
               func_name == string("COND")||func_name == string("QUOTE")||func_name == string("DEFUN"))
                throw runtime_error("CF_DF");
            
			// Formals are a list, and each one is a distinct literal atom (not T or NIL)
			TreeNode *formal_list = node->right->right->left;
			set<string> formal_names;
			if (checkIsAtom(formal_list))
			{
				if (formal_list->expr != string("NIL"))
					throw runtime_error("WT_DF_FM");
			}
			else
			{
				do{
					if (!checkIsAtom(formal_list->left))
						throw runtime_error("WT_DF_FM");
					else
					{
						if (formal_list->left->expr.find_first_not_of("0123456789") == string::npos||
							formal_list->left->expr == string("T")||
							formal_list->left->expr == string("NIL"))
							throw runtime_error("WT_DF_FM");
						if (formal_names.find(formal_list->left->expr) == formal_names.end())
							formal_names.insert(formal_list->left->expr);
						else
							throw runtime_error("WT_DF_FM");
					}
					formal_list = formal_list->right;
				} while (!checkIsAtom(formal_list));
				if (formal_list->expr != string("NIL"))
					throw runtime_error("WT_DF_FM");
			}

			TreeNode * temp = node->right->right->right;
            node->right->right->right = temp->left;
            temp->left = NULL;
            lp->deleteBinaryTree(temp);
            temp=NULL;
            lp->dlistMap[node->right->left->expr] = node->right->right;
            node->right->right = NULL;
			*node = *(node->right->left);
        }
        else
        {
            TreeNode *temp1= node->left;
            //TreeNode *temp2 = node->right;
                      vector<int> flags;
                      //lp->testPrint(temp2,flags);
                      //cout << endl;
            *node =*applyFunction(node,generateEvList(node->right,alistMap));
//                      flags.clear();
//                      lp->testPrint(node,flags);
//                      cout <<" node end " <<endl;

//                      flags.clear();
//                      lp->testPrint(temp1, flags);
//                      cout <<" temp1 end " <<endl;

            //          flags.clear();
            //          lp->testPrint(temp2, flags);
            //          cout <<" temp2 end "<< endl;
            //if(temp2 != NULL)
            //{
            //    temp2->left = NULL;
            //    if(temp1->expr==string("CONS"))
            //    {
            //        temp2->right = NULL;
            //    }
            //    lp->deleteBinaryTree(temp2);
            //    temp2 = NULL;
            //}
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

TreeNode* LispEvaluater::generateEvList(TreeNode *node, map<string, TreeNode*> alistMap)
{
	if (!(checkIsAtom(node) && node->expr == string("NIL")))
	{
		if (!checkIsAtom(node))
		{
		evaluateExpr(node->left,alistMap);
		//cout << node->left->expr << endl;
		generateEvList(node->right,alistMap);
		//cout << node->right->expr << endl;
		}
    }
    return node;
}

TreeNode* LispEvaluater::applyFunction(TreeNode *node, TreeNode *node_para)
{
    TreeNode *node_func = node->left;

    if(node_func->expr == string("ATOM") ||
       node_func->expr == string("INT")  ||
       node_func->expr == string("NULL"))
    {
        if(node_para->left == NULL)
            throw runtime_error(string("WA_")+node_func->expr);
		if (node_para->right->expr != string("NIL"))
			throw runtime_error(string("NL_")+node_func->expr);
		
		bool flag = false;
		node_para->nodeValue.vType = BOOL_TYPE;
        if(node_func->expr == string("ATOM"))
            flag = checkIsAtom(node_para->left);
        else if(node_func->expr == string("INT"))
            flag = (node_para->left->nodeValue.vType == INT_TYPE) ? true : false;
        else if(node_func->expr == string("NULL"))
            flag = (checkIsAtom(node_para->left) && node_para->left->expr == string("NIL")) ? true:false;
        if(flag)
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
		if (node_para->left == NULL ||
			node_para->right == NULL ||
			node_para->right->left == NULL)
			throw runtime_error(string("WA_CO"));
		if (node_para->right->right->expr != string("NIL"))
			throw runtime_error(string("NL_CO"));
		bool flag = false;
        if(node_func->expr == string("EQ"))
        {
            if((!checkIsAtom(node_para->left)) || (!checkIsAtom(node_para->right->left)))
                throw runtime_error("WT_EQ");
            flag = (node_para->left->expr == node_para->right->left->expr) ? true : false;
        }
        else if(node_func->expr == string("LESS"))
        {
			if (node_para->left->nodeValue.vType != INT_TYPE || node_para->right->left->nodeValue.vType != INT_TYPE)
				throw runtime_error("WT_LESS");
            flag = (node_para->left->nodeValue.intValue < node_para->right->left->nodeValue.intValue) ? true : false;
        }
        else if(node_func->expr == string("GREATER"))
        {
			if (node_para->left->nodeValue.vType != INT_TYPE || node_para->right->left->nodeValue.vType != INT_TYPE)
				throw runtime_error("WT_GREATER");
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
    //PLUS, MINUS, TIMES, QUOTIENT, REMAINDER
    else if(node_func->expr == string("PLUS")||
            node_func->expr == string("MINUS")||
            node_func->expr == string("TIMES")||
            node_func->expr == string("QUOTIENT")||
            node_func->expr == string("REMAINDER"))
    {
		if (node_para->left == NULL ||
			node_para->right == NULL ||
			node_para->right->left == NULL)
			throw runtime_error(string("WA_AF"));
		if (node_para->right->right->expr != string("NIL"))
			throw runtime_error(string("NL_AF"));
		if (node_para->left->nodeValue.vType != INT_TYPE || node_para->right->left->nodeValue.vType != INT_TYPE)
			throw runtime_error("WT_AF");

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
        node_para->expr = lp->int2str(node_para->nodeValue.intValue);
        lp->deleteBinaryTree(node_para->left);
        lp->deleteBinaryTree(node_para->right);
        node_para->left = NULL;
        node_para->right = NULL;
    }
    else if(node_func->expr == string("CAR"))
    {
		if (node_para->left == NULL ||
			node_para->left->left == NULL)
			throw runtime_error("WA_CAR");
		if (node_para->right->expr != string("NIL"))
			throw runtime_error("NL_CAR");
		// check whether argument is a list
		//lp->updateIsList(node_para->left);
		//lp->checkInnerNodesList(node_para->left);
		//if (!lp->getIsAllList())
		//	throw runtime_error("NLA_CAR");
		node->right = node_para->left->left;
        node_para->left->left = NULL;
        //lp->deleteBinaryTree(node_para);
        node_para = node->right;
    }
    else if(node_func->expr == string("CDR"))
    {
		if (node_para->left == NULL ||
			node_para->left->right == NULL)
			throw runtime_error("WA_CDR");
		if (node_para->right->expr != string("NIL"))
			throw runtime_error("NL_CDR");
		// check whether argument is a list
		//lp->updateIsList(node_para->left);
		//lp->checkInnerNodesList(node_para->left);
		//if (!lp->getIsAllList())
		//	throw runtime_error("NLA_CDR");

        node->right = node_para->left->right;
        node_para->left->right = NULL;
        //lp->deleteBinaryTree(node_para);
        node_para = node->right;
    }
    else if(node_func->expr == string("CONS"))
    {
        //        vector<int> flags;
        //        testPrint(node_para,flags);
        //        cout << endl;
		if (node_para->right == NULL ||
			node_para->right->left == NULL)
			throw runtime_error("WA_CONS");
		if (node_para->right->right->expr != string("NIL"))
			throw runtime_error("NL_CONS");
        TreeNode *tmp = node_para->right;
        node_para->right = node_para->right->left;
        tmp->left = NULL;
        lp->deleteBinaryTree(tmp);
        tmp = NULL;
        //        flags.clear();
        //        testPrint(node_para,flags);
    }
    else if(lp->dlistMap.find(node_func->expr) != lp->dlistMap.end())
    {

        TreeNode *para_list = lp->dlistMap[node_func->expr]->left;
        TreeNode *formal_iter = para_list;
		map<string, TreeNode*> alistSubMap;
        addValuePair(formal_iter,node_para,alistSubMap);
        //lp->testPrintAList();
        TreeNode *func_body_cpy = new TreeNode();
        lp->copyTree(lp->dlistMap[node_func->expr]->right,func_body_cpy);
        //lp->printExpr(func_body_cpy);
        //cout <<endl;
        evaluateExpr(func_body_cpy,alistSubMap);
		lp->clearListMap(alistSubMap);
        //vector<int> flag;
        //lp->testPrint(func_body_cpy,flag);
        return func_body_cpy;
    }
    else
    {
		cout << "func name: " << node_func->expr << endl;
        throw runtime_error("UF"); // ERROR: Unrecognized function
    }
    return node_para;
}

void LispEvaluater::computeCond(TreeNode *root, map<string, TreeNode*> alistMap)
{
	TreeNode* node = root->right;
	if (checkIsAtom(node) && node->expr == string("NIL"))
		throw runtime_error("UD_COND"); // missing argument
	if (node->left == NULL ||
        node->left->left == NULL||
        node->left->right->left == NULL||
        node->left->right->right->expr != string("NIL"))
		throw runtime_error("WA_COND");
    evaluateExpr(node->left->left, alistMap);
	if (node->left->left->nodeValue.vType != BOOL_TYPE)
		throw runtime_error("WT_COND");
    if(node->left->left->nodeValue.boolValue)
    {
        evaluateExpr(node->left->right->left, alistMap);
        root->right = node->left->right->left;
        node->left->right->left = NULL;
        lp->deleteBinaryTree(node);
        node = NULL;
    }
    else
    {
        root->right = node->right;
        node->right = NULL;
        lp->deleteBinaryTree(node);
        node = NULL;
        computeCond(root, alistMap);
    }
}

void LispEvaluater::addValuePair(TreeNode *formal, TreeNode *actual, map<string, TreeNode*>& listpair)
{
    while(formal->expr != string("NIL"))
    {
		TreeNode *actual_node = new TreeNode();
		if (actual->left == NULL)
			throw runtime_error("WA_SDF");
		lp->copyTree(actual->left, actual_node);
		listpair[formal->left->expr] = actual_node;

        formal = formal->right;
        actual = actual->right;
        addValuePair(formal,actual,listpair);
    }
	if (actual->expr != string("NIL"))
		throw runtime_error("WA_SDF");
}



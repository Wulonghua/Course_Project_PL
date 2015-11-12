/*******************************************************************
*  Copyright(c) 2015     wu.2724@osu.edu
*  All rights reserved.
*
*  File name:   main
*  Description: this is the main entry file of LISP Parser
*
*  Date: 10/01/2015
*  Modified: 10/27/2015    description: redesign structure
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

	string strExpr;
	LispParser *lp = new LispParser();
	LispEvaluater *le = new LispEvaluater(lp);

	char t;
	//put all texts from the inputfile to the string strExpr
	while(true)
	{
	    t = getchar();
	    if(t==char_traits<char>::eof())
	    {
	        break;
	    }
	    strExpr.push_back(t);
	}

	size_t i = 0;
	size_t strLen = strExpr.length();
	try{
		while (i < strLen)
		{
			TreeNode *root = new TreeNode();
			lp->resetStatus();
			lp->buildBinaryTree(strExpr, i, root);
			map<string, TreeNode*> alistMap;
			le->evaluateExpr(root, alistMap);
			lp->updateIsList(root);
			lp->printExpr(root);
			//                vector<int> flag;
			//                lp->testPrint(root,flag);
			//                cout <<endl;
			if (i < strLen)
				cout << endl;
			lp->deleteBinaryTree(root);
			root = NULL;
		}
	}
	catch (runtime_error re)
	{
		cout << "ERROR: ";
		if (re.what() == string("CF_DF"))
			cout << "Conflic name of self-defined function.";
		else if (re.what() == string("ID"))
			cout << "Invalid atom which contains letter beginning with digit.";
		else if (re.what() == string("IM"))
			cout << "Invalide atom with minus sign at wrong position.";
		else if (re.what() == string("MDOT"))
			cout << "Missing dot symbol";
		else if (re.what() == string("MCLO"))
			cout << "Missing closeparenthesis";
		else if (re.what() == string("MAO"))
			cout << "Missing atom or openparenthesis";
		else if (re.what() == string("NL_AF"))
			cout << "The expression of arithmetic functions should be a list with two parameters";
		else if (re.what() == string("NL_ATOM"))
			cout << "The expression of ATOM should be a list with one parameter";
		else if (re.what() == string("NL_NULL"))
			cout << "The expression of NULL should be a list with one parameter";
		else if (re.what() == string("NL_INT"))
			cout << "The expression of INT should be a list with one parameter";
		else if (re.what() == string("NL_CAR"))
			cout << "The expression of CAR should be a list with one parameter";
		else if (re.what() == string("NL_CDR"))
			cout << "The expression of CDR should be a list with one parameter";
		else if (re.what() == string("NL_CONS"))
			cout << "The expression of CONS should be a list with two parameters";
		else if (re.what() == string("NL_CO"))
			cout << "The expression of comparison(Equal or LESS or GREATER) should be a list with two parameters";
		else if (re.what() == string("NL_QUO"))
			cout << "The expression of QUOTE should be a list with one parameter";
		else if (re.what() == string("UBL"))
			cout << "Unbound literal";
		else if (re.what() == string("UD_COND"))
			cout << "Conditions are all NIL, the value is undefined.";
		else if (re.what() == string("UF"))
			cout << "Unrecognized function";
		else if (re.what() == string("US"))
			cout << "Unaccecptable symbol.";
		else if (re.what() == string("US_LL"))
			cout << "Unacceptable symbol-letter in lower case.";
		else if (re.what() == string("WA_ATOM"))
			cout << "Argument of ATOM is wrong.";
		else if (re.what() == string("WA_AF"))
			cout << "Arguments of arithmetic functions are wrong.";
		else if (re.what() == string("WA_CAR"))
			cout << "Argument of CAR is wrong.";
		else if (re.what() == string("WA_CDR"))
			cout << "Argument of CDR is wrong.";
		else if (re.what() == string("WA_CO"))
			cout << "Arguments of comparison(Equal or LESS or GREATER) are wrong";
		else if (re.what() == string("WA_COND"))
			cout << "Arguments of COND are wrong.";
		else if (re.what() == string("WA_CONS"))
			cout << "Arguments of CONS are wrong.";
		else if (re.what() == string("WA_DF"))
			cout << "Arguments of DEFUN are wrong.";
		else if (re.what() == string("WA_INT"))
			cout << "Argument of INT is wrong.";
		else if (re.what() == string("WA_NULL"))
			cout << "Argument of NULL is wrong.";
		else if (re.what() == string("WA_QUO"))
			cout << "Argument of QUOTE is wrong.";
		else if (re.what() == string("WA_SDF"))
			cout << "Arguments of the self-defined function should be corresponded to the formals";
		else if (re.what() == string("WT_AF"))
			cout << "Arguments of arithmetic functions should be integers";
		else if (re.what() == string("WT_DF_F"))
			cout << "Self defined function's name should be a literal atom (not a number).";
		else if (re.what() == string("WT_DF_FM"))
			cout << " Formals should be in the form of a list, and each one is a distinct literal atom (not T or NIL).";
		else if (re.what() == string("WT_EQ"))
			cout << "Arguments of EQ should be atoms";
		else if (re.what() == string("WT_COND"))
			cout << "Conditions of COND should be booleans.";
		else if (re.what() == string("WT_GREATER"))
			cout << "Arguments of GREATER should be integers.";
		else if (re.what() == string("WT_LESS"))
			cout << "Arguments of LESS should be integers.";
		cout << endl;
		exit(0);
	}
	return 0;
}


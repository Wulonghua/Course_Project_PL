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
	 //VALID TEST CASES
 //   (ATOM.((QUOTE.(X.NIL)).NIL))
 //   (EQ.(T.(T.NIL)))
	//(GREATER.(6.((QUOTE.(5.NIL)).NIL)))
 //   (INT.(5.NIL))
 //   (NULL.(5.NIL))
 //   (PLUS.((PLUS.(1.(2.NIL))).(3.NIL)))
 //   (CAR.((QUOTE.((7.(8.NIL)).NIL)).NIL))
    //(CDR.((QUOTE.((7.(8.NIL)).NIL)).NIL))
    //(CONS.(7.(10.NIL)))
	//(CONS.((QUOTE.((7.9).NIL)).(10.NIL)))
	//(CONS.((TIMES.((QUOTIENT.(5.(3.NIL))).(3.NIL))).((REMAINDER.(5.(3.NIL))).NIL)))
    //(REMAINDER.(5.(-2.NIL)))
    //(PLUS.((TIMES.((QUOTIENT.(5.(3.NIL))).(3.NIL))).((REMAINDER.(5.(3.NIL))).NIL)))
    //(LESS.((PLUS.(2.(3.NIL))).(6.NIL)))
    //(COND.(((EQ.(3.(4.NIL))).(9.NIL)).((T.(7.NIL)).NIL)))
	//(PLUS.((QUOTE.(5.NIL)).(2.NIL)))
	// INVALID TEST CASES
	//(5.6)
 //   (COND.NIL)
 //   (POWER.((QUOTE.(X.NIL)).NIL))
 //   (NULL.NILL)
 //   (ATOM.(X.NIL))
	//(INT.(5.6))
	//(LESS.(5.6))
	//(EQ.(5.(6.(NIL.NIL))))
	//(LESS.((QUOTE.(X.NIL)).(6.NIL)))
	//(PLUS.((QUOTE.(X.NIL)).(2.NIL)))
	//(CAR.((1.2).NIL))
	//(CAR.((QUOTE.(7.NIL)).NIL))
	//(CAR.((QUOTE.((7.(8.NIL)).NIL)).T))
	//(CONS.(7.(8.9)))
	//(COND.(((EQ.(3.(4.NIL))).((8.9).NIL)).((NIL.((6.7).NIL)).NIL)))
	//(COND.(((PLUS.(3.(4.NIL))).((8.9).NIL)).((NIL.((6.7).NIL)).NIL)))
	//(COND.(EQ.(3.(4.NIL))))
    //(EQ.((QUOTE.((5.NIL).NIL)).(5.NIL)))
    //(COND.((NIL.(5.NIL)).((T.(6.(7.NIL))).NIL)))
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

    size_t i=0;
    size_t strLen = strExpr.length();
    try{
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
    }catch(runtime_error re)
    {
        cout << "ERROR: ";
        if(re.what() == string("CF_DF"))
            cout << "Conflic name of self-defined function.";
        else if(re.what() == string("ID"))
            cout << "Invalid atom which contains letter beginning with digit." ;
        else if(re.what() == string("IM"))
            cout << "Invalide atom with minus sign at wrong position.";
        else if(re.what()==string("MDOT"))
            cout << "Missing dot symbol" ;
        else if(re.what()==string("MCLO"))
            cout << "Missing closeparenthesis" ;
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
        else if(re.what()==string("UF"))
            cout << "Unrecognized function";
        else if(re.what() == string("US"))
            cout << "Unaccecptable symbol.";
		else if (re.what() == string("US_LL"))
			cout << "Unacceptable symbol-letter in lower case.";
		else if (re.what() == string("WA_CAR"))
			cout << "Argument of CAR is wrong.";
		else if (re.what() == string("WA_CDR"))
			cout << "Argument of CDR is wrong.";
		else if (re.what() == string("WA_CO"))
			cout << "Arguments of comparison(Equal or LESS or GREATER) are wrong";
        else if(re.what() == string("WA_COND"))
            cout << "Arguments of COND are wrong.";
		else if (re.what() == string("WA_CONS"))
			cout << "Arguments of CONS are wrong.";
		else if (re.what() == string("WA_ATOM"))
			cout << "Argument of ATOM is wrong.";
		else if (re.what() == string("WA_AF"))
			cout << "Arguments of arithmetic functions are wrong.";
        else if(re.what() == string("WA_INT"))
            cout << "Argument of INT is wrong.";
		else if (re.what() == string("WA_NULL"))
			cout << "Argument of NULL is wrong.";
		else if (re.what() == string("WA_QUO"))
			cout << "Argument of QUOTE is wrong.";
		else if (re.what() == string("WT_AF"))
			cout << "Arguments of arithmetic functions should be integers";
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


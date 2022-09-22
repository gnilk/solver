#include "../src/expsolver.h"
#include <testinterface.h>
#include <vector>
#include <functional>
#include <math.h>
#include <string.h>

// test exports
extern "C" {
    int test_expsolver_functions(ITesting *t);
    int test_expsolver_variables(ITesting *t);
    int test_expsolver_booleans(ITesting *t);
    int test_expsolver_ifop(ITesting *t);
    int test_expsolver_simple(ITesting *t);
    int test_expsolver_lshift(ITesting *t);
    int test_expsolver_rshift(ITesting *t);
}


using namespace gnilk;

extern "C" {
	static double functionCallBack(void *pUser, const char *data, int args, double *arg, int *bOk_out);
	static double varCallBack(void *pUser, const char *data, int *bOk_out);
}

static double varCallBack(void *pUser, const char *data, int *bOk_out) 
{
//	printf("Var: %s\n",data);
	*bOk_out = 1;
	if (!strcmp(data,"t")) return 4;
	*bOk_out = 0;
	return 0;
}
static double functionCallBack(void *pUser, const char *data, int args, double *arg, int *bOk_out)
{
	double result = 0.0;
	*bOk_out = 0;

	// printf("Func: %s\n", data);
	// printf("Args: %d\n", args);

	// for(int i=0;i<args;i++) {
	// 	printf("%d:%f\n", i, arg[i]);
	// }


	if (!strcmp(data,"inc")) {
	    *bOk_out = 1;
	    if (args > 0) {
	    	result = 0;
	    	for(int i=0;i<args;i++) {
	    		result += arg[i];
	    	}
	    }
	}


	return result;
}

int test_expsolver_functions(ITesting *t) {

    // The inc function adds all expresions together see 'functionCallback'
    // inc(2+1, 2, inc(3+1),5) => 3+2+4+5 => 14
    // 14*3 = 42
	static const char *expression = "inc(2+1, 2, inc(3+1), 5)*3";

	ExpSolver exp(expression);
	exp.RegisterUserFunctionCallback(functionCallBack, NULL);
	if (!exp.Prepare()) {
        return kTR_Fail;
    }
	double val = exp.Evaluate();
    if (val != 42.0) {
        return kTR_Fail;
    }
	//printf("Value: %f\n",val);
		
    return kTR_Pass;
}

int test_expsolver_variables(ITesting *t) {
    // Variable callback returns 4 for 't'
    // result should be 5, the second part is ignored as there is no operator binding it
	ExpSolver exp("t+1 inc(1)");
	exp.RegisterUserFunctionCallback(functionCallBack, NULL);
	exp.RegisterUserVariableCallback(varCallBack, NULL);

	if (!exp.Prepare()) {
        return kTR_Fail;
    }
    double val = exp.Evaluate();
    if (val != 5.0) {
        return kTR_Fail;
    }

    return kTR_Pass;
}

int test_expsolver_booleans(ITesting *t) {
    double tmp;
	if (!ExpSolver::Solve(&tmp, "4>1")) {
        return kTR_Fail;
    }
    // The double should be '1' 
    if (tmp < 1.0) {
        return kTR_Fail;
    }

    return kTR_Pass;
}

int test_expsolver_ifop(ITesting *t) {
    double tmp;

	if (!ExpSolver::Solve(&tmp, "4<1?4*2+1:3*2+1")) {
        return kTR_Fail;
    }
    if (tmp != 7.0) {
        return kTR_Fail;
    }

    // Should not work
    printf("NOTE: Errors Expected\n");
	if (ExpSolver::Solve(&tmp, "4<1?3*2+1")) {
        return kTR_Fail;
	}
	if (ExpSolver::Solve(&tmp, "4<1?")) {
		return kTR_Fail;
	}

    return kTR_Pass;
}

int test_expsolver_simple(ITesting *t) {
	double tmp;
	if (!ExpSolver::Solve(&tmp, "-4+-1")) {
        return kTR_Fail;
    }
    if (tmp != -5.0) {
        return kTR_Fail;
    }
    return kTR_Pass;
}

int test_expsolver_lshift(ITesting *t) {
    double tmp;
    if (!ExpSolver::Solve(&tmp, "1<<4")) {
        return kTR_Fail;
    }
    TR_ASSERT(t, tmp == 1<<4);
    return kTR_Pass;
}

int test_expsolver_rshift(ITesting *t) {
    double tmp;
    if (!ExpSolver::Solve(&tmp, "8>>2")) {
        return kTR_Fail;
    }
    TR_ASSERT(t, tmp==2);
    return kTR_Pass;
}

// static void testExpSolver() {

// 	printf("Test simple expressions\n");

// 	double tmp;
// 	ExpSolver::Solve(&tmp, "-4+-1");
// 	printf("Value: %f\n",tmp);
// 	printf("boolean test\n");
// 	ExpSolver::Solve(&tmp, "4>1");
// 	printf("Bool: %f\n",tmp);
// 	printf("IF Operator\n");
// 	ExpSolver::Solve(&tmp, "4<1?4*2+1:3*2+1");
// 	printf("if: %f (7)\n",tmp);
// 	if (!ExpSolver::Solve(&tmp, "4<1?3*2+1")) {
// 		printf("Ok, should not work\n");
// 	}
// 	if (!ExpSolver::Solve(&tmp, "4<1?")) {
// 		printf("Ok, should not work\n");
// 	}
// }


#pragma once

#include "tokenizer.h"

namespace gnilk
{

	#ifdef WIN32
	#define CALLCONV __stdcall
	#else
	#define CALLCONV
	#endif

	#define EXP_SOLVER_MAX_ARGS 32


	extern "C"
	{
		typedef double (CALLCONV *PFNEVALUATE)(void *pUser, const char *data, int *bOk_out);
		typedef double (CALLCONV *PFNEVALUATEFUNC)(void *pUser, const char *data, int args, double *arg, int *bOk_out);
	}

	class BaseNode {
	public:
		virtual ~BaseNode() = default;
		virtual double Evaluate() = 0;
	};

	class ConstNode : public BaseNode {
	public:
		ConstNode(const char *input, bool negative);
		virtual ~ConstNode() = default;
		double Evaluate();
    protected:
        double numeric;
	};

	class ConstUserNode :public BaseNode {
	public:
		ConstUserNode(PFNEVALUATE func, void *pUser, const char *input);
		virtual ~ConstUserNode();
		double Evaluate();
    protected:
        void *pUser;
        const char *sData;
        PFNEVALUATE pCallback;
	};

	class FuncNode : public BaseNode {
	public:
		FuncNode(PFNEVALUATEFUNC func, void *pUser, const char *name, BaseNode *pArg);
		FuncNode(PFNEVALUATEFUNC func, void *pUser, const char *name, int args, BaseNode **pArg);
		virtual ~FuncNode();
		double Evaluate();
    protected:
        void *pUser;
        const char *sFuncName;
        PFNEVALUATEFUNC pCallback;
        int args;
        BaseNode *pArgument[EXP_SOLVER_MAX_ARGS];
	};

	class BinOpNode : public BaseNode {
	public:
		BinOpNode(const char *op, BaseNode *pLeft, BaseNode *pRight);
		virtual ~BinOpNode();
		double Evaluate();
    protected:
        const char *op;
        BaseNode *pLeft;
        BaseNode *pRight;
	};

	class BoolOpNode : public BinOpNode {
	public:
		BoolOpNode(const char *op, BaseNode *pLeft, BaseNode *pRight);
		virtual ~BoolOpNode();
		double Evaluate();
	};

	class IfOperatorNode : public BaseNode {
	public:
		IfOperatorNode(BaseNode *exp, BaseNode *pTrue, BaseNode *pFalse);
		virtual ~IfOperatorNode();
		double Evaluate();
    protected:
        BaseNode *exp;
        BaseNode *pTrue;
        BaseNode *pFalse;
	};

	class ExpSolver {
	public:
		explicit ExpSolver(const char *expression);
		virtual ~ExpSolver();
		void RegisterUserVariableCallback(PFNEVALUATE pFunc, void *pUser);
		void RegisterUserFunctionCallback(PFNEVALUATEFUNC pFunc, void *pUser);
		bool Prepare();
		double Evaluate();
        static bool Solve(double *out, const char *expression);
    protected:
        BaseNode *BuildUserCall();
        BaseNode *BuildTerm();
        BaseNode *BuildFact();
        BaseNode *BuildAddSub();
        BaseNode *BuildShift();
        BaseNode *BuildBool();
        BaseNode *BuildIf();
        BaseNode *BuildTree();
    protected:
        typedef enum
        {
            kTokenClass_Unknown,
            kTokenClass_Numeric,
            kTokenClass_Variable,
        } kTokenClass;
        kTokenClass ClassifyToken(const char *token);

        PFNEVALUATE pVariableCallback;
        PFNEVALUATEFUNC pFuncCallback;

        void *pVariableContext;
        void *pFunctionContext;

        Tokenizer *tokenizer;
        BaseNode *tree;


        std::vector<BaseNode *> nodes;

	};

}
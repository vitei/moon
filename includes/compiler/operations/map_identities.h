#ifndef COMPILER_OPERATIONS_MAP_IDENTITIES_H
#define COMPILER_OPERATIONS_MAP_IDENTITIES_H

#include <queue>
#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class MapIdentities : public Operation
	{
	public:
		static void run(tree::Program *program);

		void add(tree::Scope *scope);
		void add(tree::Node *scope, tree::Expressions *expressions);
		void add(tree::Node *scope, tree::Statements *statements);
		void addFunction(tree::Scope *scope);
		void addFunction(tree::Node *scope, tree::Expressions *expressions);
		void addFunction(tree::Node *scope, tree::Statements *statements);
		void process();

		virtual void setup(tree::Function *function);

		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Identity *identity);

	private:
		class ScopeList
		{
		public:
			ScopeList(tree::Node *_scope, tree::Expressions *_expressions) : scope(_scope), expressions(_expressions), statements(NULL) {}
			ScopeList(tree::Node *_scope, tree::Statements *_statements) : scope(_scope), expressions(NULL), statements(_statements) {}

			tree::Node *scope;
			tree::Expressions *expressions;
			tree::Statements *statements;
		};

		MapIdentities() {}

		tree::Node *mCurrentScope;
		std::queue<ScopeList> mVisitNext;
		std::queue<ScopeList> mVisitFunctions;
	};
}

#endif

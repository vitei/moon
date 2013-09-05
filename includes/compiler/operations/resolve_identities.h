#ifndef COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H
#define COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H

#include <map>
#include <queue>
#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class ResolveIdentities : public Restructure
	{
	public:
		static void run(tree::Program *program);

		void add(tree::Scope *parentScope, tree::Scope *scope);
		void process();

		virtual void dispatch(tree::Scope *scope);
		virtual void dispatch(tree::Function *function);

		virtual void visit(tree::Identity *identity);
		virtual void visit(tree::TypeDefinition *typeDefinition);
		virtual void visit(tree::Expression *expression);
		virtual void visit(tree::Assign *assign);

		virtual tree::Node *restructure(tree::Identifier *identifier);

		virtual bool doProcessChildren(tree::Assign *assign) { return false; }

	private:
		ResolveIdentities() : mCanCreateIdentifier(false) {}

		bool mCanCreateIdentifier;
		std::map< tree::Scope *, std::queue<tree::Scope *> > mVisitList;
	};
}

#endif

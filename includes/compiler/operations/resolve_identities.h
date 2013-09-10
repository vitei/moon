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
		static bool run(tree::Program *program);

		void add(tree::Scope *parentScope, tree::Scope *scope);
		void process();

		virtual void dispatch(tree::Scope *scope);
		virtual void dispatch(tree::Function *function);
		virtual void dispatch(tree::Import *import);
		virtual void dispatch(tree::UDT *udt);

		virtual void visit(tree::Identity *identity);
		virtual void visit(tree::TypeDefinition *typeDefinition);
		virtual void visit(tree::Expression *expression);
		virtual void visit(tree::Assign *assign);

		virtual tree::Node *restructure(tree::Identifier *identifier);

		virtual bool doProcessChildren(tree::Assign *assign) { return false; }

	private:
		ResolveIdentities() : mValidated(false), mCanCreateIdentifier(false), mCurrentUDT(NULL) {}

		bool mValidated;
		bool mCanCreateIdentifier;
		tree::UDT *mCurrentUDT;
		std::map< tree::Scope *, std::queue<tree::Scope *> > mVisitList;
	};
}

#endif

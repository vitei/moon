#ifndef COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H
#define COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H

#include <map>
#include <queue>
#include "compiler/behaviours/named_map.h"
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
		virtual void visit(tree::Assign *assign);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::ArrayAccess *arrayAccess);
		virtual void visit(tree::DirectAccess *directAccess);
		virtual void visit(tree::Member *member);

		virtual tree::Node *restructure(tree::Identifier *identifier);

		virtual bool doProcessChildren(tree::Assign *assign) { return false; }
		virtual bool doProcessChildren(tree::FunctionCall *functionCall) { return false; }
		virtual bool doProcessChildren(tree::ArrayAccess *arrayAccess) { return false; }
		virtual bool doProcessChildren(tree::DirectAccess *directAccess) { return false; }
		virtual bool doProcessChildren(tree::Member *member) { return false; }

	private:
		ResolveIdentities() : mValidated(false), mCanCreateIdentifier(false), mCanMapIdentifier(true), mCurrentMap(NULL) {}

		bool mValidated;
		bool mCanCreateIdentifier;
		bool mCanMapIdentifier;
		behaviour::NamedMap *mCurrentMap;
		std::map< tree::Scope *, std::queue<tree::Scope *> > mVisitList;
	};
}

#endif

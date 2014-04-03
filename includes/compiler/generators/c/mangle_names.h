#ifndef COMPILER_GENERATORS_C_MANGLE_NAMES_H
#define COMPILER_GENERATORS_C_MANGLE_NAMES_H

#include "common.h"

#include <string>
#include "compiler/operations.h"
#include "compiler/tree.h"

namespace generator
{
	namespace c
	{
		class MangleNames : public operation::Operation
		{
		public:
			static void run(tree::Program *program);

			virtual void visit(tree::Scope *scope);
			virtual void visit(tree::Program *program);
			virtual void visit(tree::Aggregate *aggregate);
			virtual void visit(tree::Use *use);
			virtual void visit(tree::Function *function);
			virtual void visit(tree::Method *method);

		private:
			MangleNames() : mProgram(nullptr), mAggregate(nullptr), mUse(nullptr), mType(nullptr), mFunction(nullptr) {}

			void dispatch(tree::Node *node);

			void mangle(tree::Node *node)
			{
#ifdef DEBUG
				node->printNode();
#endif
				ERROR("MangleNames::mangle can't process node...");
			}

			void mangle(tree::Identity *identity);
			void mangle(tree::Variable *variable);
			void mangle(tree::FunctionPrototype *functionPrototype);
			void mangle(tree::UDT *udt);

			tree::Program *mProgram;
			tree::Aggregate *mAggregate;
			tree::Use *mUse;
			tree::Type *mType;
			tree::Function *mFunction;
		};
	}
}

#endif

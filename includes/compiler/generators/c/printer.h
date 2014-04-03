#ifndef COMPILER_GENERATORS_C_PRINTER_H
#define COMPILER_GENERATORS_C_PRINTER_H

#include "common.h"

#include <ostream>
#include <string>
#include "compiler/tree.h"

namespace generator
{
	namespace c
	{
		class Printer final
		{
		public:
			Printer() : mOutput(nullptr), mProgram(nullptr), mDepth(0) {}

			void init(std::ostream &output, tree::Program *program)
			{
				mOutput = &output;
				mProgram = program;
				mStructName = "moon_" + program->getName() + "Data";
			}

			void dispatch(tree::Node *node);

			void output(tree::Scope *scope);
			void output(tree::Function *function);
			void output(tree::AnonymousScope *anonymousScope);
			void output(tree::Import *import);
			void output(tree::FunctionPrototype *functionPrototype);
			void output(tree::Identity *identity);
			void output(tree::Cast *cast);
			void output(tree::IfExpression *ifExpression);
			void output(tree::DirectAccess *directAccess);
			void output(tree::MessageAccess *messageAccess);
			void output(tree::ArrayAccess *arrayAccess);
			void output(tree::FunctionCall *functionCall);
			void output(tree::BoolLiteral *boolLiteral);
			void output(tree::IntLiteral *intLiteral);
			void output(tree::FloatLiteral *floatLiteral);
			void output(tree::StringLiteral *stringLiteral);
			void output(tree::Assign *assign);
			void output(tree::LogicalOr *logicalOr);
			void output(tree::LogicalAnd *logicalAnd);
			void output(tree::Or *orExpression);
			void output(tree::Xor *xorExpression);
			void output(tree::And *andExpression);
			void output(tree::Equal *equal);
			void output(tree::Unequal *unequal);
			void output(tree::LessThan *lessThan);
			void output(tree::LessEqual *lessEqual);
			void output(tree::GreaterThan *greaterThan);
			void output(tree::GreaterEqual *greaterEqual);
			void output(tree::Add *add);
			void output(tree::Subtract *subtract);
			void output(tree::Multiply *multiply);
			void output(tree::Divide *divide);
			void output(tree::Modulus *modulus);
			void output(tree::LogicalNot *logicalNot);
			void output(tree::Not *notExpression);
			void output(tree::Minus *minus);
			void output(tree::UDT *udt);
			void output(tree::This *th1s);

			void output(tree::Break *breakStatement);
			void output(tree::Execute *execute);
			void output(tree::If *ifStatement);
			void output(tree::For *forStatement);
			void output(tree::While *whileStatement);
			void output(tree::Return *returnStatement);

			void output(tree::Node *node)
			{
#ifdef DEBUG
				node->printNode();
				ERROR("Should never reach here");
#endif
			}

			void outputEOS();
			void outputExtern(tree::TypedIdentity *typedIdentity);
			void outputDeclaration(tree::TypedIdentity *typedIdentity, bool functionPrototype = false);
			void outputType(tree::Type *type);
			void outputDimensions(tree::Type *type);

			void outputVariablesBegin();
			void outputVariablesEnd();

			void outputNewBegin();
			void outputNewEnd();

			void outputBootstrapMain();

			void outputPragma(std::string pragma);

			void outputRaw(std::string data);
			void outputTabs();

			void increaseDepth()
			{
				mDepth++;
			}

			void decreaseDepth()
			{
				mDepth--;
			}

		private:
			std::ostream *mOutput;
			tree::Program *mProgram;
			std::string mStructName;
			unsigned int mDepth;
		};
	}
}

#endif

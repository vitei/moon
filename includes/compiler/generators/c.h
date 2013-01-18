#ifndef COMPILER_GENERATORS_C_H
#define COMPILER_GENERATORS_C_H

#include <ostream>
#include <string>
#include "compiler/tree.h"
#include "generator.h"


namespace generator
{
	class C : public Generator
	{
	public:
		virtual void run(std::ostream &output, tree::Program *program);

		class Printer
		{
		public:
			Printer() : mOutput(NULL), mDepth(0) {}

			void init(std::ostream &output, tree::Program *program)
			{
				mOutput = &output;
				mProgram = program;
				mStructName = "moon_" + program->getName() + "Data";
			}

			void dispatch(tree::Node *node);

			void output(tree::Scope *scope);
			void output(tree::Function *function);
			void output(tree::FunctionPrototype *functionPrototype);
			void output(tree::Identity *identity);
			void output(tree::Reference *reference);
			void output(tree::Cast *cast);
			void output(tree::DirectAccess *directAccess);
			void output(tree::MessageAccess *messageAccess);
			void output(tree::ArrayAccess *arrayAccess);
			void output(tree::FunctionCall *functionCall);
			void output(tree::NullReference *nullReference);
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

			void output(tree::Execute *execute);
			void output(tree::Return *returnExpression);

			void output(tree::Node *node)
			{
#ifdef DEBUG
				node->printNode();
				ERROR("Should never reach here");
#endif
			}

			void outputEOS();
			void outputExtern(tree::TypedIdentity *typedIdentity);
			void outputDeclaration(tree::TypedIdentity *typedIdentity);

			void outputVariablesBegin();
			void outputVariablesEnd();

			void outputNewBegin();
			void outputNewEnd();

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

	private:
		virtual void generate(tree::Program *program);

		void mangleNames(tree::Program *program);
		void outputConstants(tree::Program *program);
		void outputVariables(tree::Program *program);
		void outputFunctions(tree::Program *program);
		void outputNew(tree::Program *program);

		Printer mPrinter;
	};
}

#endif

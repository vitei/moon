#ifndef COMPILER_GENERATORS_C_H
#define COMPILER_GENERATORS_C_H

#include "compiler/tree.h"
#include "generator.h"


namespace generator
{
	class C : public Generator
	{
	public:
		C() {}

		virtual void run(std::ostream &output, tree::Program *program);

		virtual void dispatch(tree::Node *node);

		virtual void generate(tree::Node *node)
		{
#ifdef DEBUG
			node->printNode();
		//	ERROR("Should never reach here");
#endif
		}

		virtual void generate(tree::Scope *scope);
		virtual void generate(tree::Program *program);
		virtual void generate(tree::Function *function);

		virtual void generate(tree::Identity *identity);
		virtual void generate(tree::Reference *reference);
		virtual void generate(tree::Cast *cast);
		virtual void generate(tree::DirectAccess *directAccess);
		virtual void generate(tree::MessageAccess *messageAccess);
		virtual void generate(tree::ArrayAccess *arrayAccess);
		virtual void generate(tree::FunctionCall *functionCall);
		virtual void generate(tree::NullReference *nullReference);
		virtual void generate(tree::BoolLiteral *boolLiteral);
		virtual void generate(tree::IntLiteral *intLiteral);
		virtual void generate(tree::FloatLiteral *floatLiteral);
		virtual void generate(tree::StringLiteral *stringLiteral);
		virtual void generate(tree::Assign *assign);
		virtual void generate(tree::LogicalOr *logicalOr);
		virtual void generate(tree::LogicalAnd *logicalAnd);
		virtual void generate(tree::Or *orExpression);
		virtual void generate(tree::Xor *xorExpression);
		virtual void generate(tree::And *andExpression);
		virtual void generate(tree::Equal *equal);
		virtual void generate(tree::Unequal *unequal);
		virtual void generate(tree::LessThan *lessThan);
		virtual void generate(tree::LessEqual *lessEqual);
		virtual void generate(tree::GreaterThan *greaterThan);
		virtual void generate(tree::GreaterEqual *greaterEqual);
		virtual void generate(tree::Add *add);
		virtual void generate(tree::Subtract *subtract);
		virtual void generate(tree::Multiply *multiply);
		virtual void generate(tree::Divide *divide);
		virtual void generate(tree::Modulus *modulus);
		virtual void generate(tree::LogicalNot *logicalNot);
		virtual void generate(tree::Not *notExpression);
		virtual void generate(tree::Minus *minus);

		virtual void generate(tree::Execute *execute);
		virtual void generate(tree::Return *returnExpression);

		void outputDeclaration(tree::Identity *identity);

	private:
		std::ostream *mOutput;
	};
}

#endif

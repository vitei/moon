#ifndef COMPILER_TREE_EXPRESSION_H
#define COMPILER_TREE_EXPRESSION_H

#include <list>
#include <string>
#include "node.h"
#include "type.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Expression : public Node
	{
	public:
		class InvalidException : public std::exception
		{
		public:
			InvalidException(Expression *_expression) : expression(_expression) {}

			virtual void reset() = 0;

			Expression *expression;
		};

		Type *getType()
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Node::childAccept(operation);

			if(mType)
			{
				mType->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	protected:
		Expression() : mType(NULL) { /* Abstract class */ }

	private:
		Type *mType;
	};

	typedef std::list<Expression *> Expressions;

	class Identity : public Expression
	{
	public:
		const std::string &getName()
		{
			return mName;
		}

		void setName(std::string &name)
		{
			mName = name;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	protected:
		Identity(std::string name) : mName(name) { /* Abstract class */ }

	private:
		std::string mName;
	};

	class TypedIdentity : public Identity
	{
	protected:
		TypedIdentity(Type *type, std::string name) : Identity(name)
		{
			/* Abstract class */
			setType(type);
		}
	};

	class Access : public Expression
	{
	public:
		class InvalidException : public tree::Expression::InvalidException
		{
		public:
			InvalidException(Expression *_expression, Access *_access) : tree::Expression::InvalidException(_expression), access(_access) {}

			Access *access;
		};

		class InvalidContainerException : public tree::Access::InvalidException
		{
		public:
			InvalidContainerException(Access *_access) : tree::Access::InvalidException(_access->getContainer(), _access) {}

			virtual void reset()
			{
				LOG("tree::Access::InvalidContainerException::reset");
				access->setContainer(NULL);
			}
		};

		class InvalidTargetException : public tree::Access::InvalidException
		{
		public:
			InvalidTargetException(Access *_access) : tree::Access::InvalidException(_access->getTarget(), _access) {}

			virtual void reset()
			{
				LOG("tree::Access::InvalidTargetException::reset");
				access->setTarget(NULL);
			}
		};

		Expression *getContainer()
		{
			return mContainer;
		}

		void setContainer(Expression *container);

		Expression *getTarget()
		{
			return mTarget;
		}

		void setTarget(Expression *target);

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mContainer)
			{
				mContainer->accept(operation);
			}

			if(mTarget)
			{
				mTarget->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	protected:
		Access(Expression *container, Expression *target) : mContainer(container), mTarget(target) { /* Abstract class */ }

	private:
		Expression *mContainer;
		Expression *mTarget;
	};

	class Literal : public Expression
	{
	protected:
		Literal(Type *type)
		{
			/* Abstract class */
			setType(type);
		}
	};

	class UnaryExpression : public Expression
	{
	public:
		class InvalidException : public tree::Expression::InvalidException
		{
		public:
			InvalidException(UnaryExpression *_unaryExpression) : tree::Expression::InvalidException(_unaryExpression->getExpression()), unaryExpression(_unaryExpression) {}

			virtual void reset()
			{
				LOG("tree::UnaryExpression::InvalidException::reset");
				unaryExpression->setExpression(NULL);
			}

			UnaryExpression *unaryExpression;
		};

		Expression *getExpression()
		{
			return mExpression;
		}

		void setExpression(Expression *expression);

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mExpression)
			{
				mExpression->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	protected:
		UnaryExpression(Expression *expression) : mExpression(expression) { /* Abstract class */ }

	private:
		Expression *mExpression;
	};

	class BinaryExpression : public Expression
	{
	public:
		class InvalidException : public tree::Expression::InvalidException
		{
		public:
			InvalidException(Expression *_expression, BinaryExpression *_binaryExpression) : tree::Expression::InvalidException(_expression), binaryExpression(_binaryExpression) {}

			BinaryExpression *binaryExpression;
		};

		class InvalidLHSException : public tree::BinaryExpression::InvalidException
		{
		public:
			InvalidLHSException(BinaryExpression *_binaryExpression) : tree::BinaryExpression::InvalidException(_binaryExpression->getLHS(), _binaryExpression) {}

			virtual void reset()
			{
				LOG("tree::BinaryExpression::InvalidLHSException::reset");
				binaryExpression->setLHS(NULL);
			}
		};

		class InvalidRHSException : public tree::BinaryExpression::InvalidException
		{
		public:
			InvalidRHSException(BinaryExpression *_binaryExpression) : tree::BinaryExpression::InvalidException(_binaryExpression->getRHS(), _binaryExpression) {}

			virtual void reset()
			{
				LOG("tree::BinaryExpression::InvalidRHSException::reset");
				binaryExpression->setRHS(NULL);
			}
		};

		Expression *getLHS()
		{
			return mLHS;
		}

		void setLHS(Expression *lhs);

		Expression *getRHS()
		{
			return mRHS;
		}

		void setRHS(Expression *rhs);

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mLHS)
			{
				mLHS->accept(operation);
			}

			if(mRHS)
			{
				mRHS->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	protected:
		BinaryExpression(Expression *lhs, Expression *rhs) : mLHS(lhs), mRHS(rhs) { /* Abstract class */ }

	private:
		Expression *mLHS;
		Expression *mRHS;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Identifier : public Expression
	{
	public:
		Identifier(std::string name) : mName(name) {}

		const std::string &getName()
		{
			return mName;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Identifier"); }
#endif

	private:
		std::string mName;
	};

	class Constant : public TypedIdentity
	{
	public:
		Constant(Type *type, std::string name) : TypedIdentity(type, name) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Constant"); }
#endif
	};

	class Variable : public TypedIdentity
	{
	public:
		Variable(Type *type, std::string name) : TypedIdentity(type, name) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Variable"); }
#endif
	};

	class Reference : public TypedIdentity
	{
	public:
		Reference(Type *type, std::string name) : TypedIdentity(type, name) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Reference"); }
#endif
	};

	class Cast : public Expression
	{
	public:
		Cast(Type *type, Expression *expression) : mExpression(expression)
		{
			setType(type);
		}

		Expression *getExpression()
		{
			return mExpression;
		}

		void setExpression(Expression *expression)
		{
			mExpression = expression;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mExpression)
			{
				mExpression->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Cast"); }
#endif

	private:
		Expression *mExpression;
	};

	class DirectAccess : public Access
	{
	public:
		DirectAccess(Expression *container, Expression *target) : Access(container, target) {}

#ifdef DEBUG
		virtual void printNode() { LOG("DirectAccess"); }
#endif
	};

	class MessageAccess : public Access
	{
	public:
		MessageAccess(Expression *container, Expression *target) : Access(container, target) {}

#ifdef DEBUG
		virtual void printNode() { LOG("MessageAccess"); }
#endif
	};

	class ArrayAccess : public Access
	{
	public:
		ArrayAccess(Expression *array, Expression *index) : Access(array, index) {}

#ifdef DEBUG
		virtual void printNode() { LOG("ArrayAccess"); }
#endif
	};

	class FunctionPrototype;

	class FunctionCall : public Expression
	{
	public:
		class InvalidException : public tree::Expression::InvalidException
		{
		public:
			InvalidException(Expression *_expression, FunctionCall *_functionCall) : tree::Expression::InvalidException(_expression), functionCall(_functionCall) {}

			FunctionCall *functionCall;
		};

		class InvalidFunctionException : public tree::FunctionCall::InvalidException
		{
		public:
			InvalidFunctionException(FunctionCall *_functionCall) : tree::FunctionCall::InvalidException(_functionCall->getFunctionPrototype(), _functionCall) {}

			virtual void reset()
			{
				LOG("tree::FunctionCall::InvalidFunctionException::reset");
				functionCall->setFunctionPrototype(NULL);
			}
		};

		class InvalidArgumentsException : public tree::FunctionCall::InvalidException
		{
		public:
			InvalidArgumentsException(FunctionCall *_functionCall) : tree::FunctionCall::InvalidException(_functionCall->getFunctionPrototype(), _functionCall) {}

			virtual void reset()
			{
				LOG("tree::FunctionCall::InvalidArgumentsException::reset");
				functionCall->setFunctionPrototype(NULL);
			}
		};

		FunctionCall(Identifier *functionPrototype, Expressions *arguments = NULL) : mFunctionPrototype(functionPrototype), mArguments(arguments) {}

		Expression *getFunctionPrototype()
		{
			return mFunctionPrototype;
		}

		void setFunctionPrototype(Expression *functionPrototype);

		Expressions *getArguments()
		{
			return mArguments;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mFunctionPrototype)
			{
				mFunctionPrototype->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

#ifdef DEBUG
		virtual void printNode() { LOG("FunctionCall"); }
#endif

	private:
		Expression *mFunctionPrototype;
		Expressions *mArguments;
	};

	class NullReference : public Expression
	{
	public:
#ifdef DEBUG
		virtual void printNode() { LOG("NullReference"); }
#endif
	};

	class BoolLiteral : public Literal
	{
	public:
		BoolLiteral(bool value) : Literal(new Bool()), mValue(value) {}

		bool getValue()
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("BoolLiteral"); }
#endif

	private:
		bool mValue;
	};

	class IntLiteral : public Literal
	{
	public:
		IntLiteral(int value) : Literal(new Int()), mValue(value) {}

		int getValue()
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("IntLiteral"); }
#endif

	private:
		int mValue;
	};

	class FloatLiteral : public Literal
	{
	public:
		FloatLiteral(float value) : Literal(new Float()), mValue(value) {}

		float getValue()
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("FloatLiteral"); }
#endif

	private:
		float mValue;
	};

	class StringLiteral : public Literal
	{
	public:
		StringLiteral(std::string value) : Literal(new String(value.length())), mValue(value) {}

		std::string getValue()
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("StringLiteral"); }
#endif

	private:
		std::string mValue;
	};

	class Assign : public BinaryExpression
	{
	public:
		Assign(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Assign"); }
#endif
	};

	class LogicalOr : public BinaryExpression
	{
	public:
		LogicalOr(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("LogicalOr"); }
#endif
	};

	class LogicalAnd : public BinaryExpression
	{
	public:
		LogicalAnd(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("LogicalAnd"); }
#endif
	};

	class Or : public BinaryExpression
	{
	public:
		Or(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Or"); }
#endif
	};

	class Xor : public BinaryExpression
	{
	public:
		Xor(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Xor"); }
#endif
	};

	class And : public BinaryExpression
	{
	public:
		And(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("And"); }
#endif
	};

	class Equal : public BinaryExpression
	{
	public:
		Equal(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Equal"); }
#endif
	};

	class Unequal : public BinaryExpression
	{
	public:
		Unequal(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Unequal"); }
#endif
	};

	class LessThan : public BinaryExpression
	{
	public:
		LessThan(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("LessThan"); }
#endif
	};

	class LessEqual : public BinaryExpression
	{
	public:
		LessEqual(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("LessEqual"); }
#endif
	};

	class GreaterThan : public BinaryExpression
	{
	public:
		GreaterThan(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("GreaterThan"); }
#endif
	};

	class GreaterEqual : public BinaryExpression
	{
	public:
		GreaterEqual(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("GreaterEqual"); }
#endif
	};

	class Add : public BinaryExpression
	{
	public:
		Add(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Add"); }
#endif
	};

	class Subtract : public BinaryExpression
	{
	public:
		Subtract(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Subtract"); }
#endif
	};

	class Multiply : public BinaryExpression
	{
	public:
		Multiply(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Multiply"); }
#endif
	};

	class Divide : public BinaryExpression
	{
	public:
		Divide(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Divide"); }
#endif
	};

	class Modulus : public BinaryExpression
	{
	public:
		Modulus(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Modulus"); }
#endif
	};

	class LogicalNot : public UnaryExpression
	{
	public:
		LogicalNot(Expression *expression) : UnaryExpression(expression) {}

#ifdef DEBUG
		virtual void printNode() { LOG("LogicalNot"); }
#endif
	};

	class Not : public UnaryExpression
	{
	public:
		Not(Expression *expression) : UnaryExpression(expression) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Not"); }
#endif
	};

	class Minus : public UnaryExpression
	{
	public:
		Minus(Expression *expression) : UnaryExpression(expression) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Minus"); }
#endif
	};
}

#endif

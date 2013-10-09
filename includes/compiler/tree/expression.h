#ifndef COMPILER_TREE_EXPRESSION_H
#define COMPILER_TREE_EXPRESSION_H

#include "common.h"

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
			virtual void reset() = 0;

			Expression *expression;

		protected:
			InvalidException(Expression *_expression) : expression(_expression) { /* Abstract class */ }
		};

		class InvalidTypeException final : public InvalidException
		{
		public:
			InvalidTypeException(Expression *_expression) : InvalidException(_expression) {}

			virtual void reset()
			{
				LOG("tree::Expression::InvalidTypeException::reset");
				expression->setType(nullptr);
			}
		};

		Type *getType() const
		{
			return mType;
		}

		virtual void setType(Type *type);

		virtual void childAccept(operation::Operation *operation)
		{
			Node::childAccept(operation);

			if(mType)
			{
				mType->accept(operation);
			}
		}

	protected:
		Expression() : mType(nullptr) { /* Abstract class */ }

	private:
		Type *mType;
	};

	typedef std::list<Expression *> Expressions;

	class Identity : public Expression
	{
	public:
		virtual const std::string &getName() const
		{
			return mName;
		}

		void setName(std::string &name)
		{
			mName = name;
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
			Access *access;

		protected:
			InvalidException(Expression *_expression, Access *_access) : tree::Expression::InvalidException(_expression), access(_access) { /* Abstract class */ }
		};

		class InvalidContainerException final : public tree::Access::InvalidException
		{
		public:
			InvalidContainerException(Access *_access) : tree::Access::InvalidException(_access->getContainer(), _access) {}

			virtual void reset()
			{
				LOG("tree::Access::InvalidContainerException::reset");
				access->setContainer(nullptr);
			}
		};

		class InvalidTargetException final : public tree::Access::InvalidException
		{
		public:
			InvalidTargetException(Access *_access) : tree::Access::InvalidException(_access->getTarget(), _access) {}

			virtual void reset()
			{
				LOG("tree::Access::InvalidTargetException::reset");
				access->setTarget(nullptr);
			}
		};

		Expression *getContainer() const
		{
			return mContainer;
		}

		void setContainer(Expression *container);

		Expression *getTarget() const
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

	protected:
		Access(Expression *container, Expression *target) : mContainer(container), mTarget(target) { /* Abstract class */ }

	private:
		Expression *mContainer;
		Expression *mTarget;
	};

	class Literal : public Expression
	{
	public:
		class InvalidException final : public tree::Expression::InvalidException
		{
		public:
			InvalidException(Literal *_literal) : tree::Expression::InvalidException(_literal) {}

			virtual void reset() {}
		};

	protected:
		Literal(Type *type)
		{
			/* Abstract class */
			setType(type);
		}

		virtual bool equals(const Literal &literal) const = 0;

	private:
		friend bool operator == (const Literal &literal1, const Literal &literal2);
	};

	inline bool operator == (const Literal &literal1, const Literal &literal2)
	{
		return literal1.equals(literal2);
	}

	inline bool operator != (const Literal &literal1, const Literal &literal2)
	{
		return !(literal1 == literal2);
	}

	class NumericLiteral : public Literal
	{
	protected:
		NumericLiteral(Type *type) : Literal(type) { /* Abstract class */ }

		virtual bool lessThan(const NumericLiteral &numericLiteral) const = 0;

	private:
		friend bool operator < (const NumericLiteral &numericLiteral1, const NumericLiteral &numericLiteral2);
	};

	inline bool operator < (const NumericLiteral &numericLiteral1, const NumericLiteral &numericLiteral2)
	{
		return numericLiteral1.lessThan(numericLiteral2);
	}

	inline bool operator <= (const NumericLiteral &numericLiteral1, const NumericLiteral &numericLiteral2)
	{
		return numericLiteral1 < numericLiteral2 || numericLiteral1 == numericLiteral2;
	}

	inline bool operator > (const NumericLiteral &numericLiteral1, const NumericLiteral &numericLiteral2)
	{
		return !(numericLiteral1 < numericLiteral2);
	}

	inline bool operator >= (const NumericLiteral &numericLiteral1, const NumericLiteral &numericLiteral2)
	{
		return numericLiteral1 > numericLiteral2 || numericLiteral1 == numericLiteral2;
	}

	class Operation : public Expression
	{
	public:
		class NotAllowedException final : public tree::Expression::InvalidException
		{
		public:
			NotAllowedException(Operation *_operation) : tree::Expression::InvalidException(_operation) {}

			virtual void reset()
			{
				LOG("tree::Operation::NotAllowedException::reset");
				expression->setType(nullptr);
			}
		};

		virtual void setType(Type *type);

		virtual Literal *calculate() const = 0;
	};

	class UnaryOperation : public Operation
	{
	public:
		class InvalidException final : public tree::Expression::InvalidException
		{
		public:
			InvalidException(UnaryOperation *_unaryOperation) : tree::Expression::InvalidException(_unaryOperation->getExpression()), unaryOperation(_unaryOperation) {}

			virtual void reset()
			{
				LOG("tree::UnaryOperation::InvalidException::reset");
				unaryOperation->setExpression(nullptr);
			}

			UnaryOperation *unaryOperation;
		};

		Expression *getExpression() const
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

	protected:
		UnaryOperation(Expression *expression) : mExpression(expression) { /* Abstract class */ }

	private:
		Expression *mExpression;
	};

	class BooleanUnaryOperation : public UnaryOperation
	{
	protected:
		BooleanUnaryOperation(Expression *expression) : UnaryOperation(expression)
		{
			/* Abstract class */
			setType(new Bool());
		}
	};

	class BinaryOperation : public Operation
	{
	public:
		class InvalidException : public tree::Expression::InvalidException
		{
		public:
			BinaryOperation *binaryOperation;

		protected:
			InvalidException(Expression *_expression, BinaryOperation *_binaryOperation) : tree::Expression::InvalidException(_expression), binaryOperation(_binaryOperation) { /* Abstract class */ }
		};

		class InvalidLHSException final : public tree::BinaryOperation::InvalidException
		{
		public:
			InvalidLHSException(BinaryOperation *_binaryOperation) : tree::BinaryOperation::InvalidException(_binaryOperation->getLHS(), _binaryOperation) {}

			virtual void reset()
			{
				LOG("tree::BinaryOperation::InvalidLHSException::reset");
				binaryOperation->setLHS(nullptr);
			}
		};

		class InvalidRHSException final : public tree::BinaryOperation::InvalidException
		{
		public:
			InvalidRHSException(BinaryOperation *_binaryOperation) : tree::BinaryOperation::InvalidException(_binaryOperation->getRHS(), _binaryOperation) {}

			virtual void reset()
			{
				LOG("tree::BinaryOperation::InvalidRHSException::reset");
				binaryOperation->setRHS(nullptr);
			}
		};

		Expression *getLHS() const
		{
			return mLHS;
		}

		void setLHS(Expression *lhs);

		Expression *getRHS() const
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

	protected:
		BinaryOperation(Expression *lhs, Expression *rhs) : mLHS(lhs), mRHS(rhs) { /* Abstract class */ }

		Expression *mLHS;
		Expression *mRHS;
	};

	class BooleanBinaryOperation : public BinaryOperation
	{
	protected:
		BooleanBinaryOperation(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs)
		{
			/* Abstract class */
			setType(new Bool());
		}
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Constant final : public TypedIdentity
	{
	public:
		Constant(Type *type, std::string name) : TypedIdentity(type, name), mValue(nullptr) {}

		Literal *getValue() const
		{
			return mValue;
		}

		void setValue(Literal *value)
		{
			mValue = value;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Constant: %s", getName().c_str()); }
#endif

	private:
		Literal *mValue;
	};

	class Variable final : public TypedIdentity
	{
	public:
		Variable(Type *type, std::string name) : TypedIdentity(type, name) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Variable: %s", getName().c_str()); }
#endif
	};

	class Cast final : public Expression
	{
	public:
		class InvalidException final : public tree::Expression::InvalidException
		{
		public:
			InvalidException(Expression *_expression) : tree::Expression::InvalidException(_expression) {}

			virtual void reset()
			{
				LOG("tree::Cast::InvalidException::reset");
				//binaryExpression->setRHS(nullptr);
			}
		};

		Cast(Type *type, Expression *expression, bool autoCast = false) : mExpression(expression), mAutoCast(autoCast)
		{
			setType(type);
		}

		void checkCast();

		Expression *getExpression() const
		{
			return mExpression;
		}

		void setExpression(Expression *expression)
		{
			mExpression = expression;
			//checkCast();
		}

		bool getAutoCast() const
		{
			return mAutoCast;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mExpression)
			{
				mExpression->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Cast"); }
#endif

	private:
		Expression *mExpression;
		bool mAutoCast;
	};

	class ComputedArray final : public Expression
	{
	public:
		ComputedArray(tree::Expression *from, tree::Expression *to, tree::Expression *step = nullptr) : mFrom(from), mTo(to), mStep(step) {}

		Expression *getFrom() const
		{
			return mFrom;
		}

		void setFrom(Expression *from)
		{
			mFrom = from;
		}

		Expression *getTo() const
		{
			return mTo;
		}

		void setTo(Expression *to)
		{
			mTo = to;
		}

		Expression *getStep() const
		{
			return mStep;
		}

		void setStep(Expression *step)
		{
			mStep = step;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mFrom)
			{
				mFrom->accept(operation);
			}

			if(mTo)
			{
				mTo->accept(operation);
			}

			if(mStep)
			{
				mStep->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("ComputedArray"); }
#endif

	private:
		tree::Expression *mFrom;
		tree::Expression *mTo;
		tree::Expression *mStep;
	};

	class This final : public Expression
	{
	public:
		This() {}

#ifdef DEBUG
		virtual void printNode() { LOG("This"); }
#endif
	};

	class DirectAccess final : public Access
	{
	public:
		DirectAccess(Expression *container, Expression *target) : Access(container, target) {}

#ifdef DEBUG
		virtual void printNode() { LOG("DirectAccess"); }
#endif
	};

	class MessageAccess final : public Access
	{
	public:
		MessageAccess(Expression *container, Expression *target) : Access(container, target) {}

#ifdef DEBUG
		virtual void printNode() { LOG("MessageAccess"); }
#endif
	};

	class ArrayAccess final : public Access
	{
	public:
		ArrayAccess(Expression *array, Expression *index) : Access(array, index) {}

#ifdef DEBUG
		virtual void printNode() { LOG("ArrayAccess"); }
#endif
	};

	class FunctionPrototype final : public TypedIdentity
	{
	public:
		class InvalidException : public std::exception
		{
		public:
			FunctionPrototype *functionPrototype;

		protected:
			InvalidException(FunctionPrototype *_functionPrototype) : functionPrototype(_functionPrototype) { /* Abstract class */ }
		};

		class InvalidTargetException final : public tree::FunctionPrototype::InvalidException
		{
		public:
			InvalidTargetException(FunctionPrototype *_functionPrototype) : tree::FunctionPrototype::InvalidException(_functionPrototype) {}

			virtual void reset()
			{
				LOG("tree::FunctionPrototype::InvalidTargetException::reset");
				functionPrototype->setTarget(nullptr);
			}
		};

		FunctionPrototype(Type *type, std::string name, Expressions *arguments) : TypedIdentity(type, name), mArguments(arguments), mTarget(nullptr) {}

		Expressions *getArguments() const
		{
			return mArguments;
		}

		Node *getTarget() const
		{
			return mTarget;
		}

		void setTarget(Node *target);

#ifdef DEBUG
		virtual void printNode() { LOG("FunctionPrototype: %s", getName().c_str()); }
#endif

	private:
		Expressions *mArguments;

		Node *mTarget;
	};

	class FunctionCall final : public Expression
	{
	public:
		class InvalidException : public tree::Expression::InvalidException
		{
		public:

			FunctionCall *functionCall;

		protected:
			InvalidException(Expression *_expression, FunctionCall *_functionCall) : tree::Expression::InvalidException(_expression), functionCall(_functionCall) { /* Abstract class */ }
		};

		class InvalidFunctionException final : public tree::FunctionCall::InvalidException
		{
		public:
			InvalidFunctionException(FunctionCall *_functionCall) : tree::FunctionCall::InvalidException(_functionCall->getPrototype(), _functionCall) {}

			virtual void reset()
			{
				LOG("tree::FunctionCall::InvalidFunctionException::reset");
				functionCall->setPrototype(nullptr);
			}
		};

		class InvalidArgumentsException final : public tree::FunctionCall::InvalidException
		{
		public:
			InvalidArgumentsException(FunctionCall *_functionCall) : tree::FunctionCall::InvalidException(_functionCall->getPrototype(), _functionCall) {}

			virtual void reset()
			{
				LOG("tree::FunctionCall::InvalidArgumentsException::reset");
				functionCall->setPrototype(nullptr);
			}
		};

		FunctionCall(FunctionPrototype *functionPrototype, Expressions *arguments = nullptr) : mPrototype(functionPrototype), mArguments(arguments) {}

		FunctionPrototype *getPrototype() const
		{
			return mPrototype;
		}

		void setPrototype(FunctionPrototype *prototype);

		Expressions *getArguments() const
		{
			return mArguments;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mPrototype)
			{
				mPrototype->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("FunctionCall"); }
#endif

	private:
		FunctionPrototype *mPrototype;
		Expressions *mArguments;
	};

	class None final : public Literal
	{
	public:
		None() : Literal(new Void()) {}

#ifdef DEBUG
		virtual void printNode() { LOG("None"); }
#endif

	protected:
		virtual bool equals(const Literal &literal) const;
	};

	class BoolLiteral final : public Literal
	{
	public:
		BoolLiteral(bool value) : Literal(new Bool()), mValue(value) {}
		BoolLiteral(Literal *literal, tree::Bool *type = nullptr);

		bool getValue() const
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("BoolLiteral"); }
#endif

	protected:
		virtual bool equals(const Literal &literal) const;

	private:
		bool mValue;
	};

	class IntLiteral final : public NumericLiteral
	{
	public:
		IntLiteral(int value) : NumericLiteral(new Int()), mValue(value) {}
		IntLiteral(Literal *literal, tree::Int *type = nullptr);

		int getValue() const
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("IntLiteral"); }
#endif

	protected:
		virtual bool lessThan(const NumericLiteral &numericLiteral) const;
		virtual bool equals(const Literal &literal) const;

	private:
		int mValue;
	};

	class FloatLiteral final : public NumericLiteral
	{
	public:
		FloatLiteral(float value) : NumericLiteral(new Float()), mValue(value) {}
		FloatLiteral(Literal *literal, tree::Float *type = nullptr);

		float getValue() const
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("FloatLiteral"); }
#endif

	protected:
		virtual bool lessThan(const NumericLiteral &numericLiteral) const;
		virtual bool equals(const Literal &literal) const;

	private:
		float mValue;
	};

	class StringLiteral final : public Literal
	{
	public:
		StringLiteral(std::string value) : Literal(new String(new IntLiteral(value.length()))), mValue(value) {}
		StringLiteral(Literal *literal, tree::String *type = nullptr);

		const std::string getValue() const
		{
			return mValue;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("StringLiteral"); }
#endif

	protected:
		virtual bool equals(const Literal &literal) const;

	private:
		std::string mValue;
	};

	class Assign final : public BinaryOperation
	{
	public:
		Assign(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const
		{
			ERROR("Should not be called"); // FIXME, this is probably better as a special binary operation that CAN calculate??
			return nullptr;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mRHS)
			{
				mRHS->accept(operation);
			}

			if(mLHS)
			{
				mLHS->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Assign"); }
#endif
	};

	class LogicalOr final : public BooleanBinaryOperation
	{
	public:
		LogicalOr(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("LogicalOr"); }
#endif
	};

	class LogicalAnd final : public BooleanBinaryOperation
	{
	public:
		LogicalAnd(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("LogicalAnd"); }
#endif
	};

	class Or final : public BinaryOperation
	{
	public:
		Or(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Or"); }
#endif
	};

	class Xor final : public BinaryOperation
	{
	public:
		Xor(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Xor"); }
#endif
	};

	class And final : public BinaryOperation
	{
	public:
		And(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("And"); }
#endif
	};

	class Equal final : public BooleanBinaryOperation
	{
	public:
		Equal(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Equal"); }
#endif
	};

	class Unequal final : public BooleanBinaryOperation
	{
	public:
		Unequal(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Unequal"); }
#endif
	};

	class LessThan final : public BooleanBinaryOperation
	{
	public:
		LessThan(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("LessThan"); }
#endif
	};

	class LessEqual final : public BooleanBinaryOperation
	{
	public:
		LessEqual(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("LessEqual"); }
#endif
	};

	class GreaterThan final : public BooleanBinaryOperation
	{
	public:
		GreaterThan(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("GreaterThan"); }
#endif
	};

	class GreaterEqual final : public BooleanBinaryOperation
	{
	public:
		GreaterEqual(Expression *lhs, Expression *rhs) : BooleanBinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("GreaterEqual"); }
#endif
	};

	class Add final : public BinaryOperation
	{
	public:
		Add(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Add"); }
#endif
	};

	class Subtract final : public BinaryOperation
	{
	public:
		Subtract(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Subtract"); }
#endif
	};

	class Multiply final : public BinaryOperation
	{
	public:
		Multiply(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Multiply"); }
#endif
	};

	class Divide final : public BinaryOperation
	{
	public:
		Divide(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Divide"); }
#endif
	};

	class Modulus final : public BinaryOperation
	{
	public:
		Modulus(Expression *lhs, Expression *rhs) : BinaryOperation(lhs, rhs) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Modulus"); }
#endif
	};

	class LogicalNot final : public BooleanUnaryOperation
	{
	public:
		LogicalNot(Expression *expression) : BooleanUnaryOperation(expression) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("LogicalNot"); }
#endif
	};

	class Not final : public UnaryOperation
	{
	public:
		Not(Expression *expression) : UnaryOperation(expression) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Not"); }
#endif
	};

	class Minus final : public UnaryOperation
	{
	public:
		Minus(Expression *expression) : UnaryOperation(expression) {}

		virtual Literal *calculate() const;

#ifdef DEBUG
		virtual void printNode() { LOG("Minus"); }
#endif
	};

	class IfExpression final : public Expression
	{
	public:
		IfExpression(Expression *test, Expression *trueResult, Expression *falseResult = nullptr) : mTest(test), mTrueResult(trueResult), mFalseResult(falseResult) {}

		Expression *getTest() const
		{
			return mTest;
		}

		void setTest(Expression *test)
		{
			mTest = test;
		}

		Expression *getTrueResult() const
		{
			return mTrueResult;
		}

		void setTrueResult(Expression *trueResult)
		{
			mTrueResult = trueResult;
		}

		Expression *getFalseResult() const
		{
			return mFalseResult;
		}

		void setFalseResult(Expression *falseResult)
		{
			mFalseResult = falseResult;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);

			if(mTest)
			{
				mTest->accept(operation);
			}

			if(mTrueResult)
			{
				mTrueResult->accept(operation);
			}

			if(mFalseResult)
			{
				mFalseResult->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("IfExpression"); }
#endif

	private:
		Expression *mTest;
		Expression *mTrueResult;
		Expression *mFalseResult;
	};

	class Member final : public TypedIdentity
	{
	public:
		Member(Type *type, std::string name) : TypedIdentity(type, name) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Member"); }
#endif
	};

	typedef std::list<Member *> Members;
}

#endif

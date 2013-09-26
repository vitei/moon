#ifndef COMPILER_TREE_NODE_H
#define COMPILER_TREE_NODE_H

#include "common.h"

#include <string>

namespace operation
{
	class Operation;
	class Restructure;
}

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Node
	{
	public:
		class Location
		{
		public:
			class Position
			{
			public:
				//Position() : line(1), character(1) {}

				void reset()
				{
					line = 1;
					character = 1;
				}

				unsigned int line;
				unsigned int character;
			};

			//Location() : filename(NULL) {}

			void reset()
			{
				filename = NULL;
				start.reset();
				end.reset();
			}

			void advance()
			{
				start = end;
			}

			void advanceCharacter(unsigned int num)
			{
				end.character += num;
			}

			void advanceLine(unsigned int num)
			{
				end.character = 1;
				end.line += num;
			}


#ifdef DEBUG
			void printLocation()
			{
				LOG("%u:%u to %u:%u in file \"%s\"", start.line, start.character, end.line, end.character, filename->c_str());
			}
#endif

			std::string *filename;
			Position start;
			Position end;
		};

		virtual void childAccept(operation::Operation *operation) {}

		void accept(operation::Operation *operation);
		void setup(operation::Operation *operation);
		void visit(operation::Operation *operation);
		Node *restructure(operation::Restructure *operation);

		const Location &getLocation() const
		{
			return mLocation;
		}

		void setLocation(const Location &location)
		{
			ASSERT(location.filename);
			mLocation = location;
		}

		void *getMetadata() const
		{
			return mMetadata;
		}

		void setMetadata(void *metadata)
		{
			mMetadata = metadata;
		}

#ifdef DEBUG
		virtual void printNode() = 0;
#endif

	protected:
		Node() : mMetadata(NULL)
		{
			/* Abstract class */
			mLocation.reset();
		}

	private:
		Location mLocation;

		void *mMetadata;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	template <class T>
	T const node_cast(tree::Node *node)
	{
		return dynamic_cast<T>(node);
	}

	/* ---- Helper macros ---- */

	// This is rather ugly but it saves a LOT of typing...
	#define DISPATCH_ACTIONS(class, node, function) \
		if(dynamic_cast<class *>(static_cast<tree::Node *>(node))) \
		{ \
			return function(static_cast<class *>(node)); \
		}

	#define GENERATE_DISPATCH(node, function) \
		ASSERT(node); \
		 \
		DISPATCH_ACTIONS(tree::Minus, node, function) \
		DISPATCH_ACTIONS(tree::Not, node, function) \
		 \
		DISPATCH_ACTIONS(tree::String, node, function) \
		DISPATCH_ACTIONS(tree::Int, node, function) \
		DISPATCH_ACTIONS(tree::Float, node, function) \
		DISPATCH_ACTIONS(tree::Array, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Void, node, function) \
		DISPATCH_ACTIONS(tree::UDT, node, function) \
		DISPATCH_ACTIONS(tree::Bool, node, function) \
		 \
		DISPATCH_ACTIONS(tree::For, node, function) \
		DISPATCH_ACTIONS(tree::While, node, function) \
		DISPATCH_ACTIONS(tree::TypeDefinition, node, function) \
		DISPATCH_ACTIONS(tree::Return, node, function) \
		DISPATCH_ACTIONS(tree::Execute, node, function) \
		DISPATCH_ACTIONS(tree::Import, node, function) \
		DISPATCH_ACTIONS(tree::If, node, function) \
		 \
		DISPATCH_ACTIONS(tree::SharedScoping, node, function) \
		DISPATCH_ACTIONS(tree::GlobalScoping, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Method, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Function, node, function) \
		DISPATCH_ACTIONS(tree::AnonymousScope, node, function) \
		DISPATCH_ACTIONS(tree::Aggregate, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Use, node, function) \
		DISPATCH_ACTIONS(tree::Program, node, function) \
		 \
		DISPATCH_ACTIONS(tree::FloatLiteral, node, function) \
		DISPATCH_ACTIONS(tree::IntLiteral, node, function) \
		 \
		DISPATCH_ACTIONS(tree::StringLiteral, node, function) \
		DISPATCH_ACTIONS(tree::BoolLiteral, node, function) \
		 \
		DISPATCH_ACTIONS(tree::This, node, function) \
		DISPATCH_ACTIONS(tree::IfExpression, node, function) \
		DISPATCH_ACTIONS(tree::FunctionCall, node, function) \
		DISPATCH_ACTIONS(tree::Cast, node, function) \
		 \
		DISPATCH_ACTIONS(tree::LogicalNot, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Unequal, node, function) \
		DISPATCH_ACTIONS(tree::LogicalOr, node, function) \
		DISPATCH_ACTIONS(tree::LogicalAnd, node, function) \
		DISPATCH_ACTIONS(tree::LessThan, node, function) \
		DISPATCH_ACTIONS(tree::LessEqual, node, function) \
		DISPATCH_ACTIONS(tree::GreaterThan, node, function) \
		DISPATCH_ACTIONS(tree::GreaterEqual, node, function) \
		DISPATCH_ACTIONS(tree::Equal, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Xor, node, function) \
		DISPATCH_ACTIONS(tree::Subtract, node, function) \
		DISPATCH_ACTIONS(tree::Or, node, function) \
		DISPATCH_ACTIONS(tree::Multiply, node, function) \
		DISPATCH_ACTIONS(tree::Modulus, node, function) \
		DISPATCH_ACTIONS(tree::Divide, node, function) \
		DISPATCH_ACTIONS(tree::Assign, node, function) \
		DISPATCH_ACTIONS(tree::And, node, function) \
		DISPATCH_ACTIONS(tree::Add, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Variable, node, function) \
		DISPATCH_ACTIONS(tree::Member, node, function) \
		DISPATCH_ACTIONS(tree::FunctionPrototype, node, function) \
		DISPATCH_ACTIONS(tree::Constant, node, function) \
		 \
		DISPATCH_ACTIONS(tree::MessageAccess, node, function) \
		DISPATCH_ACTIONS(tree::DirectAccess, node, function) \
		DISPATCH_ACTIONS(tree::ArrayAccess, node, function) \
		 \
		DISPATCH_ACTIONS(tree::Identifier, node, function) \
		 \
		ERROR("Unknown concrete class");

	#define ASSIGN_ACTIONS(dst, class, node, function) \
		if(dynamic_cast<class *>(static_cast<tree::Node *>(node))) \
		{ \
			dst = function(static_cast<class *>(node)); \
		}

	#define GENERATE_ASSIGN(dst, node, function) \
		ASSERT(node); \
		 \
		ASSIGN_ACTIONS(dst, tree::Minus, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Not, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::String, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Int, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Float, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Array, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Void, node, function) \
		else ASSIGN_ACTIONS(dst, tree::UDT, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Bool, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::For, node, function) \
		else ASSIGN_ACTIONS(dst, tree::While, node, function) \
		else ASSIGN_ACTIONS(dst, tree::TypeDefinition, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Return, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Execute, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Import, node, function) \
		else ASSIGN_ACTIONS(dst, tree::If, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::SharedScoping, node, function) \
		else ASSIGN_ACTIONS(dst, tree::GlobalScoping, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Method, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Function, node, function) \
		else ASSIGN_ACTIONS(dst, tree::AnonymousScope, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Aggregate, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Use, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Program, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::FloatLiteral, node, function) \
		else ASSIGN_ACTIONS(dst, tree::IntLiteral, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::StringLiteral, node, function) \
		else ASSIGN_ACTIONS(dst, tree::BoolLiteral, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::This, node, function) \
		else ASSIGN_ACTIONS(dst, tree::IfExpression, node, function) \
		else ASSIGN_ACTIONS(dst, tree::FunctionCall, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Cast, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::LogicalNot, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Unequal, node, function) \
		else ASSIGN_ACTIONS(dst, tree::LogicalOr, node, function) \
		else ASSIGN_ACTIONS(dst, tree::LogicalAnd, node, function) \
		else ASSIGN_ACTIONS(dst, tree::LessThan, node, function) \
		else ASSIGN_ACTIONS(dst, tree::LessEqual, node, function) \
		else ASSIGN_ACTIONS(dst, tree::GreaterThan, node, function) \
		else ASSIGN_ACTIONS(dst, tree::GreaterEqual, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Equal, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Xor, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Subtract, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Or, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Multiply, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Modulus, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Divide, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Assign, node, function) \
		else ASSIGN_ACTIONS(dst, tree::And, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Add, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Variable, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Member, node, function) \
		else ASSIGN_ACTIONS(dst, tree::FunctionPrototype, node, function) \
		else ASSIGN_ACTIONS(dst, tree::Constant, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::MessageAccess, node, function) \
		else ASSIGN_ACTIONS(dst, tree::DirectAccess, node, function) \
		else ASSIGN_ACTIONS(dst, tree::ArrayAccess, node, function) \
		 \
		else ASSIGN_ACTIONS(dst, tree::Identifier, node, function) \
		 \
		else ERROR("Unknown concrete class");
}

#endif

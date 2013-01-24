#ifndef COMPILER_TREE_NODE_H
#define COMPILER_TREE_NODE_H

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

			std::string *filename;
			Position start;
			Position end;
		};

		virtual void childAccept(operation::Operation *operation) {}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

		void setup(operation::Operation *operation);
		void visit(operation::Operation *operation);
		Node *restructure(operation::Restructure *operation);

		Location &getLocation()
		{
			return mLocation;
		}

		void setLocation(const Location &location)
		{
			ASSERT(location.filename);
			mLocation = location;
		}

		void *getMetadata()
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


	/* ---- Helper macros ---- */

	// This is rather ugly but it saves a LOT of typing...
	#define DISPATCH_ACTIONS(class, node, function) \
		if(dynamic_cast<class *>(node)) \
		{ \
			return function(static_cast<class *>(node)); \
		}

	#define GENERATE_DISPATCH(node, function) \
		ASSERT(node); \
		DISPATCH_ACTIONS(tree::Add, node, function) \
		DISPATCH_ACTIONS(tree::Aggregate, node, function) \
		DISPATCH_ACTIONS(tree::And, node, function) \
		DISPATCH_ACTIONS(tree::AnonymousScope, node, function) \
		DISPATCH_ACTIONS(tree::ArrayAccess, node, function) \
		DISPATCH_ACTIONS(tree::Assign, node, function) \
		DISPATCH_ACTIONS(tree::Bool, node, function) \
		DISPATCH_ACTIONS(tree::BoolLiteral, node, function) \
		DISPATCH_ACTIONS(tree::Cast, node, function) \
		DISPATCH_ACTIONS(tree::Constant, node, function) \
		DISPATCH_ACTIONS(tree::DefaultState, node, function) \
		DISPATCH_ACTIONS(tree::DirectAccess, node, function) \
		DISPATCH_ACTIONS(tree::Divide, node, function) \
		DISPATCH_ACTIONS(tree::Equal, node, function) \
		DISPATCH_ACTIONS(tree::Execute, node, function) \
		DISPATCH_ACTIONS(tree::Float, node, function) \
		DISPATCH_ACTIONS(tree::FloatLiteral, node, function) \
		DISPATCH_ACTIONS(tree::Function, node, function) \
		DISPATCH_ACTIONS(tree::FunctionCall, node, function) \
		DISPATCH_ACTIONS(tree::FunctionPrototype, node, function) \
		DISPATCH_ACTIONS(tree::GlobalScoping, node, function) \
		DISPATCH_ACTIONS(tree::GreaterEqual, node, function) \
		DISPATCH_ACTIONS(tree::GreaterThan, node, function) \
		DISPATCH_ACTIONS(tree::Identifier, node, function) \
		DISPATCH_ACTIONS(tree::If, node, function) \
		DISPATCH_ACTIONS(tree::Import, node, function) \
		DISPATCH_ACTIONS(tree::Int, node, function) \
		DISPATCH_ACTIONS(tree::IntLiteral, node, function) \
		DISPATCH_ACTIONS(tree::LessEqual, node, function) \
		DISPATCH_ACTIONS(tree::LessThan, node, function) \
		DISPATCH_ACTIONS(tree::LogicalAnd, node, function) \
		DISPATCH_ACTIONS(tree::LogicalNot, node, function) \
		DISPATCH_ACTIONS(tree::LogicalOr, node, function) \
		DISPATCH_ACTIONS(tree::MessageAccess, node, function) \
		DISPATCH_ACTIONS(tree::Minus, node, function) \
		DISPATCH_ACTIONS(tree::Modulus, node, function) \
		DISPATCH_ACTIONS(tree::Multiply, node, function) \
		DISPATCH_ACTIONS(tree::NamedState, node, function) \
		DISPATCH_ACTIONS(tree::Not, node, function) \
		DISPATCH_ACTIONS(tree::NullReference, node, function) \
		DISPATCH_ACTIONS(tree::Or, node, function) \
		DISPATCH_ACTIONS(tree::Program, node, function) \
		DISPATCH_ACTIONS(tree::Reference, node, function) \
		DISPATCH_ACTIONS(tree::Return, node, function) \
		DISPATCH_ACTIONS(tree::SetState, node, function) \
		DISPATCH_ACTIONS(tree::SharedScoping, node, function) \
		DISPATCH_ACTIONS(tree::String, node, function) \
		DISPATCH_ACTIONS(tree::StringLiteral, node, function) \
		DISPATCH_ACTIONS(tree::Subtract, node, function) \
		DISPATCH_ACTIONS(tree::UDT, node, function) \
		DISPATCH_ACTIONS(tree::Unequal, node, function) \
		DISPATCH_ACTIONS(tree::Use, node, function) \
		DISPATCH_ACTIONS(tree::Variable, node, function) \
		DISPATCH_ACTIONS(tree::Void, node, function) \
		DISPATCH_ACTIONS(tree::While, node, function) \
		DISPATCH_ACTIONS(tree::Xor, node, function) \
		ERROR("Unknown concrete class");
}

#endif

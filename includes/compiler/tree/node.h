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
				Position() : line(1), character(1) {}

				unsigned int line;
				unsigned int character;
			};

			Location() : filename(NULL) {}

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

	protected:
		Node() { /* Abstract class */ }

	private:
		Location mLocation;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

}

#endif

---
layout: default
title: "The Moon Programming Language"
---

Moon is a new programming language from Vitei, designed to offer the speed of development of scripting languages but the run-time speed of C.

Features
--------
Words go here...

Example
-------
Here's a short example of a Moon program:

	function main()
		names = ["Billy", "Jean", "Jackson"]

		for name in names
			print name
		end
	end

To compile this program to C code simply run

	moon filename.moon

and then put the resulting output through your favorite C compiler.

## Obtaining
To compile Moon you will need a C++ compiler, GNU Flex, a recent version of GNU Bison and Make.

	git clone git://github.com/vitei/moon.git
	cd moon
	make

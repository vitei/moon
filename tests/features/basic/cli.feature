Feature: CLI
	Test the basic commandline interface of the compiler.

	Scenario: No input
		Show no file error

		When I run `moon`
		Then the stderr should contain exactly "Error: No input files\n"
		And the exit status should be 1

	Scenario: No valid input
		Show file not found error

		When I run `moon egg.moon`
		Then the stderr should contain exactly "Error: Could not load file egg.moon\n"
		And the exit status should be 1

#	Scenario Outline: Valid Parameters
#		Valid parameters are specified for options that require them

#		When I run `moon -<opt><param> sources/basic/pass.moon`
#		Then the exit status should be 0

#		Examples:
#			| opt | param |
#			|  D  | 1     |
#			|  G  | 1     |
#			|  I  | 1     |
#			|  U  | 1     |
#			|  o  | 1     |

#	Scenario Outline: Missing Parameters
#		Parameters are missing for options that require them

#		When I run `moon -<opt>`
#		Then the stderr should contain exactly "Error: Option \"<opt>\" requires a parameter\n"
#		And the exit status should be 1

#		Examples:
#			| opt |
#			|  D  |
#			|  G  |
#			|  I  |
#			|  U  |
#			|  o  |

#	Scenario Outline: Invalid directories
#		Compiler should error when directories do not exist (define/include/use files)

	Scenario: Help
		Show help output

		When I run `moon -h`
		Then the exit status should be 2

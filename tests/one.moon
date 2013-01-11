include output
use Base

global var float:positionX
global var positionY

var x = 30
var y = 20

#global var x

global ref string:test

global ref moon

const string:STR_ONE = "One" # Some more text here...
const string:STR_TWO = "Two"
const string:STR_THREE = "Three"

function float:test_add(float:one, int:two)
	return one + two
end

#global function main()
function main()
	var float:localX = 0.0
	#var string:test = STR_ONE + STR_TWO

	positionX = 0

	#positionX = "STR_ONE" + positionX

	# Illegal
	#positionX(1, 2.0)
	#test_add = positionX + positionX
	#test_add(1, 2.0, 2)
	#positionX + positionX = positionX + positionX

	test_add(1, 2.0)

	-#-
	for(test = 0; test < 10; test++)
		x = x + test
	end
	-#-
end

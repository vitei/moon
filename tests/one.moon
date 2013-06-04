include output
use Base

import function printf(string:data)

#global var float:positionX
#global var positionY

var x = 5:020 % 20
var y = 20

#global var x

const STR_ONE = "One" # Some more text here...
const STR_TWO = "Two"
const STR_THREE = "Three"

function float:test_add(float:one, int:two)
	return one + two
end

#global function main()
shared function main()
	var localX = 0.0
	var localY = 2
	#var string:test = STR_ONE + STR_TWO

	while localX < 10
		printf("test some stuff for me...\n")
		localX += 0.1
	end

	#positionX = 0

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

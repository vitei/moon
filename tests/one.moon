include output
use Base

import def printf(string:data)

#global def float:positionX
#global def positionY

def x = 5:020 % 20
def y = 20

#global def x

const STR_ONE = "One" # Some more text here...
const STR_TWO = "Two"
const STR_THREE = "Three"

def test_add(float:one, float:two)
	voom = 1

	return 2 if voom == 1
	return 1

#	return voom if one < two else voom = one + two
#	return voom
end

#global def main()
shared def main()
	const COOK = 1

	test = COOK
	test = test_add(1, 2)

#	foom = 1
#	moo = foom + foom
#	localX = moo + moo
#	localY = 2

#	foom = test_add(1.2, 1.2) while foom < 10

	#def string:test = STR_ONE + STR_TWO

#	localZ = 0
#	if localY == 2
#		localZ = 1
#	else
#		localZ = 0.4
#	end

#	while localX < 10
#		printf("test some stuff for me...\n")
#		localX += 0.05
#		localX = test_add(localX, 0.05)
#	end

	#positionX = 0

	#positionX = "STR_ONE" + positionX

	# Illegal
	#positionX(1, 2.0)
	#test_add = positionX + positionX
	#test_add(1, 2.0, 2)
	#positionX + positionX = positionX + positionX

#	test_add(1, 2.0)

	-#-
	for(test = 0; test < 10; test++)
		x = x + test
	end
	-#-
end

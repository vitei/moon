include output
use Base

import def printf(data:string, param1:int)

#global def float:positionX
#global def positionY

#def x = 5:020 % 20
def y = 20

#global def x

-#-
const STR_ONE = "One" # Some more text here...
const STR_TWO = "Two"
const STR_THREE = "Three"
-#-

def test_add(one:float, two:float):int
	voom = 1

	return 2 if one == 1
	return 1

#	return voom if one < two else voom = one + two
#	return voom
end

#global def main()
shared def main()
	def ccc:int[30]
	def ccx:int[20][30]

	#	ccc = 3

	#	def xxxx : int[10]
	#	def yyyy : float[10]

	#	yyyy = xxxx
	#	xxxx = yyyy


#	def float[20][30]:ccx

#	x[1] = 10
#	ccc = 1[1]

	#def int[0]:notallowed
	#def int[-1]:notallowed

	#	ccc = ccx
	#	ccx = ccc

	#	ccc = ccx[0]

	def dd:float
	dd = 1.0

	#########################################
	y = 0
	while y < 20
		x = 0
		while x < 30
			ccx[y][x] = 10
			x += 1
		end
		y += 1
	end
	#########################################


	yyy = true
	yyy = 1
	yyy = 3.2
	test_add(yyy, dd)

	x = 1
	while x < 30
		ccc[x] = x
		ccc[x - 1] += x
		x += 1
	end

	x = 0
	while x < 30 # Need a for loop
		printf("XXX: %u\n", ccc[x])
		#printf("...")  if x % 3 != 0 && x % 5 != 0
		#printf("fizz") if x % 3 == 0
		#printf("buzz") if x % 5 == 0
		#printf("\n")

		x += 1
	end


	#const COOK = 1

	#test = COOK
	#test = test_add(1, 2)

	#if test == COOK
	#	const MOO = 2

	#	egg  =MOO
	#end

	#boombox = MOO

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

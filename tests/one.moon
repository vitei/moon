//include "vector.inc";
use Base;

global var positionX;
global var positionY;

var x = 30;
var y = 20;

var x = 30;

ref test = x;

const string:STR_ONE = "One";
const string:STR_TWO = "Two";

function float:test_add(float:one, int:two)
{
	return one + two;
}

function main()
{
	var float:localX = 0.0;
	var string:test = STR_ONE + STR_TWO;

	positionX = 0;

	test_add(1.0, 2.0);

//	for(test = 0; test < 10; test++)
//	{
//		x = x + test;
//	}
}
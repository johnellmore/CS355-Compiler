function increment:short number:short end
== this is a comment in the file
vars x:bool end

function sum:long in:long[100] ; size:byte
vars total:long ; j:byte do
	set j 0 ;
	{
		set total + total in[++j]
	} ^ < j size
	return total

function addthree:long n1:long ; n2:long ; n3: long
vars arr:long[100] do
	set arr[0] n1;
	set arr[1] n2;
	set arr[2] n3
	return sum arr 3 !

function thethree:long 
	return addthree 111 2222 331000 !

function negate:long input:long 
	return neg input

function position:long needle:short; haystack:short[10]
	vars i:short; found:bool do
	set i 0;
	set found false;
	{
		{
			set found not false
		} if = haystack[i] needle else {
			set i increment i !
		}
	} ^ and < i 10 = found false
	return neg negate i !

== the is the definition of a previously declared function
function increment:short number:short do
	--number;
	++number;
	set number + - 1 number 2
	return number

== make sure that we can add later variables to the module global vars
vars y:long end

== another declaration (also tries fake-out identifiers which start with keywords)
function andtest:short something:byte end

function andtest:short something:byte 
	return something

end

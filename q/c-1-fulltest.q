function increment:short number:short end
== this is a comment in the file
vars x:bool end

function position:short needle:short; haystack:short[10]
	vars i:short; found:bool do
	set i 0;
	set found false;
	{
		{
			set found true
		} if = haystack[i] needle else {
			set i increment i !
		}
	} ^ and < i 10 = found false
	return neg neg i

== the is the definition of a previously declared function
function increment:short number:short do
	--number;
	++number;
	set number + number 1
	return number

== make sure that we can add later variables to the module global vars
vars y:long end

== now just a declaration (also tries fake-out identifiers which start with keywords)
function andtest:bool something:bool end

end

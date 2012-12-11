function add:long n1:long ; n2:long
	return + n1 n2

function test:long input:long do
	add input ! ;
	add input input input !
	return input

end
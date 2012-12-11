function sum:long in:long[100] ; size:byte
vars total:long ; j:byte do
	set j 0 ;
	{
		set total + total in[j];
		++j
	} ^ < j size
	return total

function addthree:long n1:long ; n2:long ; n3: long
vars arr:long[10] do
	set arr[0] n1;
	set arr[1] n2;
	set arr[2] n3
	return sum arr 3 !

end
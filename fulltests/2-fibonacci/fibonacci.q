function qFibonacci:long n:long
vars r:long do
	{
		set r 1
	} if <= n 2 else {
		set r + qFibonacci - n 1 ! qFibonacci - n 2 !
	}
	return r

end
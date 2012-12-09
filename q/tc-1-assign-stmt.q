function increment:long 
vars b:bool ; t:byte ; s:short ; n:long ; a:long[10] do
	
	--t ;
	++b
	
	=={
	==	set n + s t
	==} if + s n
	== numerics yielding numeric
	==set n + s t ;
	==set n - n n ;
	==set n * n n ;
	==set n / n n ;
	==set n % n n ;
	
	== numerics yielding bool
	==set b < n s ;
	==set b > s n ;
	==set b <= n n ;
	==set b >= n n ;
	==set b = n n ;
	==set b <> n n ;
	
	== bools yielding bool
	==set b and b b ;
	==set b or b b
	return n

end

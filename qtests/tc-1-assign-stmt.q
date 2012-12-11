function test:bool blist:bool[10] ; n:long[5] do
	set blist[3] blist[5] ;
	--n[3]
	return not blist[3]

function increment:long
	b1:bool ;
	b2:bool[10] ;
	by1:byte ;
	by2 : byte[10] ;
	s1:short ;
	s2:short[10] ;
	l1:long ;
	l2:long[10]
do
	set l1 l2[5]
	return l2[5]

function callFunc:bool
vars
	ab1:bool ;
	ab2:bool[10] ;
	aby1:byte ;
	aby2 : byte[10] ;
	as1:short ;
	as2:short[5] ;
	al1:long ;
	al2:long[10]
do
	set al1 increment ab1 ab2 aby1 aby2 as1 as2 al2[5] as2 ! ;
	set ab2[5] ab1
	return = al1 al2[5]

end

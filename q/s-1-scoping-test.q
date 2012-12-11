function othertest:short p:short end

function scopetest:long param:long end

vars x:short ; p:short end

function scopetest:long param:long 
vars local:long do
	set local * p x
	return local

function othertest:short p:short
vars l:short do
	set l * p x
	return l

function fgothertest:short p:short
vars l:short do
	set l * p x
	return l

function othesdfrtest:short p:short
vars l:short do
	set l * p x
	return l

end

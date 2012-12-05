function position:short needle:short; haystack:short[10]
	vars i:short; found:bool do
	set i 0;
	set found false;
	{
		{
			set found true
		} if = haystack[i] needle;
		++ i
	} ^ and < i 10 = false
	return i

end


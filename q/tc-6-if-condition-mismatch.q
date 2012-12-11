function convert:bool input:bool[2]
vars temp:bool do
	{
		set temp input[0]
	} if 5 else {
		set temp input[1]
	}
	return temp

end
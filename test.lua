local RandomBox = require "RandomBox"

function My_print_tb(tb,tabn)
	if type(tb) ~= "table" then
		return
	end
	tabn = tabn or 0
	local ktab = string.rep("\t",tabn + 1)
	local ttab = string.rep("\t",tabn)
	print(ttab.."{")
	for k,v in pairs(tb) do
		if type(v) == "table" then
			print(ktab..k.." = ")
			My_print_tb(v,tabn+1)
		else
			print(ktab..k.." = "..v..",")
		end
	end
	print(ttab.."}"..(tabn>0 and "," or " "))
end

function wait_for(sec)
	local wtime = os.time()+sec
	while(wtime > os.time)do

	end
end


--My_print_tb(s)

function sleep(n)
	os.execute("sleep " .. n)
end

--print(os.date('%Y-%m-%d %H:%M:%S',os.time()))

local tab = {
	{per = 10,id = 1},
	{per = 10,id = 2},
	{per = 10,id = 3},
	{per = 10,id = 4},
	{per = 10,id = 5},
	{per = 10,id = 6},
	{per = 10,id = 7},
	{per = 10,id = 8},
	{per = 10,id = 9},
}

--local rand = RandomBox:new()





local  GM_tool = {}

GM_tool.playerinfo = nil

local function print_tb(tb)
	for k,v in pairs(tb) do
		print(k,v)
	end
end

local function getNum(num)
	if string.match(num,"%a") then
		return string.match(num,"%d+e%d+")
	else
		return num
	end
end

GM_tool.callfun = {
	["addmoney"] = function(num)  
		local gnum = getNum(num)
		GM_tool.playerinfo:giveBnGoldDirect(num)
		GM_tool.playerinfo:updatePlayerPropertyDB(true)
		--print(gnum)
	end,
}

GM_tool.splist = function (str)
	local s_tb = {}
	str = string.lower(str)
	for s in string.gmatch(str,"%w+") do
		table.insert( s_tb, s)
	end
	return s_tb
end

GM_tool.chick = function (playerinfo,str)
	if playerinfo == nil then return end
	GM_tool.playerinfo = playerinfo
	local s_tb = GM_tool.splist(str)

	if s_tb[1] == "gm" then
		local gm = {}
		gm.pro = s_tb[2]
		gm.hdl = s_tb[3]
		gm.num = s_tb[4] or 0

		local handle = GM_tool.callfun[gm.hdl..gm.pro]
		if handle then handle(gm.num) end
		return true
	end
	return false
end

return GM_tool

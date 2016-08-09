--- 抽卡 ------------
local RandomBox = require "utility.RandomBox"
local MDrawCard = {}
-- 消耗钻石
MDrawCard.OneCost = 50
MDrawCard.TenCost = 500
-- 抽卡类型
MDrawCard.DRAW_NOR = 1
MDrawCard.DRAW_ONE_DIA = 2
MDrawCard.DRAW_TEN_DIA = 3
-- 卡池类型
MDrawCard.DIA_POOL = 1
MDrawCard.S_CARD_POOL = 2
MDrawCard.S_CARDFU_POOL = 3

MDrawCard.norRand = nil		-- 普通卡池
MDrawCard.diaRand = nil		-- 钻石卡池
MDrawCard.SCardRand = nil	-- S级主卡池
MDrawCard.SCardFuRand = nil  -- S级副卡池
--- 十连抽结构 ----
MDrawCard.drawStruct = {
	all = 10,
	SCard = {min = 2,max = 2},
	SCardFu = {min = 1,max = 1},
}

function MDrawCard.init()
	MDrawCard.norRand = RandomBox:new()
	MDrawCard.diaRand = RandomBox:new()
	MDrawCard.SCardRand = RandomBox:new()
	MDrawCard.SCardFuRand = RandomBox:new()

	MDrawCard.setNorRand()
	MDrawCard.setDiaRand()
end
-- 加载奖池
function MDrawCard.setNorRand()
	for k,v in pairs(MTemplate["template_drawnor"]) do
		MDrawCard.norRand:insert(v)
	end
end
-- 加载奖池
function MDrawCard.setDiaRand()
	for k,v in pairs(MTemplate["template_drawdia"]) do
		
		MDrawCard.diaRand:insert(v)
		
		if v["pooltype"] == MDrawCard.S_CARD_POOL then
			MDrawCard.SCardRand:insert(v)
		elseif v["pooltype"] == MDrawCard.S_CARDFU_POOL then
			MDrawCard.SCardFuRand:insert(v)
		end
	end
end
-- 玩家抽卡
function MDrawCard.DrawCard(playerinfo, param, socketid)
 	local drawtype = param.drawtype
 	local cost = param.cost					--	钻石消耗
 	local pldrawcard = playerinfo:getPlayerDrawCard()
 	local goods = nil
 	My_print_tb(param)
 	if drawtype == MDrawCard.DRAW_NOR then
 		local dfreenum = pldrawcard:get_rest_freecount()
 		print("freenum ------- ",dfreenum)
 		if dfreenum > 0 then
 			goods = MDrawCard.DrawNor()
 			pldrawcard:refreshNorFree()
 		end
 	elseif drawtype == MDrawCard.DRAW_ONE_DIA then
 		local freetime = pldrawcard:get_dia_freetime()
 		if os.time() >= freetime then -- 免费
 			print("-------- 钻石免费 -------------")
 			goods = MDrawCard.DrawOneDia()
 			pldrawcard:refreshDiaFree()
 		else
	 		if cost == MDrawCard.OneCost and playerinfo:checkDia(cost) then
	 			goods = MDrawCard.DrawOneDia()
	 			playerinfo:subDia(cost)
	 		else
	 			MError.sendError(socketid, MError.error_code_tabel.DIA_AMOUNT_ERROR)
				return
	 		end
	 	end
 	elseif drawtype == MDrawCard.DRAW_TEN_DIA then
 		print("----------- 钻石十连抽 --------------")
 		if cost == MDrawCard.TenCost and playerinfo:checkDia(cost) then
 			goods = MDrawCard.DrawTenDia()
 			playerinfo:subDia(cost)
 		else
 			MError.sendError(socketid, MError.error_code_tabel.DIA_AMOUNT_ERROR)
			return
 		end
 	end

 	if goods then
 		--MDrawCard.sendGood(playerinfo,goods)
 		pldrawcard:updateDB()
 		My_print_tb(goods)
 	end
 end 

--- 普通抽卡一次
function MDrawCard.DrawNor()
	local good = MDrawCard.norRand:rand()
	return {good}
end
--- 钻石抽卡一次
function MDrawCard.DrawOneDia()
	local good = MDrawCard.diaRand:rand()
	return {good}
end
--- 钻石抽卡 10 次
function MDrawCard.DrawTenDia()
	local struct = MDrawCard.getNumStruct()
	local goods = {}
	local good = nil

	good = MDrawCard.SCardRand:randUnRep(struct.SCard)
	for k,v in pairs(good) do
		table.insert( goods, v )
		struct.SCard = struct.SCard - 1
	end

	good = MDrawCard.SCardFuRand:randUnRep(struct.SCardFu)
	for k,v in pairs(good) do
		table.insert( goods, v )
		struct.SCardFu = struct.SCardFu - 1
	end

	struct.other = struct.other + struct.SCard + struct.SCardFu
	if MDrawCard.diaRand:getSize() > 0 then
		while(struct.other > 0) do
			good = MDrawCard.diaRand:rand()
			if good.pooltype == MDrawCard.DIA_POOL then
				table.insert( goods, good )
				struct.other = struct.other - 1
			end
		end
	end
	return goods
end
-- 数量结构
function MDrawCard.getNumStruct()
	local struct = {}
	struct.SCard = math.random(MDrawCard.drawStruct.SCard.min,MDrawCard.drawStruct.SCard.max)
	struct.SCardFu = math.random(MDrawCard.drawStruct.SCardFu.min,MDrawCard.drawStruct.SCardFu.max)
	struct.other = MDrawCard.drawStruct.all - struct.SCard - struct.SCardFu
	return struct
end
-- 发送物品
function MDrawCard.sendGood(playerinfo,good)
	if type(good) == type({}) then
		for k,v in pairs(good) do
			local libao = {[v.itemtype] = v.num}
			MLiBao.parseLiBaoTpl(playerinfo,libao)
		end
		playerinfo:callClient("onMDrawCard",good)
	end
end

MDrawCard._module = {
	DrawCard = MDrawCard.DrawCard,
}

return MDrawCard
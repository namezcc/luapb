-- 扭蛋抽卡数据 -----
local ObjectBaseInfo = require "game_sys.game_obj.ObjectBaseInfo"
local PLayerDrawCard = ObjectBaseInfo:new()

PLayerDrawCard.autoGenFuncList = {
	"nor_freetime",
	"dia_freetime",
	"rest_freecount",
}

PLayerDrawCard:initAutoGenFunc()

function PLayerDrawCard:SetDefaultData(pid)
	if pid == nil or self.baseInfo ~= nil then return end

	local tab = {}
	tab.pid = pid
	tab.nor_freetime = os.time()
	tab.dia_freetime = os.time()
	tab.rest_freecount = MGameEnum.drawFreeCount
	self:init(tab)
	self:insertDB(pid)
end
-- 刷新免费次数和时间
function PLayerDrawCard:refreshNorFree()
	self.baseInfo.nor_freetime = os.time() + 3600
	if self.baseInfo.rest_freecount > 0 then
		self.baseInfo.rest_freecount = self.baseInfo.rest_freecount - 1
	end
end

function PLayerDrawCard:refreshDiaFree()
	self.baseInfo.dia_freetime = os.time() + 24*3600
end

function PLayerDrawCard:updateDB()
	local function callback( )

	end
	local filter = {
		pid = 1,
	}
	local sql = MUtility.genUpdateSqlForDB("player_draw_card",self.baseInfo,"pid",filter)

	MSys.DBQuery(sql,callback)
end

function PLayerDrawCard:insertDB(pid)
	if not pid then return end
	local function callback()
		
	end 
	local sql = string.format("INSERT INTO player_draw_card(pid) VALUES(%d)",pid)
	MSys.DBQuery(sql,callback)
end

return PLayerDrawCard
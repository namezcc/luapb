local TigerMachinList = {}
TigerMachinList.goodlist = {}
TigerMachinList.stop = true
TigerMachinList.needgoodidx = 0
TigerMachinList.losebgidx = 0
TigerMachinList.state = 1  -----1 加速 2 匀速 3 减速
TigerMachinList.up = false
TigerMachinList.listview = nil
TigerMachinList.info = {
	speed = 0,
	addv = 2,
	losev = 4,
	maxv = 50,
	goodidx = 1,
	goodHeight = 500,
	pos = 0,
}

function TigerMachinList:new(obj)
	obj = obj or {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end

function TigerMachinList:init(goodlist,idx)
	self.goodlist = goodlist
	self.stop = false
	self.listview =listview
	self.needgoodidx = idx
	--self.goodHeight = goodlist[1]:getContentSize().height

	--self:setPos()

end

function TigerMachinList:run(dt)
	if not self.stop then
		self:changeV(dt)
		self:setPos()
	else
		return self.info.goodidx --self.goodlist[self.needgoodidx]
	end
end

function TigerMachinList:setPos()
	local pos =self.info.pos + self.info.speed
	local pass,passpos = math.modf(pos/self.info.goodHeight)

	local idx = self.info.goodidx
	if pass == 0 then
		self.info.pos = pos
	else
		self.info.pos = passpos
		idx = self:setNextIdx(-pass)
	end

	local gooddis = self:getListDis(idx)
	for k,v in pairs(gooddis) do
		self.goodlist[k].pos = v * self.info.goodHeight + pos
	end
	return pass
end

function TigerMachinList:getNextIdx(nowidx,pass)
	local size = #self.goodlist
	local idx = math.mod(nowidx + pass,size)
	if idx == 0 then
		idx = size
	elseif idx <0 then
		idx = size + idx
	end
	return idx
end

function TigerMachinList:setNextIdx(pass)
	self.info.goodidx = self:getNextIdx(self.info.goodidx,pass)
	return self.info.goodidx
end

function TigerMachinList:getdis(fir,sen,size)
	local dis = sen - fir
	local half = math.floor(size/2)
	if math.abs(dis) <= half then
		return dis
	else
		if dis > 0 then
			return dis - size
		else
			return size + dis
		end
	end
end

function TigerMachinList:getListDis(idx)
	local listdis = {}
	for i=1,#self.goodlist do
		listdis[i] = self:getdis(idx,i,#self.goodlist)
	end
	return listdis
end

function TigerMachinList:changeV(dt)
	if self.state == 1 then
		self.info.speed = self.info.speed + self.info.addv
		if self.info.speed >= math.abs(self.info.maxv) then
			self.info.speed = self.info.maxv
			self.state = 2
			self:getLoseCondition(dt)
		end
		return
	elseif self.state == 2 then
		if self.info.goodidx == self.losebgidx then
			if math.abs(self.info.pos) >= math.abs(self.info.goodHeight - self.residue) then
				print(self.info.goodidx,self.info.pos,self.residue)
				self.state = 3
			end
		end
		return
	else
		if  math.abs(self.info.speed)<= self.info.losev then
			self.stop = true
			self.info.pos = 0
		end
		self.info.speed = self.info.speed - self.info.losev
	end
end

function TigerMachinList:getLoseCondition(dt)
	local x = self.info.maxv*(self.info.maxv/self.info.losev)*dt
	local passnum,residue = math.modf(x/self.info.goodHeight)

	self.losebgidx = self:getNextIdx(self.needgoodidx,passnum)
	self.residue = residue * self.info.goodHeight

	print(x,passnum,self.losebgidx,self.info.goodidx)
end

return TigerMachinList


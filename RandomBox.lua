local RandomBox = {}
--[[
	随机盒子
--]]
math.randomseed(tostring(os.time()):reverse():sub(1, 6))
function RandomBox:new(obj)
	obj = obj or {}
	setmetatable(obj, self)
	self.__index = self

	obj.Box = {}
	obj.allPer = 0
	obj.size = 0

	return obj
end
---增加总权重
function RandomBox:addPer(per)
	self.allPer = self.allPer + per
end
--tb形式{per = 权重 整数 ,.... }
function RandomBox:insert(tb)
	if tb and tb.per then
		table.insert(self.Box,tb)
		self:addPer(tb.per)
		self.size = self.size + 1
	end
end
-- 插入表
function RandomBox:insertTab(tab)
	if type(tab) == type({}) then
		for k,v in pairs(tab) do
			if v.per then
				self:insert(v)
			end
		end
	end
end
--移除键值对的一项
function RandomBox:removeVal(key,val)
	for k,v in pairs(self.Box) do
		if v[key] == val then
			self:addPer(-v.per)
			self.Box[k] = nil
			self.size = self.size - 1
			return v
		end
	end
end
---移除idx项，或最后一项 
function RandomBox:remove(idx)
	if idx then
		if self.Box[idx] then
			local t = self.Box[idx]
			self:addPer(-self.Box[idx].per)
			self.Box[idx] = nil 
			self.size = self.size - 1
			return t
		end
	end
end

function RandomBox:clear()
	self.Box = {}
	self.allPer = 0
end
----获取随机项
function RandomBox:rand()
	if self.size <= 0 then 
		return
	end

	local randNum = math.random(self.allPer)
	local keyNum = 0
	for k,v in pairs(self.Box) do
		keyNum = keyNum + v.per
		if keyNum >= randNum then
			return v,k
		end
	end
end
---获取随机项除了哪几项
function RandomBox:randWithOut(tb)
	if self.size <= 0 then 
		return
	end

	local rtb = {}
	for k,v in pairs(tb) do
		table.insert(rtb,self:removeVal(v.key,v.val))
	end
	 
	local res = self:rand()

	self:insertTab(rtb)
	
	return res
end

function RandomBox:randAndRemove()
	if self.size <= 0 then 
		return
	end

	local v,k = self:rand()
	self:remove(k)
	return v
end
-- 获取不重复的几项
function RandomBox:randUnRep(num)
	if self.size < num then
		return
	end
	local tmp = {}
	local rtmp = {}
	for i=1,num do
		local v,k = self:rand()
		tmp[i] = v
		rtmp[i] = self:remove(k)
	end
	self:insertTab(rtmp)
	return tmp
end

function RandomBox:getSize()
	return self.size
end

function RandomBox:getData()
	return self.Box
end

return RandomBox

function initSeed(seed)
    math.randomseed(seed);
end

function newActionID(start)
	if start == nil then
		start = 10;
	end
	return function()
		start = start + 1;
		return start;
	end
end

function ERROR_LOG(error_msg)
	
end

function DEBUG_LOG(debug_msg)

end

function WARN_LOG(warn_msg)

end

function TRACE_LOG(trace_msg)

end

--1:天族 2:魔族 3:山海大陆（中立）


--任务类型
--1：主线任务 2:支线任务 3:日常任务
--1:对话 2:打怪 3:采集 4:护送 5: 使用物品 6:收集 7:送信 8:探索


--0：表示非有效action
--任务相关	1
--商店行为	2
--铁匠行为	3
--宝石行为 4
--装备洗练	5
--装备合成 6


-- 0 : 表示正常状态(即非任务状态)或任务正在进行中 1 : 表示任务可接 2 : 表示任务可提交

ActionTable = {
	m_ActionType = 0,
	m_ActionToken = 0,
	m_ActionID = 0,
	m_ActionStep = 0,
	m_NpcMsg = "",			--初始化时预留
	m_ActionMsg = ""
	};
	
NpcTrigger = {
	m_NpcId = 0,
	m_ActionCont = {}
	};
	
function ActionTable:Instance(o)
	o = o or {};
	setmetatable(o, self);
	self.__index = self;
	return o;	
end
	
function NpcTrigger:Instance(o)
	o = o or {};
	setmetatable(o, self);
	self.__index = self;
	return o;	
end

function NpcTrigger:SetNpcId(npcId)
	self._npcId = npcId;
end

function NpcTrigger:SetNpcMsg(npcMsg)
	self._npcMsg = npcMsg;
end

function NpcTrigger:PushNpcActionMsg(actionMsg)
	table.insert(self._actionCont, actionMsg);
end

function NpcTrigger:PopNpcActionMsg()
	local size = table.getn(self._actionCont);
	if size == 0 then
		return nil;
	end
	local elem = self._actionCont[size];
	table.remove(self._actionCont);
	return elem;
end

function NpcTrigger:GetActionContSize()
	return table.getn(self._actionCont);
end

function NpcTrigger:GetActionElem(index)
	return self._actionCont[index];
end

function  NpcTrigger:GetActionCont()
	return self._actionCont;
end


function TaskCanAccept(task)
	
end

function GetPlayer()
	return _GameActionLua:GetPlayer1();
end

function GetTaskData(id)
	return _GameActionLua:GetTaskData(GetPlayer(), id);
end

function GetTaskIdByTaskType(taskType)
	
end

function GetPlayerData(field)
	return _GameActionLua:GetPlayerData(GetPlayer(), field);
end

function RunConveyAction(player, taskId)
	return _GameActionLua:RunConveyAction(player, taskId);
end

function RunAutoBattleAction(player, battle, turns)
	return _GameActionLua:RunAutoBattleAction(player, battle, turns);
end

function RunExploreBattleAction(player, npcId)
	return _GameActionLua:RunExploreBattleAction(player, npcId);
end

function GetPlayerName(player)
	return _GameActionLua:GetPlayerName(player);
end

function GetPlayerStateName(player)
	return _GameActionLua:GetPlayerStateName(player);
end

function RunTask(player, taskId, npcId)
	return _GameActionLua:RunTask(player, taskId, npcId);
end

function RunDayTaskAccept(player, taskId, npcId)
	return _GameActionLua:RunDayTaskAccept(player, taskId, npcId);
end

function GetCompletedNpcDayTask(player, npcId)
	return _GameActionLua:GetCompletedNpcDayTask(player, npcId);
end

function GetDayTaskCompletedCount(player, dayTaskId)
	return _GameActionLua:GetDayTaskCompletedCount(player, dayTaskId);
end

function GetDayTaskFlushColor(player, dayTaskId)
	return _GameActionLua:GetDayTaskFlushColor(player, dayTaskId);
end

function GetItemName(itemId)
	return _GameActionLua:GetItemName(itemId);
end

function GetGreatFighterName(fgtId)
	return _GameActionLua:GetGreatFighterName(fgtId);
end

function GetSharpDay(now)
	return _GameActionLua:GetSharpDay(now);
end

function RunItemTaskAction(player, taskId, dummyNpcId)
	_GameActionLua:RunItemTaskAction(player, taskId, dummyNpcId);
end

function GetStateName(cny)
	if cny == 0 then
		return "峨眉";
    end
    if cny == 1 then
		return "昆仑";
    end
    return "中立";
end

--[[
gold = 1;
coupon = 2;
tael = 3;
coin = 4;
status = 5;
country = 6; 
location = 7;
inCity = 8;
]]--

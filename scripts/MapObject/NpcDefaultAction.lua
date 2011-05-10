require("global")



local Npc_Inn = {};
--愚人节清明节活动
local stage = getActivityStage();
if stage == 3 then	
	local action = ActionTable:Instance();
	action.m_ActionType = 0x30;
	action.m_ActionToken = 1;
	action.m_ActionID = 9;
	action.m_ActionStep = 0;
	action.m_ActionMsg = "兑换清明祭祖礼包";
	table.insert(Npc_Inn, action);
end

if stage == 1 then
	local action = ActionTable:Instance();
	action.m_ActionType = 0x30;
	action.m_ActionToken = 1;
	action.m_ActionID = 8;
	action.m_ActionStep = 0;
	action.m_ActionMsg = "兑换鱼宴邀请函";
	table.insert(Npc_Inn, action);
end


--春节活动
--local stage = getActivityStage();
--if stage > 0 and stage < 4 then	
--	local action = ActionTable:Instance();
--	action.m_ActionType = 0x30;
--	action.m_ActionToken = 1;
--	action.m_ActionID = 7;
--	action.m_ActionStep = 0;
--	action.m_ActionMsg = "兑换飞机票";
--	table.insert(Npc_Inn, action);
	
--	local action = ActionTable:Instance();
--	action.m_ActionType = 0x30;
--	action.m_ActionToken = 1;
--	action.m_ActionID = 6;
--	action.m_ActionStep = 0;
--	action.m_ActionMsg = "兑换火车票、汽车票、动车票";
--	table.insert(Npc_Inn, action);
--end

--圣诞元旦活动
local action = ActionTable:Instance();
action.m_ActionType = 0x30;
action.m_ActionToken = 1;
action.m_ActionID = 5;
action.m_ActionStep = 0;
action.m_ActionMsg = "兑换福星画像+禄星画像+寿星画像+三星高照";
--table.insert(Npc_Inn, action);

action = ActionTable:Instance();
action.m_ActionType = 0x30;
action.m_ActionToken = 1;
action.m_ActionID = 4;
action.m_ActionStep = 0;
action.m_ActionMsg = "兑换福星画像+禄星画像+寿星画像";
--table.insert(Npc_Inn, action);

action = ActionTable:Instance();
action.m_ActionType = 0x30;
action.m_ActionToken = 1;
action.m_ActionID = 3;
action.m_ActionStep = 0;
action.m_ActionMsg = "兑换生肖兔";
--table.insert(Npc_Inn, action);

-- 中秋活动
action = ActionTable:Instance();
action.m_ActionType = 0x30;
action.m_ActionToken = 1;
action.m_ActionID = 2;
action.m_ActionStep = 0;
action.m_ActionMsg = "兑换五角星荷包";
--table.insert(Npc_Inn, action);

action = ActionTable:Instance();
action.m_ActionType = 0x30;
action.m_ActionToken = 1;
action.m_ActionID = 1;
action.m_ActionStep = 0;
action.m_ActionMsg = "兑换月饼";
--table.insert(Npc_Inn, action);


local Npc_EquipStrength = {};
action = ActionTable:Instance();

--装备分解
action.m_ActionType = 0x010;
action.m_ActionToken = 0;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "装备分解";
table.insert(Npc_EquipStrength, action);

--装备强化
action = ActionTable:Instance();
action.m_ActionType = 0x011;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "装备强化";
table.insert(Npc_EquipStrength, action);



local Npc_GemMaster = {};
action = ActionTable:Instance();

--[[
--装备打孔
action.m_ActionType = 0x012;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "装备打孔";
table.insert(Npc_GemMaster, action);
]]--

--宝石镶嵌
action = ActionTable:Instance();
action.m_ActionType = 0x013;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "宝石镶嵌";
table.insert(Npc_GemMaster, action);

--宝石合成
action = ActionTable:Instance();
action.m_ActionType = 0x014;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "宝石合成";
table.insert(Npc_GemMaster, action);

--宝石拆卸
action = ActionTable:Instance();
action.m_ActionType = 0x015;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "宝石拆卸";
table.insert(Npc_GemMaster, action);



local Npc_EquipForge = {};
action = ActionTable:Instance();

--装备洗练
action.m_ActionType = 0x016;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "装备洗炼";
table.insert(Npc_EquipForge, action);

action = ActionTable:Instance();
action.m_ActionType = 0x021;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "属性激活";
table.insert(Npc_EquipForge, action);



local Npc_EquipReplace = {};
action = ActionTable:Instance();
action.m_ActionType = 0x017;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "装备置换";
table.insert(Npc_EquipReplace, action);

action = ActionTable:Instance();
action.m_ActionType = 0x018;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "部位置换";
table.insert(Npc_EquipReplace, action);



local Npc_Train = {};
action = ActionTable:Instance();
action.m_ActionType = 0x020;
action.m_ActionToken = 4;
action.m_ActionID = 0;
action.m_ActionStep = 0;
action.m_ActionMsg = "潜力洗炼";
table.insert(Npc_Train, action);


local NpcDefaultActionTable = {
	--中秋活动
	[236+4096] = Npc_Inn,
	[237+4096] = Npc_Inn,
	--装备强化
	[225+4096] = Npc_EquipStrength,
	[229+4096] = Npc_EquipStrength,
	[240+4096] = Npc_EquipStrength,
	--宝石大师
	[226+4096] = Npc_GemMaster,
	[230+4096] = Npc_GemMaster,
	[241+4096] = Npc_GemMaster,
	--装备锻造
	[227+4096] = Npc_EquipForge,
	[231+4096] = Npc_EquipForge,
	[242+4096] = Npc_EquipForge,
	--装备置换
	[228+4096] = Npc_EquipReplace,
	[232+4096] = Npc_EquipReplace,
	[243+4096] = Npc_EquipReplace,
	--装备置换
	[238+4096] = Npc_Train,
	[239+4096] = Npc_Train,
	};

function RunNpcDefaultAction(npcId)
	if NpcDefaultActionTable[npcId] == nil then
		return {};
	end
	return NpcDefaultActionTable[npcId];
end

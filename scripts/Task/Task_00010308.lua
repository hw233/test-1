--����Ľ�������
function Task_Accept_00010308()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10308) or task:HasCompletedTask(10308) or task:HasSubmitedTask(10308) then
		return false;
	end
	if not task:HasSubmitedTask(10307) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010308()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10308) or task:HasCompletedTask(10308) or task:HasSubmitedTask(10308) then
		return false;
	end
	if not task:HasSubmitedTask(10307) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010308()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10308) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010308(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10308) == npcId and Task_Accept_00010308 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10308
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "战斗的成长";
	elseif task:GetTaskSubmitNpc(10308) == npcId then
		if Task_Submit_00010308() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10308
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "战斗的成长";
		elseif task:HasAcceptedTask(10308) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10308
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "战斗的成长";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010308_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我的勇士，你不是去见那个神秘人了吗？他应该是没有威胁的吧，你怎么会受了这么重的伤？";
	action.m_ActionMsg = "我去打瓦萨部落的食人族，可他们太厉害了，我也受了伤。";
	return action;
end

function Task_00010308_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是这样，我想我有责任指导你如何获得更加强大的力量。找到阵型，然后然后选择一个3人阵型，以后再遇到危险的情景，这个阵型会帮助你获得更好的攻防能力。现在回到前面让你损伤惨重的食人族那里，再次把他们打败，练习这个阵型的使用吧。";
	action.m_ActionMsg = "好的，我来试试阵型到底有多厉害的效果。";
	return action;
end

function Task_00010308_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士很高兴看到你实力的成长。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010308_step_table = {
		[1] = Task_00010308_step_01,
		[2] = Task_00010308_step_02,
		[10] = Task_00010308_step_10,
		};

function Task_00010308_step(step)
	if Task_00010308_step_table[step] ~= nil then
		return Task_00010308_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010308_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010308() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10308) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00010308_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8911,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10308) then
		return false;
	end

	if IsEquipTypeId(8911) then
		for k = 1, 1 do
			package:AddEquip(8911, 1);
		end
	else 
		package:AddItem(8911,1,1);
	end

	player:AddExp(1800);
	player:getCoin(2900);
	return true;
end

--��������
function Task_00010308_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10308);
end

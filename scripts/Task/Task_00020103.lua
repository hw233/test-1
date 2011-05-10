--����Ľ�������
function Task_Accept_00020103()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20103) or task:HasCompletedTask(20103) or task:HasSubmitedTask(20103) then
		return false;
	end
	if not task:HasSubmitedTask(20102) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(20103) or task:HasCompletedTask(20103) or task:HasSubmitedTask(20103) then
		return false;
	end
	if not task:HasSubmitedTask(20102) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20103) == npcId and Task_Accept_00020103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "勇气的试炼";
	elseif task:GetTaskSubmitNpc(20103) == npcId then
		if Task_Submit_00020103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "勇气的试炼";
		elseif task:HasAcceptedTask(20103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "勇气的试炼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好年轻的勇士，很高兴我们魔族的名将李清照向我引荐了你，想在我们魔族出人头地，不是那么容易的事情，只有真正有天赋和实力的人，才能最终脱颖而出。";
	action.m_ActionMsg = "你怎么知道我不是这样的人。";
	return action;
end

function Task_00020103_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我现在让你去打败我们魔族的战神因达罗的影子，证明你不仅有足够的勇气，而且是有相当的潜力，能够征战在这个世界里，如果你做到了，就去旁边的七叶堂拜见我们的雷神罗睺罗。";
	action.m_ActionMsg = "只是影子而已，对我来说不在话下，我这就去。";
	return action;
end

function Task_00020103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻的勇士，你做的非常出色。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020103_step_table = {
		[1] = Task_00020103_step_01,
		[2] = Task_00020103_step_02,
		[10] = Task_00020103_step_10,
		};

function Task_00020103_step(step)
	if Task_00020103_step_table[step] ~= nil then
		return Task_00020103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20103) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00020103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(3,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20103) then
		return false;
	end

	if IsEquipTypeId(3) then
		for k = 1, 1 do
			package:AddEquip(3, 1);
		end
	else 
		package:AddItem(3,1,1);
	end

	player:AddExp(700);
	player:getCoin(600);
	player:getTael(5);
	return true;
end

--��������
function Task_00020103_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20103);
end

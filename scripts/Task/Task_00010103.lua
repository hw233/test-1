--����Ľ�������
function Task_Accept_00010103()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10103) or task:HasCompletedTask(10103) or task:HasSubmitedTask(10103) then
		return false;
	end
	if not task:HasSubmitedTask(10102) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(10103) or task:HasCompletedTask(10103) or task:HasSubmitedTask(10103) then
		return false;
	end
	if not task:HasSubmitedTask(10102) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10103) == npcId and Task_Accept_00010103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "勇气的试炼";
	elseif task:GetTaskSubmitNpc(10103) == npcId then
		if Task_Submit_00010103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "勇气的试炼";
		elseif task:HasAcceptedTask(10103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10103
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
function Task_00010103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好年轻的勇士，很高兴我们天族的名将郭嘉向我引荐了你，想在我们天族出人头地，不是那么容易的事情，只有真正有天赋和实力的人，才能最终脱颖而出。";
	action.m_ActionMsg = "你怎么知道我不是这样的人。";
	return action;
end

function Task_00010103_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我现在让你去打败我们天族的战神忉利天的影子，证明你不仅有足够的勇气，而且是有相当的潜力，能够征战在这个世界里，如果你做到了，就去旁边的善法堂拜见我们的护法天神梵天。";
	action.m_ActionMsg = "我这就去，只是影子而已，对我来说肯定不在话下。";
	return action;
end

function Task_00010103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻的勇士，你做的非常出色。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010103_step_table = {
		[1] = Task_00010103_step_01,
		[2] = Task_00010103_step_02,
		[10] = Task_00010103_step_10,
		};

function Task_00010103_step(step)
	if Task_00010103_step_table[step] ~= nil then
		return Task_00010103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10103) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00010103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8908,1,1) + package:GetItemUsedGrids(3,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10103) then
		return false;
	end

	if IsEquipTypeId(8908) then
		for k = 1, 1 do
			package:AddEquip(8908, 1);
		end
	else 
		package:AddItem(8908,1,1);
	end
	if IsEquipTypeId(3) then
		for k = 1, 1 do
			package:AddEquip(3, 1);
		end
	else 
		package:AddItem(3,1,1);
	end

	player:AddExp(500);
	player:getCoin(600);
	return true;
end

--��������
function Task_00010103_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10103);
end

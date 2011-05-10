--����Ľ�������
function Task_Accept_00080042()
	local player = GetPlayer();
	if player:GetLev() < 42 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80042) or task:HasCompletedTask(80042) or task:HasSubmitedTask(80042) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080042()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 42 then
		return false;
	end
	if task:HasAcceptedTask(80042) or task:HasCompletedTask(80042) or task:HasSubmitedTask(80042) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080042()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80042) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080042(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80042) == npcId and Task_Accept_00080042 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80042
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "不懂幽默";
	elseif task:GetTaskSubmitNpc(80042) == npcId then
		if Task_Submit_00080042() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80042
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "不懂幽默";
		elseif task:HasAcceptedTask(80042) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80042
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "不懂幽默";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080042_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，我们急需你的帮助。山海大陆的想必你已经有所了解。就说我们附近的烟雨林这里，最近真是越来越不太平了，那些幽默狼人实在是太让人无法忍受了，我们现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080042_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。恭喜你进入更高级的修炼阶段。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080042_step_table = {
		[1] = Task_00080042_step_01,
		[10] = Task_00080042_step_10,
		};

function Task_00080042_step(step)
	if Task_00080042_step_table[step] ~= nil then
		return Task_00080042_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080042_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080042() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80042) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080042_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8934,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80042) then
		return false;
	end

	if IsEquipTypeId(8934) then
		for k = 1, 1 do
			package:AddEquip(8934, 1);
		end
	else 
		package:AddItem(8934,1,1);
	end

	player:AddExp(6000);
	return true;
end

--��������
function Task_00080042_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80042);
end

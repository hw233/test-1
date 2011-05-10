--����Ľ�������
function Task_Accept_00050404()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50404) or task:HasCompletedTask(50404) or task:HasSubmitedTask(50404) then
		return false;
	end
	if not task:HasSubmitedTask(50403) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050404()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(50404) or task:HasCompletedTask(50404) or task:HasSubmitedTask(50404) then
		return false;
	end
	if not task:HasSubmitedTask(50403) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050404()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50404) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050404(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50404) == npcId and Task_Accept_00050404 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50404
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "炼出丹药";
	elseif task:GetTaskSubmitNpc(50404) == npcId then
		if Task_Submit_00050404() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50404
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "炼出丹药";
		elseif task:HasAcceptedTask(50404) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50404
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "炼出丹药";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050404_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "有了你刚刚收集到的邪士之血，要炼制出绝世的长生丹药已经不成问题了。";
	action.m_ActionMsg = "那为什么还不快给我呢？";
	return action;
end

function Task_00050404_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这种丹药可是急不得，不仅炼制急不得，尤其是使用的时候急不得，我现在把炼好的丹药给你，可是你万万不能自己服用，而要先去蒲昌海的真寂殿，去请一峰道长指导你如何应对这丹药。";
	action.m_ActionMsg = "怎么这么麻烦？不就是颗丹药嘛.";
	return action;
end

function Task_00050404_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "显无意都已经帮你炼好丹药了啊，你对这个事情你还真是挺执着的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050404_step_table = {
		[1] = Task_00050404_step_01,
		[2] = Task_00050404_step_02,
		[10] = Task_00050404_step_10,
		};

function Task_00050404_step(step)
	if Task_00050404_step_table[step] ~= nil then
		return Task_00050404_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050404_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050404() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15050, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(50404) then
		return false;
	end
	task:AddTaskStep(50404);
	return true;
end



--�ύ����
function Task_00050404_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15050,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50404) then
		return false;
	end

	package:DelItemAll(15050,1);

	player:AddExp(1200);
	player:getCoin(1150);
	return true;
end

--��������
function Task_00050404_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15050,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50404);
end

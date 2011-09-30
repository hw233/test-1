--����Ľ�������
function Task_Accept_00000065()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(65) or task:HasCompletedTask(65) or task:HasSubmitedTask(65) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000065()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(65) or task:HasCompletedTask(65) or task:HasSubmitedTask(65) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000065()
	if GetPlayer():GetTaskMgr():HasCompletedTask(65) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000065(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(65) == npcId and Task_Accept_00000065 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 65
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "炼药之错";
	elseif task:GetTaskSubmitNpc(65) == npcId then
		if Task_Submit_00000065() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 65
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "炼药之错";
		elseif task:HasAcceptedTask(65) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 65
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "炼药之错";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000065_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我在长沙有个小友叫黄玄极，昔年因为道根不净，走火入魔崩倒了玄真子的两座丹炉而被逐出门墙。这几年里他一意苦修，走遍各处名山，想将药材采办齐全，再造一炉丹药，如今就差一味乌风草。现在草药还有点剩余，不如你就把这剩下的乌风草送给他吧。";
	action.m_ActionMsg = "这位黄大哥现在哪里呢？";
	return action;
end

function Task_00000065_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "黄小友目前在岳麓山一带逗留，你过去找找他。";
	action.m_ActionMsg = "好的，晚辈这就去寻他。";
	return action;
end

function Task_00000065_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "啊，这就是我遍寻名山找不到的乌风草！真是太感谢你了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000065_step_table = {
		[1] = Task_00000065_step_01,
		[2] = Task_00000065_step_02,
		[10] = Task_00000065_step_10,
		};

function Task_00000065_step(step)
	if Task_00000065_step_table[step] ~= nil then
		return Task_00000065_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000065_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000065() then
		return false;
	end
	local package = player:GetPackage();
	local reqGrids = 0;
	reqGrids = reqGrids + package:GetItemUsedGrids(807, 1, 1);
	if reqGrids > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1012, 0);
		return false;
	end
	if not task:AcceptTask(65) then
		return false;
	end
	package:AddItem(807, 1, 1);
	task:AddTaskStep(65);
	return true;
end



--�ύ����
function Task_00000065_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(807,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(65) then
		return false;
	end

	package:DelItem(807,1,1);

	player:AddExp(10000);
	return true;
end

--��������
function Task_00000065_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItem(807, 1, 1);
	local itemNum = 0;
	return GetPlayer():GetTaskMgr():AbandonTask(65);
end

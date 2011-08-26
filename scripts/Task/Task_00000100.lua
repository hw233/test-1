--����Ľ�������
function Task_Accept_00000100()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(100) or task:HasCompletedTask(100) or task:HasSubmitedTask(100) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(99) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(99) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(99) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000100()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(100) or task:HasCompletedTask(100) or task:HasSubmitedTask(100) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(99) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(99) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(99) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000100()
	if GetPlayer():GetTaskMgr():HasCompletedTask(100) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000100(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(100) == npcId and Task_Accept_00000100 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 100
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "玉露符";
	elseif task:GetTaskSubmitNpc(100) == npcId then
		if Task_Submit_00000100() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 100
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "玉露符";
		elseif task:HasAcceptedTask(100) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 100
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "玉露符";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000100_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我昔年与乙休为一件事情反目，将躯壳萎化，隐居在这白犀潭已有数年，不问世事已久。白云大师元敬与我是近邻，倒也有点往来，这玉露符乃是聚宫阙之下地根泉眼的灵气而化的灵符，却不是什么稀罕之物，你这就带去给大师吧。";
	action.m_ActionMsg = "好的，韩前辈我这就去。";
	return action;
end

function Task_00000100_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有了这玉露符，净化姑婆岭的瘴气要轻松许多了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000100_step_table = {
		[1] = Task_00000100_step_01,
		[10] = Task_00000100_step_10,
		};

function Task_00000100_step(step)
	if Task_00000100_step_table[step] ~= nil then
		return Task_00000100_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000100_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000100() then
		return false;
	end
	if not task:AcceptTask(100) then
		return false;
	end
	task:AddTaskStep(100);
	return true;
end



--�ύ����
function Task_00000100_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(100) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--��������
function Task_00000100_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(100);
end

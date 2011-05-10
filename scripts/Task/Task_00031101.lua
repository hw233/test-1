--����Ľ�������
function Task_Accept_00031101()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 57 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31101) or task:HasCompletedTask(31101) or task:HasSubmitedTask(31101) then
		return false;
	end
	if not task:HasSubmitedTask(30906) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 57 then
		return false;
	end
	if task:HasAcceptedTask(31101) or task:HasCompletedTask(31101) or task:HasSubmitedTask(31101) then
		return false;
	end
	if not task:HasSubmitedTask(30906) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31101) == npcId and Task_Accept_00031101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "如烟随从";
	elseif task:GetTaskSubmitNpc(31101) == npcId then
		if Task_Submit_00031101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "如烟随从";
		elseif task:HasAcceptedTask(31101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "如烟随从";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好，很高兴在这茫茫的山海大陆遇到一位来自天族的勇士。";
	action.m_ActionMsg = "你好，有什么可以帮你的吗？";
	return action;
end

function Task_00031101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我是我们蒙不凡大人的夫人如烟的随从，可是几天前我们的夫人走失了，我们都找不到她，这么多天过去了我们都很担心，我希望你能帮助我们，你去找旁边的蒙不凡大人问问吧。";
	action.m_ActionMsg = "怎么突然发生这种事，其中肯定有问题.";
	return action;
end

function Task_00031101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "太好了，总算让我等到了一个能来救助我的人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031101_step_table = {
		[1] = Task_00031101_step_01,
		[2] = Task_00031101_step_02,
		[10] = Task_00031101_step_10,
		};

function Task_00031101_step(step)
	if Task_00031101_step_table[step] ~= nil then
		return Task_00031101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31101) then
		return false;
	end
	task:AddTaskStep(31101);
	return true;
end



--�ύ����
function Task_00031101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31101) then
		return false;
	end


	player:AddExp(84000);
	player:getCoin(84000);
	player:getTael(20);
	return true;
end

--��������
function Task_00031101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31101);
end

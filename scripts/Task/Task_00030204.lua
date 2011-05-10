--����Ľ�������
function Task_Accept_00030204()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30204) or task:HasCompletedTask(30204) or task:HasSubmitedTask(30204) then
		return false;
	end
	if not task:HasSubmitedTask(30203) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(30204) or task:HasCompletedTask(30204) or task:HasSubmitedTask(30204) then
		return false;
	end
	if not task:HasSubmitedTask(30203) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30204) == npcId and Task_Accept_00030204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "慰问边防";
	elseif task:GetTaskSubmitNpc(30204) == npcId then
		if Task_Submit_00030204() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30204
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "慰问边防";
		elseif task:HasAcceptedTask(30204) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30204
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "慰问边防";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我们击退那些疯狂的天族复仇者，也算是个好消息，你去边防那里告知士兵们这个消息吧，顺便再带去一些他们需要的物资，帮助他们更好的防守我们的势力。";
	action.m_ActionMsg = "好的，我现在去。";
	return action;
end

function Task_00030204_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士当您离开这里以后，山海大陆会有各种险恶的挑战等着您，提升自己的实力一定是最重要的，我们在这里也帮不了您更多，带着这封介绍信去阿兰若山谷的万人墓，找那里的守墓人吧，他是一个古怪的老头，没有人介绍他不会理您的，可是他却是一个高深莫测的人，会对您在山海大陆的旅程有所帮助的。";
	action.m_ActionMsg = "好的";
	return action;
end

function Task_00030204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "感谢您带来的消息和物资。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030204_step_table = {
		[1] = Task_00030204_step_01,
		[2] = Task_00030204_step_02,
		[10] = Task_00030204_step_10,
		};

function Task_00030204_step(step)
	if Task_00030204_step_table[step] ~= nil then
		return Task_00030204_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030204() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15032, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30204) then
		return false;
	end
	task:AddTaskStep(30204);
	return true;
end



--�ύ����
function Task_00030204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15032,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30204) then
		return false;
	end

	package:DelItemAll(15032,1);

	player:AddExp(4000);
	player:getCoin(5500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030204_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15032,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30204);
end

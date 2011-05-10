--����Ľ�������
function Task_Accept_00090115()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90115) or task:HasCompletedTask(90115) or task:HasSubmitedTask(90115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090115()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90115) or task:HasCompletedTask(90115) or task:HasSubmitedTask(90115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90114) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090115()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90115) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090115(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90115) == npcId and Task_Accept_00090115 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90115
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "好酒配英雄";
	elseif task:GetTaskSubmitNpc(90115) == npcId then
		if Task_Submit_00090115() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90115
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "好酒配英雄";
		elseif task:HasAcceptedTask(90115) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90115
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "好酒配英雄";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090115_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "真是可惜，只剩这一坛了。";
	action.m_ActionMsg = "我去的时候那些怪物正在饮酒作乐，看来这酒味道肯定不错。";
	return action;
end

function Task_00090115_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是呀，我费了很多心思酿制这些美酒。唉，那请勇士将这最后一坛美酒送与吕布喝吧。";
	action.m_ActionMsg = "他肯定会很高兴的。";
	return action;
end

function Task_00090115_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "好酒不怕巷子深。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090115_step_table = {
		[1] = Task_00090115_step_01,
		[2] = Task_00090115_step_02,
		[10] = Task_00090115_step_10,
		};

function Task_00090115_step(step)
	if Task_00090115_step_table[step] ~= nil then
		return Task_00090115_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090115_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090115() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15105, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(90115) then
		return false;
	end
	task:AddTaskStep(90115);
	return true;
end



--�ύ����
function Task_00090115_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15105,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90115) then
		return false;
	end

	package:DelItemAll(15105,1);

	player:AddExp(625000);
	player:getCoin(310000);
	return true;
end

--��������
function Task_00090115_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15105,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90115);
end

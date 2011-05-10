--����Ľ�������
function Task_Accept_00031404()
	local player = GetPlayer();
	if player:GetLev() < 64 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31404) or task:HasCompletedTask(31404) or task:HasSubmitedTask(31404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31403) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031404()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 64 then
		return false;
	end
	if task:HasAcceptedTask(31404) or task:HasCompletedTask(31404) or task:HasSubmitedTask(31404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31403) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031404()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31404) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031404(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31404) == npcId and Task_Accept_00031404 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31404
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "敢死将军";
	elseif task:GetTaskSubmitNpc(31404) == npcId then
		if Task_Submit_00031404() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31404
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "敢死将军";
		elseif task:HasAcceptedTask(31404) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31404
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "敢死将军";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031404_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."你真是太厉害了，居然在那么多狼人守卫的把守下还拿到了碎心塔的钥匙。";
	action.m_ActionMsg = "您过奖了，是镇邪派的严中帮我做到的。";
	return action;
end

function Task_00031404_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在这之前，我曾经多次派手下试图突破狼人守卫的防守而获得碎心塔的钥匙，但是每次都以失败告终，这次你真是帮了我们的大忙了。";
	action.m_ActionMsg = "不必客气。这还是多亏了严中，您要谢就谢他吧。";
	return action;
end

function Task_00031404_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，经常听说你的事迹，现在见到真人了很荣幸啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031404_step_table = {
		[1] = Task_00031404_step_01,
		[2] = Task_00031404_step_02,
		[10] = Task_00031404_step_10,
		};

function Task_00031404_step(step)
	if Task_00031404_step_table[step] ~= nil then
		return Task_00031404_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031404_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031404() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15073, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(31404) then
		return false;
	end
	task:AddTaskStep(31404);
	return true;
end



--�ύ����
function Task_00031404_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15073,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(31404) then
		return false;
	end

	package:DelItemAll(15073,1);

	player:AddExp(170000);
	player:getCoin(145000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031404_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15073,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31404);
end

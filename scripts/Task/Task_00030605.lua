--����Ľ�������
function Task_Accept_00030605()
	local player = GetPlayer();
	if player:GetLev() < 44 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30605) or task:HasCompletedTask(30605) or task:HasSubmitedTask(30605) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30604) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30604) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030605()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 44 then
		return false;
	end
	if task:HasAcceptedTask(30605) or task:HasCompletedTask(30605) or task:HasSubmitedTask(30605) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30604) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30604) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030605()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30605) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030605(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30605) == npcId and Task_Accept_00030605 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30605
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "闯入摩加塔";
	elseif task:GetTaskSubmitNpc(30605) == npcId then
		if Task_Submit_00030605() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30605
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "闯入摩加塔";
		elseif task:HasAcceptedTask(30605) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30605
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "闯入摩加塔";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030605_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "看来你做的不错，为我们收服了焚尸怪，哈哈哈，我们的势力再次扩大了。";
	action.m_ActionMsg = "请问有医官吗？我还是觉得头晕。";
	return action;
end

function Task_00030605_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻人别急嘛，医官很快就来，与其在这里等不如你再去帮我做一件事情，西边摩加河边的摩加塔里聚集了一些邪恶的守卫，去帮我们把他们赶走吧，说不定就是他们阻碍了医官及时赶来。";
	action.m_ActionMsg = "只好这样了。";
	return action;
end

function Task_00030605_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你好，是来帮助我们的吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030605_step_table = {
		[1] = Task_00030605_step_01,
		[2] = Task_00030605_step_02,
		[10] = Task_00030605_step_10,
		};

function Task_00030605_step(step)
	if Task_00030605_step_table[step] ~= nil then
		return Task_00030605_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030605_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030605() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30605) then
		return false;
	end
	task:AddTaskStep(30605);
	return true;
end



--�ύ����
function Task_00030605_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(301,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30605) then
		return false;
	end

	if IsEquipTypeId(301) then
		for k = 1, 1 do
			package:AddEquip(301, 1);
		end
	else 
		package:AddItem(301,1,1);
	end

	player:AddExp(32000);
	player:getCoin(25000);
	return true;
end

--��������
function Task_00030605_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30605);
end

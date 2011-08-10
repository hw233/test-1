--����Ľ�������
function Task_Accept_00000051()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(51) or task:HasCompletedTask(51) or task:HasSubmitedTask(51) then
		return false;
	end
	if not task:HasSubmitedTask(70) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000051()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(51) or task:HasCompletedTask(51) or task:HasSubmitedTask(51) then
		return false;
	end
	if not task:HasSubmitedTask(70) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000051()
	if GetPlayer():GetTaskMgr():HasCompletedTask(51) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000051(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(51) == npcId and Task_Accept_00000051 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 51
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "掌教真人";
	elseif task:GetTaskSubmitNpc(51) == npcId then
		if Task_Submit_00000051() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 51
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "掌教真人";
		elseif task:HasAcceptedTask(51) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 51
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "掌教真人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000051_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "慈云寺一战已经终了，正道中人都齐聚蜀山，"..GetPlayerName(GetPlayer()).."你也一起去吧。峨眉掌教乾坤正气妙一真人齐前辈正在峨眉派的玉清殿等着见见你这个正道的后起之秀呢。";
	action.m_ActionMsg = "嗯，我这就赶去。";
	return action;
end

function Task_00000051_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."正道中兴的重担就在你们这小一辈的肩上了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000051_step_table = {
		[1] = Task_00000051_step_01,
		[10] = Task_00000051_step_10,
		};

function Task_00000051_step(step)
	if Task_00000051_step_table[step] ~= nil then
		return Task_00000051_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000051_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000051() then
		return false;
	end
	if not task:AcceptTask(51) then
		return false;
	end
	task:AddTaskStep(51);
	return true;
end



--�ύ����
function Task_00000051_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(51) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--��������
function Task_00000051_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(51);
end

--����Ľ�������
function Task_Accept_00000527()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(527) or task:HasCompletedTask(527) or task:HasSubmitedTask(527) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000527()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(527) or task:HasCompletedTask(527) or task:HasSubmitedTask(527) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000527()
	if GetPlayer():GetTaskMgr():HasCompletedTask(527) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000527(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(527) == npcId and Task_Accept_00000527 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 527
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "蛇妖之祸";
	elseif task:GetTaskSubmitNpc(527) == npcId then
		if Task_Submit_00000527() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 527
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "蛇妖之祸";
		elseif task:HasAcceptedTask(527) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 527
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "蛇妖之祸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000527_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都郊外的云灵山有一只蛇妖盘踞，经常祸害过往行人。这只蛇妖惯于喷涂剧毒，成都知府及其手下的衙役捕快拿它毫无办法，所以上我们峨眉求助，"..GetPlayerName(GetPlayer()).."你去将这只妖物消灭吧。\n";
	action.m_ActionMsg = "为民除害是我辈本色，弟子去去就回。";
	return action;
end

function Task_00000527_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "除了此蛇妖，也可以保成都附近百姓的安宁。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000527_step_table = {
		[1] = Task_00000527_step_01,
		[10] = Task_00000527_step_10,
		};

function Task_00000527_step(step)
	if Task_00000527_step_table[step] ~= nil then
		return Task_00000527_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000527_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000527() then
		return false;
	end
	if not task:AcceptTask(527) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000527_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(527) then
		return false;
	end


	return true;
end

--��������
function Task_00000527_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(527);
end
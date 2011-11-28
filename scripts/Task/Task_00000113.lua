--����Ľ�������
function Task_Accept_00000113()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(113) or task:HasCompletedTask(113) or task:HasSubmitedTask(113) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000113()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(113) or task:HasCompletedTask(113) or task:HasSubmitedTask(113) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000113()
	if GetPlayer():GetTaskMgr():HasCompletedTask(113) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000113(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(113) == npcId and Task_Accept_00000113 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 113
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "莽苍山";
	elseif task:GetTaskSubmitNpc(113) == npcId then
		if Task_Submit_00000113() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 113
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "莽苍山";
		elseif task:HasAcceptedTask(113) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 113
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "莽苍山";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000113_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如今绿袍伏诛，我即日就与东海老友会山炼制一宗法宝，"..GetPlayerName(GetPlayer()).."你不如去莽苍山一趟，李英琼的那只爱宠袁星前不久在莽苍山走失，不过我用紫薇卦术推测，目前袁星正在马熊洞中，不如你去将它寻会，";
	action.m_ActionMsg = "好的，弟子这就去。";
	return action;
end

function Task_00000113_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你小子还真是阴魂不散，走到哪都看得到你！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000113_step_table = {
		[1] = Task_00000113_step_01,
		[10] = Task_00000113_step_10,
		};

function Task_00000113_step(step)
	if Task_00000113_step_table[step] ~= nil then
		return Task_00000113_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000113_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000113() then
		return false;
	end
	if not task:AcceptTask(113) then
		return false;
	end
	task:AddTaskStep(113);
	return true;
end



--�ύ����
function Task_00000113_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(113) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--��������
function Task_00000113_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(113);
end

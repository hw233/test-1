--����Ľ�������
function Task_Accept_00050304()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50304) or task:HasCompletedTask(50304) or task:HasSubmitedTask(50304) then
		return false;
	end
	if not task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(50304) or task:HasCompletedTask(50304) or task:HasSubmitedTask(50304) then
		return false;
	end
	if not task:HasSubmitedTask(50203) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50304) == npcId and Task_Accept_00050304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "招募张角";
	elseif task:HasAcceptedTask(50304) and npcId == 139 then
		RunAutoBattleAction(player, 139, -1);
	elseif task:GetTaskSubmitNpc(50304) == npcId and Task_Submit_00050304() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50304
		action.m_ActionToken = 2;
		action.m_ActionStep = 10;
		action.m_ActionMsg = "招募张角";
	end
	
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "拥有了刺客和谋士之后，你有没有明显感到实力大增啊？这就是队伍的力量，其实以你现在的统领能力还可以再添一员";
	action.m_ActionMsg = "是嘛，请你指点我。";
	return action;
end

function Task_00050304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在的你还缺少一名猛将，去试着挑战镇前所的张角吧。";
	action.m_ActionMsg = "事不宜迟，我现在就去。";
	return action;
end

function Task_00050304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在下愿效犬马之劳。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050304_step_table = {
		[1] = Task_00050304_step_01,
		[2] = Task_00050304_step_02,
		[10] = Task_00050304_step_10,
		};

function Task_00050304_step(step)
	if Task_00050304_step_table[step] ~= nil then
		return Task_00050304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50304) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00050304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	
	if player:isFighterFull() then
		player:sendMsgCode(0, 2006);
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50304) then
		return false;
	end
	player:takeFighter(139, true);

	player:AddExp(3000);
	player:getCoin(10000);
	player:getTael(150);

	return true;
end

--��������
function Task_00050304_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(50304);
end

--����Ľ�������
function Task_Accept_00000061()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61) or task:HasCompletedTask(61) or task:HasSubmitedTask(61) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000061()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(61) or task:HasCompletedTask(61) or task:HasSubmitedTask(61) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000061()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000061(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61) == npcId and Task_Accept_00000061 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "邪派妖道";
	elseif task:GetTaskSubmitNpc(61) == npcId then
		if Task_Submit_00000061() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "邪派妖道";
		elseif task:HasAcceptedTask(61) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "邪派妖道";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000061_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "此去往南方不远，有个极阴洞，里面住着一个邪派妖道名叫鬼道人乔瘦腾，四处劫掠美貌的妇人回去练功。";
	action.m_ActionMsg = "这个妖道真是可恶！";
	return action;
end

function Task_00000061_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "听家师说他是阴阳叟的师弟，本领高强。以前小妹一人时，情知不是他的对手，只是在暗中留意。如今有<name>你的帮助，我想应该不难消灭这个妖人。";
	action.m_ActionMsg = "那就一起去吧。";
	return action;
end

function Task_00000061_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>你真是好身手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000061_step_table = {
		[1] = Task_00000061_step_01,
		[2] = Task_00000061_step_02,
		[10] = Task_00000061_step_10,
		};

function Task_00000061_step(step)
	if Task_00000061_step_table[step] ~= nil then
		return Task_00000061_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000061_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000061() then
		return false;
	end
	if not task:AcceptTask(61) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000061_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61) then
		return false;
	end


	player:AddExp(6666);
	return true;
end

--��������
function Task_00000061_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(61);
end

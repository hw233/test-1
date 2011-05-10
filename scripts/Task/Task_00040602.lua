--����Ľ�������
function Task_Accept_00040602()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40602) or task:HasCompletedTask(40602) or task:HasSubmitedTask(40602) then
		return false;
	end
	if not task:HasSubmitedTask(40601) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040602()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(40602) or task:HasCompletedTask(40602) or task:HasSubmitedTask(40602) then
		return false;
	end
	if not task:HasSubmitedTask(40601) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040602()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40602) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040602(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40602) == npcId and Task_Accept_00040602 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40602
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "转机";
	elseif task:GetTaskSubmitNpc(40602) == npcId then
		if Task_Submit_00040602() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40602
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "转机";
		elseif task:HasAcceptedTask(40602) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40602
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "转机";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040602_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你居然会来这里找我真是太好，我还以为我都被人遗忘了，我到这里的这段时间，几乎一个人都不认识，在天庭唯一认识的一个人智然也没有消息。";
	action.m_ActionMsg = "其实就是智然让我来找你的，他还让我带来了给你写的信。";
	return action;
end

function Task_00040602_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯我已经看到了，麻烦不断的这段生活终于出现了转机，但是我还有一个麻烦，就是我最近不断的受到断头崖的一些奇怪的怪物的骚扰，可否拜托你去帮我解决他们。";
	action.m_ActionMsg = "没问题，我最善于解决这样的问题。";
	return action;
end

function Task_00040602_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你的行为真是太酷了，你会得到很棒的回报。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040602_step_table = {
		[1] = Task_00040602_step_01,
		[2] = Task_00040602_step_02,
		[10] = Task_00040602_step_10,
		};

function Task_00040602_step(step)
	if Task_00040602_step_table[step] ~= nil then
		return Task_00040602_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040602_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040602() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40602) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040602_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40602) then
		return false;
	end


	player:AddExp(1500);
	player:getCoin(1900);
	return true;
end

--��������
function Task_00040602_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40602);
end

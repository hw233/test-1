--����Ľ�������
function Task_Accept_00060602()
	local player = GetPlayer();
	if player:GetLev() < 41 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60602) or task:HasCompletedTask(60602) or task:HasSubmitedTask(60602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60601) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060602()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 41 then
		return false;
	end
	if task:HasAcceptedTask(60602) or task:HasCompletedTask(60602) or task:HasSubmitedTask(60602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60601) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060602()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60602) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060602(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60602) == npcId and Task_Accept_00060602 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60602
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "争取送镖";
	elseif task:GetTaskSubmitNpc(60602) == npcId then
		if Task_Submit_00060602() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60602
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "争取送镖";
		elseif task:HasAcceptedTask(60602) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60602
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "争取送镖";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060602_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "天武城主就这么信任你吗？他有没有跟说山海大陆的险恶啊，在这里押镖可不是那么容易的事情，你随时会遇到无法预知的危险，没有足够的实力，你根本没有办法完成这样的任务。";
	action.m_ActionMsg = "别小瞧人啊，要不要我证明给你看啊。";
	return action;
end

function Task_00060602_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "也好，那你去黑烟谷打败那些疯狂狼骑，如果你安然无恙的回来了，我才可以放心的将这些事情交给你。";
	action.m_ActionMsg = "放心吧，我肯定没有问题的。";
	return action;
end

function Task_00060602_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那些疯狂的狼骑居然这么轻松就被你打败了？真是好样的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060602_step_table = {
		[1] = Task_00060602_step_01,
		[2] = Task_00060602_step_02,
		[10] = Task_00060602_step_10,
		};

function Task_00060602_step(step)
	if Task_00060602_step_table[step] ~= nil then
		return Task_00060602_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060602_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060602() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60602) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00060602_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60602) then
		return false;
	end


	player:AddExp(12000);
	player:getCoin(15000);
	return true;
end

--��������
function Task_00060602_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60602);
end

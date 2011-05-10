--����Ľ�������
function Task_Accept_00060301()
	local player = GetPlayer();
	if player:GetLev() < 33 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60301) or task:HasCompletedTask(60301) or task:HasSubmitedTask(60301) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 33 then
		return false;
	end
	if task:HasAcceptedTask(60301) or task:HasCompletedTask(60301) or task:HasSubmitedTask(60301) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60301) == npcId and Task_Accept_00060301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "清锅冷灶";
	elseif task:GetTaskSubmitNpc(60301) == npcId then
		if Task_Submit_00060301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "清锅冷灶";
		elseif task:HasAcceptedTask(60301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "清锅冷灶";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位勇士，不容易看到这里有个人经过啊，是否有时间聊上几句啊？";
	action.m_ActionMsg = "有什么事情吗？";
	return action;
end

function Task_00060301_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不知道为啥我就来到了这个地方，记忆中我本来生活的繁荣的贵族家庭，没曾想现在居然在这么个凄凉冷清的地方，想说句话的人都没用，更不用说能有个人一起吃饭了。";
	action.m_ActionMsg = "这倒是，这个阿兰若山谷就是这样，破败的战场几乎没什么人了。";
	return action;
end

function Task_00060301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我说怎么到处都是阴凉的气氛，原来是古战场的遗址啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060301_step_table = {
		[1] = Task_00060301_step_01,
		[2] = Task_00060301_step_02,
		[10] = Task_00060301_step_10,
		};

function Task_00060301_step(step)
	if Task_00060301_step_table[step] ~= nil then
		return Task_00060301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60301) then
		return false;
	end
	task:AddTaskStep(60301);
	return true;
end



--�ύ����
function Task_00060301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60301) then
		return false;
	end


	player:AddExp(4500);
	player:getCoin(6150);
	return true;
end

--��������
function Task_00060301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60301);
end

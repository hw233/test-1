--����Ľ�������
function Task_Accept_00080801()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80801) or task:HasCompletedTask(80801) or task:HasSubmitedTask(80801) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80801) or task:HasCompletedTask(80801) or task:HasSubmitedTask(80801) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80801) == npcId and Task_Accept_00080801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "丑陋的蝴蝶";
	elseif task:GetTaskSubmitNpc(80801) == npcId then
		if Task_Submit_00080801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "丑陋的蝴蝶";
		elseif task:HasAcceptedTask(80801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "丑陋的蝴蝶";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你听说过枯木镇吗？那是一个悲哀的地方，极其凄凉。";
	action.m_ActionMsg = "是什么让那里变成了这样？";
	return action;
end

function Task_00080801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不知道，那里只有枯萎的树木和早已失去了血色的生命。估计是被诅咒了，你去结束它们痛苦的余生吧。";
	action.m_ActionMsg = "听来确实悲凉。";
	return action;
end

function Task_00080801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "悲剧的人生也是需要我们去经历的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080801_step_table = {
		[1] = Task_00080801_step_01,
		[2] = Task_00080801_step_02,
		[10] = Task_00080801_step_10,
		};

function Task_00080801_step(step)
	if Task_00080801_step_table[step] ~= nil then
		return Task_00080801_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80801) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80801) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--��������
function Task_00080801_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80801);
end

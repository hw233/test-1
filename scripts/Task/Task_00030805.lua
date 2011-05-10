--����Ľ�������
function Task_Accept_00030805()
	local player = GetPlayer();
	if player:GetLev() < 52 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30805) or task:HasCompletedTask(30805) or task:HasSubmitedTask(30805) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30804) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30804) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030805()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 52 then
		return false;
	end
	if task:HasAcceptedTask(30805) or task:HasCompletedTask(30805) or task:HasSubmitedTask(30805) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30804) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30804) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030805()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30805) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030805(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30805) == npcId and Task_Accept_00030805 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30805
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "装备完成";
	elseif task:GetTaskSubmitNpc(30805) == npcId then
		if Task_Submit_00030805() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30805
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "装备完成";
		elseif task:HasAcceptedTask(30805) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30805
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "装备完成";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030805_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "嗯，勇士，你到这里来，是为新近投靠了镇邪派的那位名将打造装备是吗？";
	action.m_ActionMsg = "正是。";
	return action;
end

function Task_00030805_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我也听说了，那位名将正是勇猛善战的大将赤佬温，他没有投靠我们东山派令我感到有点遗憾，但是能为他打造装备也是我的荣幸，没有问题，这就为他打造好，相信有了这样的装备，他定能发挥自己的实力，对于我们共同对抗夜摩盟也是有帮助的。";
	action.m_ActionMsg = "非常感谢您的帮助。";
	return action;
end

function Task_00030805_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "啊哈，真是一件帅气的龙甲啊！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030805_step_table = {
		[1] = Task_00030805_step_01,
		[2] = Task_00030805_step_02,
		[10] = Task_00030805_step_10,
		};

function Task_00030805_step(step)
	if Task_00030805_step_table[step] ~= nil then
		return Task_00030805_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030805_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030805() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15064, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30805) then
		return false;
	end
	task:AddTaskStep(30805);
	return true;
end



--�ύ����
function Task_00030805_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15064,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30805) then
		return false;
	end

	package:DelItemAll(15064,1);

	player:AddExp(64000);
	player:getCoin(53000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030805_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15064,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30805);
end

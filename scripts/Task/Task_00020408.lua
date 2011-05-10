--����Ľ�������
function Task_Accept_00020408()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20408) or task:HasCompletedTask(20408) or task:HasSubmitedTask(20408) then
		return false;
	end
	if not task:HasSubmitedTask(20407) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020408()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20408) or task:HasCompletedTask(20408) or task:HasSubmitedTask(20408) then
		return false;
	end
	if not task:HasSubmitedTask(20407) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020408()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20408) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020408(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20408) == npcId and Task_Accept_00020408 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20408
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "帝释天的阴谋";
	elseif task:GetTaskSubmitNpc(20408) == npcId then
		if Task_Submit_00020408() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20408
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "帝释天的阴谋";
		elseif task:HasAcceptedTask(20408) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20408
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "帝释天的阴谋";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020408_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你搜到了一封密信？我来看看这封信。……帝释天太无耻了，他不仅冷落公主，而且想要摧毁我们之前的讲和条件，想要减少甚至停止对我们甘露的供应，怪不得这次的甘露只有一半。而且派了这些刺客来到地界刺探我们情况。\n";
	action.m_ActionMsg = "帝释天怎么可以这样做！";
	return action;
end

function Task_00020408_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "帝释天一向是这样无耻的，他们的贪婪和狂妄，迟早会受到惩罚。既然仇恨愈演愈烈，我们时刻要准备战争的爆发，使自己变得更加强大起来，现在你去鬼蜮山的疏勒郡去找雄力，他会给你正确的指引。\n";
	action.m_ActionMsg = "那么再见了大龙王，我去鬼蜮山了。";
	return action;
end

function Task_00020408_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你是第一次来鬼蜮山吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020408_step_table = {
		[1] = Task_00020408_step_01,
		[2] = Task_00020408_step_02,
		[10] = Task_00020408_step_10,
		};

function Task_00020408_step(step)
	if Task_00020408_step_table[step] ~= nil then
		return Task_00020408_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020408_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020408() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20408) then
		return false;
	end
	task:AddTaskStep(20408);
	return true;
end



--�ύ����
function Task_00020408_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20408) then
		return false;
	end


	player:AddExp(2200);
	player:getCoin(4300);
	player:getTael(10);
	return true;
end

--��������
function Task_00020408_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20408);
end

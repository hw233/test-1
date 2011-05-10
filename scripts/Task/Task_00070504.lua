--����Ľ�������
function Task_Accept_00070504()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70504) or task:HasCompletedTask(70504) or task:HasSubmitedTask(70504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70503) then
			return false;
		end
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70504) or task:HasCompletedTask(70504) or task:HasSubmitedTask(70504) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70503) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00070504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70504) == npcId and Task_Accept_00070504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "有人的地方就有江湖";
	elseif task:GetTaskSubmitNpc(70504) == npcId then
		if Task_Submit_00070504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "有人的地方就有江湖";
		elseif task:HasAcceptedTask(70504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "有人的地方就有江湖";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "作为山河城的城主，我有责任维护整个山河城的安全，我不能让我们城内的人民受到任何威胁，尤其是来自阿兰若山谷战争废墟的怨灵鬼魂的威胁，勇士如果你愿意受雇于我，就去杀掉他们，如果我会给你一定的战功作为回报，你还会拒绝这个请求吗？";
	action.m_ActionMsg = "我会用战功向全族人证明，我是天族最勇猛的将士。";
	return action;
end

function Task_00070504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "什么是江湖？有人的地方就有江湖。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070504_step_table = {
		[1] = Task_00070504_step_01,
		[10] = Task_00070504_step_10,
		};

function Task_00070504_step(step)
	if Task_00070504_step_table[step] ~= nil then
		return Task_00070504_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70504) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(70504) then
		return false;
	end


	player:getAchievement(10);
	return true;
end

--��������
function Task_00070504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70504);
end

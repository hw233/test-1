--����Ľ�������
function Task_Accept_00070308()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70308) or task:HasCompletedTask(70308) or task:HasSubmitedTask(70308) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070308()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(70308) or task:HasCompletedTask(70308) or task:HasSubmitedTask(70308) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70308 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070308()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70308) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070308(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70308) == npcId and Task_Accept_00070308 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70308
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "以牙还牙以眼还眼";
	elseif task:GetTaskSubmitNpc(70308) == npcId then
		if Task_Submit_00070308() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70308
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "以牙还牙以眼还眼";
		elseif task:HasAcceptedTask(70308) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70308
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "以牙还牙以眼还眼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070308_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "雪边锋风木园的复仇鬼族，不仅利益熏心，而且被仇恨冲昏了头脑了，完全丧失理智了，他们只要一有机会就侵袭所有能打的过人，我们一定要以其人之道还治其人之身，让他们知道什么是对的，什么是错的。\n";
	action.m_ActionMsg = "我想他们已经疯了，再这样下去不是办法，我这就去收拾他们。";
	return action;
end

function Task_00070308_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "冲动是魔鬼，仇恨是魔鬼啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070308_step_table = {
		[1] = Task_00070308_step_01,
		[10] = Task_00070308_step_10,
		};

function Task_00070308_step(step)
	if Task_00070308_step_table[step] ~= nil then
		return Task_00070308_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070308_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070308() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70308) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070308_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70308) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70308);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070308_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70308);
end

--����Ľ�������
function Task_Accept_00070309()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70309) or task:HasCompletedTask(70309) or task:HasSubmitedTask(70309) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070309()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(70309) or task:HasCompletedTask(70309) or task:HasSubmitedTask(70309) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70309 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070309()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70309) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070309(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70309) == npcId and Task_Accept_00070309 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70309
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "只要锄头舞的好";
	elseif task:GetTaskSubmitNpc(70309) == npcId then
		if Task_Submit_00070309() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70309
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "只要锄头舞的好";
		elseif task:HasAcceptedTask(70309) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70309
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "只要锄头舞的好";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070309_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "摩加湖边湖窟真是一个恐怖的地方，有些啥不好，偏偏出现了一些满身是血的骷髅，要是让我看到了，我肯定晕过去了，但是我相信你勇士，只要锄头舞的好，没有骷髅杀不掉，勇敢的去吧。\n";
	action.m_ActionMsg = "你胆子也太小了吧，区区几个小骷髅就把你吓成这样。";
	return action;
end

function Task_00070309_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "最好是别再让我看到那些满身是血的骷髅了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070309_step_table = {
		[1] = Task_00070309_step_01,
		[10] = Task_00070309_step_10,
		};

function Task_00070309_step(step)
	if Task_00070309_step_table[step] ~= nil then
		return Task_00070309_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070309_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070309() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70309) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070309_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70309) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70309);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070309_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70309);
end

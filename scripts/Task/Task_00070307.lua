--����Ľ�������
function Task_Accept_00070307()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70307) or task:HasCompletedTask(70307) or task:HasSubmitedTask(70307) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070307()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(70307) or task:HasCompletedTask(70307) or task:HasSubmitedTask(70307) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70307 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070307()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70307) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070307(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70307) == npcId and Task_Accept_00070307 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70307
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "不蒸馒头争口气";
	elseif task:GetTaskSubmitNpc(70307) == npcId then
		if Task_Submit_00070307() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70307
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "不蒸馒头争口气";
		elseif task:HasAcceptedTask(70307) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70307
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "不蒸馒头争口气";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070307_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "天池码头废墟的渔人头目，仗着自己身强力壮，且耳目众多，几乎在天池称霸一方了，我们怎么可以容忍一个渔人这样做，去把他杀掉吧，就算不蒸馒头也要争口气。";
	action.m_ActionMsg = "又是那些半人半鱼的家伙，还真是不死心，我这就去。";
	return action;
end

function Task_00070307_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "人争一口气，佛受一柱香。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070307_step_table = {
		[1] = Task_00070307_step_01,
		[10] = Task_00070307_step_10,
		};

function Task_00070307_step(step)
	if Task_00070307_step_table[step] ~= nil then
		return Task_00070307_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070307_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070307() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70307) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070307_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70307) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70307);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070307_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70307);
end

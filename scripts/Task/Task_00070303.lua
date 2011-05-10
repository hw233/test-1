--����Ľ�������
function Task_Accept_00070303()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70303) or task:HasCompletedTask(70303) or task:HasSubmitedTask(70303) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70303) or task:HasCompletedTask(70303) or task:HasSubmitedTask(70303) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70303 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70303) == npcId and Task_Accept_00070303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "再冒险一次也不会死";
	elseif task:GetTaskSubmitNpc(70303) == npcId then
		if Task_Submit_00070303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "再冒险一次也不会死";
		elseif task:HasAcceptedTask(70303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "再冒险一次也不会死";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "威海岛浮石的那些浸毒猎手到底有完没完啊，杀完了又出现了，出现了杀掉，然后又出现了，勇士只能靠你了，要知道，就凭你的实力，再去那里冒一次险也不会死的。";
	action.m_ActionMsg = "你得弄明白他们为什么总能卷土重来,这是关键.";
	return action;
end

function Task_00070303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么样，我就知道你再冒一次险也不会死的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070303_step_table = {
		[1] = Task_00070303_step_01,
		[10] = Task_00070303_step_10,
		};

function Task_00070303_step(step)
	if Task_00070303_step_table[step] ~= nil then
		return Task_00070303_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070303() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70303) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070303_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70303) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70303);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070303_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70303);
end

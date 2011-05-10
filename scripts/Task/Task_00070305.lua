--����Ľ�������
function Task_Accept_00070305()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70305) or task:HasCompletedTask(70305) or task:HasSubmitedTask(70305) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070305()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70305) or task:HasCompletedTask(70305) or task:HasSubmitedTask(70305) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70305 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070305()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70305) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070305(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70305) == npcId and Task_Accept_00070305 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70305
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "打的不是怪兽是寂寞";
	elseif task:GetTaskSubmitNpc(70305) == npcId then
		if Task_Submit_00070305() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70305
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "打的不是怪兽是寂寞";
		elseif task:HasAcceptedTask(70305) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70305
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "打的不是怪兽是寂寞";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070305_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "前两天出去散步，路过王舍城废墟的黑风岗，差点被那里的一些怪兽吓死，幸好我跑的快，后来知道那些可怕的怪兽叫黑风兽，如果你现在正寂寞着，就去帮我杀掉他们吧。";
	action.m_ActionMsg = "你胆子也太小了吧，区区几个小怪兽就把你吓成这样。";
	return action;
end

function Task_00070305_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你打的不是怪兽是寂寞，我散的不是步，也是寂寞啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070305_step_table = {
		[1] = Task_00070305_step_01,
		[10] = Task_00070305_step_10,
		};

function Task_00070305_step(step)
	if Task_00070305_step_table[step] ~= nil then
		return Task_00070305_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070305_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070305() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70305) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070305_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70305) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70305);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070305_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70305);
end

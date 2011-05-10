--����Ľ�������
function Task_Accept_00070315()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70315) or task:HasCompletedTask(70315) or task:HasSubmitedTask(70315) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070315()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70315) or task:HasCompletedTask(70315) or task:HasSubmitedTask(70315) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070315()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70315) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070315(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70315) == npcId and Task_Accept_00070315 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70315
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "烽烟再起";
	elseif task:GetTaskSubmitNpc(70315) == npcId then
		if Task_Submit_00070315() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70315
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "烽烟再起";
		elseif task:HasAcceptedTask(70315) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70315
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "烽烟再起";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070315_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "前两天出去散步，路过王舍城废墟的黑风岗，差点被那里的一些怪兽吓死，幸好我跑的快，后来知道那些可怕的怪兽叫黑风兽，如果你现在有时间，就去帮我杀掉他们吧，貌似最近战火不断，烽烟再起，我们不能再闲着了。";
	action.m_ActionMsg = "我一直就没闲着，我胆子可比你大。";
	return action;
end

function Task_00070315_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "战火不断，烽烟再起……";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070315_step_table = {
		[1] = Task_00070315_step_01,
		[10] = Task_00070315_step_10,
		};

function Task_00070315_step(step)
	if Task_00070315_step_table[step] ~= nil then
		return Task_00070315_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070315_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070315() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70315) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070315_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(70315) then
		return false;
	end


	player:getCoin(200000);
	return true;
end

--��������
function Task_00070315_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70315);
end

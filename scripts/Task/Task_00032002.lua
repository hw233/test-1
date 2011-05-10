--����Ľ�������
function Task_Accept_00032002()
	local player = GetPlayer();
	if player:GetLev() < 74 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(32002) or task:HasCompletedTask(32002) or task:HasSubmitedTask(32002) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32001) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32001) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00032002()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 74 then
		return false;
	end
	if task:HasAcceptedTask(32002) or task:HasCompletedTask(32002) or task:HasSubmitedTask(32002) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32001) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32001) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00032002()
	if GetPlayer():GetTaskMgr():HasCompletedTask(32002) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00032002(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(32002) == npcId and Task_Accept_00032002 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 32002
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "联合同盟";
	elseif task:GetTaskSubmitNpc(32002) == npcId then
		if Task_Submit_00032002() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32002
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "联合同盟";
		elseif task:HasAcceptedTask(32002) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32002
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "联合同盟";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00032002_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "通过最近对夜摩盟的形势和动向的观察，你发现的跟我猜测的几乎一样，看来夜摩盟就快要做好准备，并要开始全面占据各种势力的行动了。";
	action.m_ActionMsg = "是的，他们已经在蠢蠢欲动了。";
	return action;
end

function Task_00032002_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想我们也应该做好正面出击的准备了，绝不能让夜摩盟在山海大陆肆虐，你作为<天/魔>族的勇士，肯定也不希望他们侵犯到你们种族的利益。你马上去木渎镇通知镇邪派的程风，我们需要联合所有的力量共同抵抗夜摩盟了。";
	action.m_ActionMsg = "我也会坚决的阻止夜摩盟的。";
	return action;
end

function Task_00032002_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "寒尧让你送来的消息正是我们等候多时了的";
	action.m_ActionMsg = "";
	return action;
end

local Task_00032002_step_table = {
		[1] = Task_00032002_step_01,
		[2] = Task_00032002_step_02,
		[10] = Task_00032002_step_10,
		};

function Task_00032002_step(step)
	if Task_00032002_step_table[step] ~= nil then
		return Task_00032002_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00032002_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00032002() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(32002) then
		return false;
	end
	task:AddTaskStep(32002);
	return true;
end



--�ύ����
function Task_00032002_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(32002) then
		return false;
	end


	player:AddExp(290000);
	player:getCoin(275000);
	player:getTael(40);
	return true;
end

--��������
function Task_00032002_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(32002);
end

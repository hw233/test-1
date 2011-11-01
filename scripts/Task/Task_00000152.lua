--����Ľ�������
function Task_Accept_00000152()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(152) or task:HasCompletedTask(152) or task:HasSubmitedTask(152) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(151) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(151) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(151) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000152()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(152) or task:HasCompletedTask(152) or task:HasSubmitedTask(152) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(151) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(151) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(151) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000152()
	if GetPlayer():GetTaskMgr():HasCompletedTask(152) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000152(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(152) == npcId and Task_Accept_00000152 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 152
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "雁荡水贼";
	elseif task:GetTaskSubmitNpc(152) == npcId then
		if Task_Submit_00000152() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 152
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "雁荡水贼";
		elseif task:HasAcceptedTask(152) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 152
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "雁荡水贼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000152_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠想必与之前在此除妖的仙子是同门吧，看来都是侠义之辈啊。老朽在雁荡湖畔生活了七十多年，这个村子也一直平安，可是前段时间不知哪里来了一伙水贼，打家劫舍，无恶不作，还希望少侠可以将他们驱逐出雁荡湖。";
	action.m_ActionMsg = "那么麻烦干什么，都送他们去投胎好了，嘿嘿。";
	return action;
end

function Task_00000152_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是谢天谢地，有少侠你来帮助我们。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000152_step_table = {
		[1] = Task_00000152_step_01,
		[10] = Task_00000152_step_10,
		};

function Task_00000152_step(step)
	if Task_00000152_step_table[step] ~= nil then
		return Task_00000152_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000152_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000152() then
		return false;
	end
	if not task:AcceptTask(152) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000152_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(152) then
		return false;
	end


	player:AddExp(90000);
	return true;
end

--��������
function Task_00000152_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(152);
end

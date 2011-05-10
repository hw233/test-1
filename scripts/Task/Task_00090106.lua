--����Ľ�������
function Task_Accept_00090106()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90106) or task:HasCompletedTask(90106) or task:HasSubmitedTask(90106) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90106) or task:HasCompletedTask(90106) or task:HasSubmitedTask(90106) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90106) == npcId and Task_Accept_00090106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "可怜的无暇";
	elseif task:GetTaskSubmitNpc(90106) == npcId then
		if Task_Submit_00090106() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90106
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "可怜的无暇";
		elseif task:HasAcceptedTask(90106) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90106
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "可怜的无暇";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "无暇是个可怜的孩子，一直到现在都不知道自己的生父母是谁。我现在在这儿助战，抽不出空去看她，你替我去吧。";
	action.m_ActionMsg = "据说她是个可人儿，这么乖巧的女儿，父母怎舍得丢弃呀。";
	return action;
end

function Task_00090106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你是谁？我认识你吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090106_step_table = {
		[1] = Task_00090106_step_01,
		[10] = Task_00090106_step_10,
		};

function Task_00090106_step(step)
	if Task_00090106_step_table[step] ~= nil then
		return Task_00090106_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090106() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90106) then
		return false;
	end
	task:AddTaskStep(90106);
	return true;
end



--�ύ����
function Task_00090106_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90106) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--��������
function Task_00090106_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90106);
end

--����Ľ�������
function Task_Accept_00090110()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90110) or task:HasCompletedTask(90110) or task:HasSubmitedTask(90110) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090110()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90110) or task:HasCompletedTask(90110) or task:HasSubmitedTask(90110) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090110()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90110) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090110(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90110) == npcId and Task_Accept_00090110 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90110
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "美好的祝福";
	elseif task:GetTaskSubmitNpc(90110) == npcId then
		if Task_Submit_00090110() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90110
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "美好的祝福";
		elseif task:HasAcceptedTask(90110) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90110
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "美好的祝福";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090110_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "貂蝉是四大美女之一，吕布非常喜欢她。你去帮我探望这位大美人，替我送上最美好的祝福。";
	action.m_ActionMsg = "平白无故干嘛去探望她，将军你不会也仰慕她吧？";
	return action;
end

function Task_00090110_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "呵呵，我自有道理，你放心，本将不是那贪色之人。";
	action.m_ActionMsg = "那就好，多少男人诚服于她，我相信将军不是那样的人。";
	return action;
end

function Task_00090110_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是勒勤将军让你来探望妾身的？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090110_step_table = {
		[1] = Task_00090110_step_01,
		[2] = Task_00090110_step_02,
		[10] = Task_00090110_step_10,
		};

function Task_00090110_step(step)
	if Task_00090110_step_table[step] ~= nil then
		return Task_00090110_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090110_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090110() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90110) then
		return false;
	end
	task:AddTaskStep(90110);
	return true;
end



--�ύ����
function Task_00090110_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90110) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--��������
function Task_00090110_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90110);
end

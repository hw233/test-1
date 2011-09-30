--����Ľ�������
function Task_Accept_00000583()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(583) or task:HasCompletedTask(583) or task:HasSubmitedTask(583) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000583()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(583) or task:HasCompletedTask(583) or task:HasSubmitedTask(583) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000583()
	if GetPlayer():GetTaskMgr():HasCompletedTask(583) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000583(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(583) == npcId and Task_Accept_00000583 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 583
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "白云大师";
	elseif task:GetTaskSubmitNpc(583) == npcId then
		if Task_Submit_00000583() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 583
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "白云大师";
		elseif task:HasAcceptedTask(583) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 583
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "白云大师";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000583_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "白云大师元敬乃是峨眉长眉真人的亲传弟子，她一般隐居在巫山峡白竹涧正修庵，偶尔会来成都云灵山一游，学道很早，同辈中年岁与玄真子、嵩山二老不相上下，剑术高强，你可以多多向她请教剑术上的疑问。";
	action.m_ActionMsg = "弟子这就去。";
	return action;
end

function Task_00000583_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢小友的好意，也多谢大师的挂念，希望小友多行侠仗义，扬我正道之气。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000583_step_table = {
		[1] = Task_00000583_step_01,
		[10] = Task_00000583_step_10,
		};

function Task_00000583_step(step)
	if Task_00000583_step_table[step] ~= nil then
		return Task_00000583_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000583_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000583() then
		return false;
	end
	if not task:AcceptTask(583) then
		return false;
	end
	task:AddTaskStep(583);
	return true;
end



--�ύ����
function Task_00000583_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(583) then
		return false;
	end


	return true;
end

--��������
function Task_00000583_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(583);
end

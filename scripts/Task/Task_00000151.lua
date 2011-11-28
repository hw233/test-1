--����Ľ�������
function Task_Accept_00000151()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(151) or task:HasCompletedTask(151) or task:HasSubmitedTask(151) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000151()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(151) or task:HasCompletedTask(151) or task:HasSubmitedTask(151) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(150) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000151()
	if GetPlayer():GetTaskMgr():HasCompletedTask(151) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000151(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(151) == npcId and Task_Accept_00000151 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 151
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "落湖镇";
	elseif task:GetTaskSubmitNpc(151) == npcId then
		if Task_Submit_00000151() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 151
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "落湖镇";
		elseif task:HasAcceptedTask(151) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 151
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "落湖镇";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000151_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在我除妖鲧的这段时间，雁荡山脚落湖镇的村民对我礼遇有加，只是恩师有命，要我除妖后即刻回百花山。这雁荡靠近东海，也滋生了不少妖孽，我暂时分身乏术，还希望你可以去帮助万村长解决周遭的祸害。";
	action.m_ActionMsg = "没问题……";
	return action;
end

function Task_00000151_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是谢天谢地，有少侠你来帮助我们。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000151_step_table = {
		[1] = Task_00000151_step_01,
		[10] = Task_00000151_step_10,
		};

function Task_00000151_step(step)
	if Task_00000151_step_table[step] ~= nil then
		return Task_00000151_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000151_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000151() then
		return false;
	end
	if not task:AcceptTask(151) then
		return false;
	end
	task:AddTaskStep(151);
	return true;
end



--�ύ����
function Task_00000151_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(151) then
		return false;
	end


	player:AddExp(60000);
	return true;
end

--��������
function Task_00000151_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(151);
end

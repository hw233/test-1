--����Ľ�������
function Task_Accept_00000110()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(110) or task:HasCompletedTask(110) or task:HasSubmitedTask(110) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(109) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(109) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(109) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000110()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(110) or task:HasCompletedTask(110) or task:HasSubmitedTask(110) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(109) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(109) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(109) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000110()
	if GetPlayer():GetTaskMgr():HasCompletedTask(110) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000110(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(110) == npcId and Task_Accept_00000110 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 110
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "祭风台";
	elseif task:GetTaskSubmitNpc(110) == npcId then
		if Task_Submit_00000110() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 110
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "祭风台";
		elseif task:HasAcceptedTask(110) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 110
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "祭风台";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000110_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "为师和你的师叔师伯不方便出面，唯恐打草惊蛇惊动了绿袍妖孽。如今绿袍手下基本已经清剿干净，只剩下祭风台一带妖孽的亲传弟子了。他们能被绿袍收为亲传弟子，基本都是十恶不赦之辈，将他们一并消灭吧"..GetPlayerName(GetPlayer()).."。";
	action.m_ActionMsg = "除魔卫道正是我辈本色。";
	return action;
end

function Task_00000110_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如今绿袍老祖的爪牙尽去，我们布置两仪微尘阵的进度可以加快不少。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000110_step_table = {
		[1] = Task_00000110_step_01,
		[10] = Task_00000110_step_10,
		};

function Task_00000110_step(step)
	if Task_00000110_step_table[step] ~= nil then
		return Task_00000110_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000110_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000110() then
		return false;
	end
	if not task:AcceptTask(110) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000110_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(110) then
		return false;
	end


	player:AddExp(38000);
	return true;
end

--��������
function Task_00000110_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(110);
end

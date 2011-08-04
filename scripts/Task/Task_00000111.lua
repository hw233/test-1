--����Ľ�������
function Task_Accept_00000111()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(111) or task:HasCompletedTask(111) or task:HasSubmitedTask(111) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(110) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(110) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(110) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000111()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(111) or task:HasCompletedTask(111) or task:HasSubmitedTask(111) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(110) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(110) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(110) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000111()
	if GetPlayer():GetTaskMgr():HasCompletedTask(111) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000111(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(111) == npcId and Task_Accept_00000111 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 111
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "金蚕蛊母";
	elseif task:GetTaskSubmitNpc(111) == npcId then
		if Task_Submit_00000111() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 111
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "金蚕蛊母";
		elseif task:HasAcceptedTask(111) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 111
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "金蚕蛊母";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000111_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."有你帮忙，清剿了绿袍的手下，我们布置两仪微尘阵至今未被绿袍发现，看来除魔大计成功有望。是时候引绿袍出洞了！在那阴风后洞有一只金蚕蛊母，是绿袍炼制百毒金蚕蛊的根本，"..GetPlayerName(GetPlayer()).."你去将蛊母消灭，一定会引的它大怒，乱了方寸，到时候我们就可以发动大阵将此獠消灭。";
	action.m_ActionMsg = "好的，弟子这就去。";
	return action;
end

function Task_00000111_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "呵呵，两仪微尘阵即日就可以布置成功了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000111_step_table = {
		[1] = Task_00000111_step_01,
		[10] = Task_00000111_step_10,
		};

function Task_00000111_step(step)
	if Task_00000111_step_table[step] ~= nil then
		return Task_00000111_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000111_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000111() then
		return false;
	end
	if not task:AcceptTask(111) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000111_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(111) then
		return false;
	end


	player:AddExp(11111);
	return true;
end

--��������
function Task_00000111_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(111);
end

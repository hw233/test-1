--����Ľ�������
function Task_Accept_00000102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(102) or task:HasCompletedTask(102) or task:HasSubmitedTask(102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(101) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(101) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(102) or task:HasCompletedTask(102) or task:HasSubmitedTask(102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(101) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(101) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(102) == npcId and Task_Accept_00000102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "斩蛇";
	elseif task:GetTaskSubmitNpc(102) == npcId then
		if Task_Submit_00000102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "斩蛇";
		elseif task:HasAcceptedTask(102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "斩蛇";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这金针圣母昔年作孽也颇多，虽然我和她有斗剑之约，但我却不会用青牛剑助她兵解。不过"..GetPlayerName(GetPlayer()).."你颇有侠名，跟我借剑却也不是不可。武当山的斩龙崖是昔年张三丰真人斩孽龙之所，只是孽龙虽除，仍有些蛇妖尚存。少侠你若去帮我除掉这些孽蛇妖，我就赠你青牛剑。";
	action.m_ActionMsg = "半边大师真是多谢你了。";
	return action;
end

function Task_00000102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠剑术高强，除掉这些蛇妖果然是易如反掌。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000102_step_table = {
		[1] = Task_00000102_step_01,
		[10] = Task_00000102_step_10,
		};

function Task_00000102_step(step)
	if Task_00000102_step_table[step] ~= nil then
		return Task_00000102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000102() then
		return false;
	end
	if not task:AcceptTask(102) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(102) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000102_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(102);
end

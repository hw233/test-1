--����Ľ�������
function Task_Accept_00000035()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(35) or task:HasCompletedTask(35) or task:HasSubmitedTask(35) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000035()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(35) or task:HasCompletedTask(35) or task:HasSubmitedTask(35) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000035()
	if GetPlayer():GetTaskMgr():HasCompletedTask(35) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000035(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(35) == npcId and Task_Accept_00000035 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 35
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "惩治恶徒";
	elseif task:GetTaskSubmitNpc(35) == npcId then
		if Task_Submit_00000035() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 35
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "惩治恶徒";
		elseif task:HasAcceptedTask(35) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 35
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "惩治恶徒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000035_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>你来得正好，小女正被那妖人侮辱，眼看名节不保，还请少侠速速施以援手。";
	action.m_ActionMsg = "老丈放心，我这就去除掉这个恶贼。";
	return action;
end

function Task_00000035_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>，大恩不言谢，老朽也没什么拿得出手的东西，而且少侠是世外高人，一般黄白之物恐怕也入不了少侠的法眼。这里有一个我年青时在峨眉山找到的奇异之物，送给少侠，希望可以对你有点帮助。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000035_step_table = {
		[1] = Task_00000035_step_01,
		[10] = Task_00000035_step_10,
		};

function Task_00000035_step(step)
	if Task_00000035_step_table[step] ~= nil then
		return Task_00000035_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000035_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000035() then
		return false;
	end
	if not task:AcceptTask(35) then
		return false;
	end
	task:AddTaskStep(35);
	return true;
end



--�ύ����
function Task_00000035_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(35) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--��������
function Task_00000035_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(35);
end

--����Ľ�������
function Task_Accept_00000105()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(105) or task:HasCompletedTask(105) or task:HasSubmitedTask(105) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(105) or task:HasCompletedTask(105) or task:HasSubmitedTask(105) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(105) == npcId and Task_Accept_00000105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "魔教始祖";
	elseif task:GetTaskSubmitNpc(105) == npcId then
		if Task_Submit_00000105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "魔教始祖";
		elseif task:HasAcceptedTask(105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "魔教始祖";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "峨眉派掌教妙一真人齐漱溟和老友东海三仙准备除掉南方魔教的始祖绿袍老祖，已经在百蛮山着手布置长眉真人遗留的两仪微尘大阵，"..GetPlayerName(GetPlayer()).."我知道你和绿袍也有些小小的过节，不如趁此机会前去做一个了断如何？";
	action.m_ActionMsg = "多谢大师指点，晚辈这就赶往百蛮山。";
	return action;
end

function Task_00000105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."今日我们东海三仙齐聚百蛮山布两仪微尘阵也将是一段佳话啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000105_step_table = {
		[1] = Task_00000105_step_01,
		[10] = Task_00000105_step_10,
		};

function Task_00000105_step(step)
	if Task_00000105_step_table[step] ~= nil then
		return Task_00000105_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000105() then
		return false;
	end
	if not task:AcceptTask(105) then
		return false;
	end
	task:AddTaskStep(105);
	return true;
end



--�ύ����
function Task_00000105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(105) then
		return false;
	end


	player:AddExp(35000);
	return true;
end

--��������
function Task_00000105_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(105);
end

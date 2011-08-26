--����Ľ�������
function Task_Accept_00000032()
	local player = GetPlayer();
	if player:GetLev() < 20 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(32) or task:HasCompletedTask(32) or task:HasSubmitedTask(32) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(31) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000032()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 20 then
		return false;
	end
	if task:HasAcceptedTask(32) or task:HasCompletedTask(32) or task:HasSubmitedTask(32) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(31) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000032()
	if GetPlayer():GetTaskMgr():HasCompletedTask(32) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000032(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(32) == npcId and Task_Accept_00000032 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 32
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "强化装备";
	elseif task:GetTaskSubmitNpc(32) == npcId then
		if Task_Submit_00000032() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "强化装备";
		elseif task:HasAcceptedTask(32) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "强化装备";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000032_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这个穷酒鬼竟然还藏着这么个好玩意，来来来，让我教教你怎么把这块太乙真金熔炼到你的飞剑中去，这次就不收你钱了，全记在那个酒鬼头上。";
	action.m_ActionMsg = "这怎么好意思呢？";
	return action;
end

function Task_00000032_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "没事，没事……你看这么大块的太乙真金一次强化也用不了这许多，剩下的就权且当做手工费了，嘿嘿。";
	action.m_ActionMsg = "前辈真是打的好算盘……";
	return action;
end

function Task_00000032_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嘿嘿，小子你以后找我的机会多着呢，这点小钱小意思了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000032_step_table = {
		[1] = Task_00000032_step_01,
		[2] = Task_00000032_step_02,
		[10] = Task_00000032_step_10,
		};

function Task_00000032_step(step)
	if Task_00000032_step_table[step] ~= nil then
		return Task_00000032_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000032_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000032() then
		return false;
	end
	if not task:AcceptTask(32) then
		return false;
	end
	task:AddTaskStep(32);
	return true;
end



--�ύ����
function Task_00000032_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(32) then
		return false;
	end


	player:AddExp(3700);
	return true;
end

--��������
function Task_00000032_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(32);
end

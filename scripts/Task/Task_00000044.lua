--����Ľ�������
function Task_Accept_00000044()
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(44) or task:HasCompletedTask(44) or task:HasSubmitedTask(44) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(43) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(43) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(43) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000044()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(44) or task:HasCompletedTask(44) or task:HasSubmitedTask(44) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(43) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(43) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(43) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000044()
	if GetPlayer():GetTaskMgr():HasCompletedTask(44) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000044(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(44) == npcId and Task_Accept_00000044 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 44
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "厢房密室";
	elseif task:GetTaskSubmitNpc(44) == npcId then
		if Task_Submit_00000044() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 44
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "厢房密室";
		elseif task:HasAcceptedTask(44) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 44
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "厢房密室";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000044_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "想不到少侠武艺如此高强，这样我就放心很多了，那周云从被关押的地牢在密室内，你要先去厢房开启密室的机关，才能进入地牢救人。";
	action.m_ActionMsg = "是什么样的机关？";
	return action;
end

function Task_00000044_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠进入厢房可以看到中堂挂着一副八仙过海图，在画的下面有一个磬锤，只要一敲，密室暗门就打开了。少侠千万注意不要惊动密室里看守的妖人。";
	action.m_ActionMsg = "好的，我这就去打开机关。";
	return action;
end

function Task_00000044_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是惊险，还好少侠你快刀斩乱麻，要是惊动了其他妖人那就糟糕了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000044_step_table = {
		[1] = Task_00000044_step_01,
		[2] = Task_00000044_step_02,
		[10] = Task_00000044_step_10,
		};

function Task_00000044_step(step)
	if Task_00000044_step_table[step] ~= nil then
		return Task_00000044_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000044_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000044() then
		return false;
	end
	if not task:AcceptTask(44) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000044_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(44) then
		return false;
	end


	player:AddExp(7000);
	return true;
end

--��������
function Task_00000044_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(44);
end

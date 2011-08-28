--����Ľ�������
function Task_Accept_00000049()
	local player = GetPlayer();
	if player:GetLev() < 32 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(49) or task:HasCompletedTask(49) or task:HasSubmitedTask(49) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000049()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 32 then
		return false;
	end
	if task:HasAcceptedTask(49) or task:HasCompletedTask(49) or task:HasSubmitedTask(49) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000049()
	if GetPlayer():GetTaskMgr():HasCompletedTask(49) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000049(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(49) == npcId and Task_Accept_00000049 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 49
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "铁掌僧";
	elseif task:GetTaskSubmitNpc(49) == npcId then
		if Task_Submit_00000049() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 49
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "铁掌僧";
		elseif task:HasAcceptedTask(49) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 49
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "铁掌僧";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000049_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "慈云寺里众妖人一直孽迹不显，不过经过这段时间的明擦暗访，我已经清楚他们一般不在周边地区作案，所以周围百姓一直被他们蒙蔽，慈云寺方丈智通手下有四大弟子号称金刚，领头的叫铁掌僧慧明，此人对智通忠心耿耿，又诡计多端，若要铲除慈云寺魔窟，智通的爪牙一定要尽早除掉。";
	action.m_ActionMsg = "邱大哥放心，管他铁掌僧还是铜掌僧，见了我都要变成软脚虾。";
	return action;
end

function Task_00000049_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "铁掌僧见到你，变成软脚虾了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000049_step_table = {
		[1] = Task_00000049_step_01,
		[10] = Task_00000049_step_10,
		};

function Task_00000049_step(step)
	if Task_00000049_step_table[step] ~= nil then
		return Task_00000049_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000049_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000049() then
		return false;
	end
	if not task:AcceptTask(49) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000049_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(49) then
		return false;
	end


	player:AddExp(15000);
	return true;
end

--��������
function Task_00000049_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(49);
end

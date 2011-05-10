--����Ľ�������
function Task_Accept_00062206()
	local player = GetPlayer();
	if player:GetLev() < 38 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62206) or task:HasCompletedTask(62206) or task:HasSubmitedTask(62206) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062206()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 38 then
		return false;
	end
	if task:HasAcceptedTask(62206) or task:HasCompletedTask(62206) or task:HasSubmitedTask(62206) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00062206()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62206) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062206(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62206) == npcId and Task_Accept_00062206 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62206
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "加入宗族";
	elseif task:GetTaskSubmitNpc(62206) == npcId then
		if Task_Submit_00062206() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62206
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "加入宗族";
		elseif task:HasAcceptedTask(62206) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62206
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "加入宗族";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062206_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "孤身一人，练至此境界是否倍感寂寞？";
	action.m_ActionMsg = "你想干嘛？（背后一丝凉风吹过）";
	return action;
end

function Task_00062206_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哈哈~~勇士别误会，我只是想说山海大陆上的宗族期待你的加入，请速去选择一个喜欢的宗族加入（点击宗族按钮，选择宗族，点击加人）";
	action.m_ActionMsg = "原来是这样，那我这就去。";
	return action;
end

function Task_00062206_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不再寂寞了吧？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062206_step_table = {
		[1] = Task_00062206_step_01,
		[2] = Task_00062206_step_02,
		[10] = Task_00062206_step_10,
		};

function Task_00062206_step(step)
	if Task_00062206_step_table[step] ~= nil then
		return Task_00062206_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062206_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062206() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62206) then
		return false;
	end
	if player:getClan() ~= nil then
		task:AddTaskStep(62206);
	end
	return true;
end



--�ύ����
function Task_00062206_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62206) then
		return false;
	end


	player:AddExp(10000);
	player:getCoin(20000);
	return true;
end

--��������
function Task_00062206_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62206);
end

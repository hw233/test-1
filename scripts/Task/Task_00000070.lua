--����Ľ�������
function Task_Accept_00000070()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70) or task:HasCompletedTask(70) or task:HasSubmitedTask(70) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000070()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(70) or task:HasCompletedTask(70) or task:HasSubmitedTask(70) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(48) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000070()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000070(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70) == npcId and Task_Accept_00000070 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "阵营选择";
	elseif task:GetTaskSubmitNpc(70) == npcId then
		if Task_Submit_00000070() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "阵营选择";
		elseif task:HasAcceptedTask(70) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "阵营选择";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000070_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "醉道人师叔已经观察你许久，觉得少侠你侠肝义胆，也是我们正道之人，所以已经象昆仑和峨眉的掌教真人推荐了少侠， 不知道少侠你愿意加入昆仑派还是峨眉派呢？";
	action.m_ActionMsg = "嗯，让我思考一下。";
	return action;
end

function Task_00000070_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我这就期待着少侠可以在将来的正邪之战中大方异彩了。";
	action.m_ActionMsg = "邱大哥，你过奖了。";
	return action;
end

function Task_00000070_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>正道中兴的重担就在你们的肩上了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000070_step_table = {
		[1] = Task_00000070_step_01,
		[2] = Task_00000070_step_02,
		[10] = Task_00000070_step_10,
		};

function Task_00000070_step(step)
	if Task_00000070_step_table[step] ~= nil then
		return Task_00000070_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000070_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000070() then
		return false;
	end
	if not task:AcceptTask(70) then
		return false;
	end
	task:AddTaskStep(70);
	return true;
end



--�ύ����
function Task_00000070_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(70) then
		return false;
	end


	player:AddExp(2000);
	return true;
end

--��������
function Task_00000070_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(70);
end

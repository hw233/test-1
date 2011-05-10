--����Ľ�������
function Task_Accept_00030606()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30606) or task:HasCompletedTask(30606) or task:HasSubmitedTask(30606) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30605) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30605) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030606()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(30606) or task:HasCompletedTask(30606) or task:HasSubmitedTask(30606) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30605) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30605) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030606()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30606) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030606(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30606) == npcId and Task_Accept_00030606 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30606
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "忽然清醒";
	elseif task:GetTaskSubmitNpc(30606) == npcId then
		if Task_Submit_00030606() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30606
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "忽然清醒";
		elseif task:HasAcceptedTask(30606) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30606
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "忽然清醒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030606_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位勇士你好，为什么这么脸色阴沉的到这里来？我看你装备上的印记，你应该是与我们东山派为伍的啊？\n";
	action.m_ActionMsg = "废话少说，赶紧离开这里。不然我只能动用武力了！";
	return action;
end

function Task_00030606_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这是怎么回事勇士，我们并不想和你打起来的。";
	action.m_ActionMsg = "对不起啊，我这是怎么回事，简直见鬼了，我怎么会这么做！";
	return action;
end

function Task_00030606_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你果然又是一个中了夜摩的阴谋的勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030606_step_table = {
		[1] = Task_00030606_step_01,
		[2] = Task_00030606_step_02,
		[10] = Task_00030606_step_10,
		};

function Task_00030606_step(step)
	if Task_00030606_step_table[step] ~= nil then
		return Task_00030606_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030606_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030606() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30606) then
		return false;
	end
	task:AddTaskStep(30606);
	return true;
end



--�ύ����
function Task_00030606_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30606) then
		return false;
	end


	player:AddExp(36000);
	player:getCoin(25000);
	player:getTael(30);
	return true;
end

--��������
function Task_00030606_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30606);
end

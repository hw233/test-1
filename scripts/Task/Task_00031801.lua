--����Ľ�������
function Task_Accept_00031801()
	local player = GetPlayer();
	if player:GetLev() < 69 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31801) or task:HasCompletedTask(31801) or task:HasSubmitedTask(31801) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31604) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31704) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 69 then
		return false;
	end
	if task:HasAcceptedTask(31801) or task:HasCompletedTask(31801) or task:HasSubmitedTask(31801) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31604) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31704) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31801) == npcId and Task_Accept_00031801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "镇邪派的叛徒";
	elseif task:GetTaskSubmitNpc(31801) == npcId then
		if Task_Submit_00031801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "镇邪派的叛徒";
		elseif task:HasAcceptedTask(31801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "镇邪派的叛徒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士很高兴再次见到你，经历了打败夜摩盟的罗阅王之后，我想你已经成为山海大陆名震一方的勇士了，既有震慑夜摩的实力，又有维护正义的侠义心肠。";
	action.m_ActionMsg = "不必这么客气，有什么事情要帮你的吗？";
	return action;
end

function Task_00031801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然勇士你已经这么说了，我也就不绕圈子了，我现在很需要你帮我去一趟无忧谷的慕士城，找到那里的额提。这个叫额提的人，原来是我们镇邪派的人，可是因为贪慕虚荣，被夜摩盟诱惑而被判了镇邪派，并且挟持了我的女儿蓦然，我想现在也只有你能够帮到我了。";
	action.m_ActionMsg = "原来是这样，这个忙我愿意帮你。";
	return action;
end

function Task_00031801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，是严中叫你来的吧?";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031801_step_table = {
		[1] = Task_00031801_step_01,
		[2] = Task_00031801_step_02,
		[10] = Task_00031801_step_10,
		};

function Task_00031801_step(step)
	if Task_00031801_step_table[step] ~= nil then
		return Task_00031801_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31801) then
		return false;
	end
	task:AddTaskStep(31801);
	return true;
end



--�ύ����
function Task_00031801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31801) then
		return false;
	end


	player:AddExp(220000);
	player:getCoin(204000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031801_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31801);
end

--����Ľ�������
function Task_Accept_00000189()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(189) or task:HasCompletedTask(189) or task:HasSubmitedTask(189) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000189()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(189) or task:HasCompletedTask(189) or task:HasSubmitedTask(189) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(188) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000189()
	if GetPlayer():GetTaskMgr():HasCompletedTask(189) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000189(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(189) == npcId and Task_Accept_00000189 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 189
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "白泽";
	elseif task:GetTaskSubmitNpc(189) == npcId then
		if Task_Submit_00000189() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 189
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "白泽";
		elseif task:HasAcceptedTask(189) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 189
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "白泽";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000189_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老夫久居摩云岭，附近有一处美景叫星宿海，可是不日前来了一只异兽白泽，破坏植被，杀死其他生灵无数。远远见到老夫的气息就逃遁的无影无踪，所以特地麻烦少侠来帮我解决这个异兽了。";
	action.m_ActionMsg = "我这就去将这只白泽抓来。";
	return action;
end

function Task_00000189_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠真是修为深厚，天赋异禀啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000189_step_table = {
		[1] = Task_00000189_step_01,
		[10] = Task_00000189_step_10,
		};

function Task_00000189_step(step)
	if Task_00000189_step_table[step] ~= nil then
		return Task_00000189_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000189_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000189() then
		return false;
	end
	if not task:AcceptTask(189) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000189_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(189) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000189_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(189);
end

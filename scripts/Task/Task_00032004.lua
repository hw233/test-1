--����Ľ�������
function Task_Accept_00032004()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(32004) or task:HasCompletedTask(32004) or task:HasSubmitedTask(32004) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00032004()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(32004) or task:HasCompletedTask(32004) or task:HasSubmitedTask(32004) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32003) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00032004()
	if GetPlayer():GetTaskMgr():HasCompletedTask(32004) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00032004(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(32004) == npcId and Task_Accept_00032004 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 32004
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "发起挑战";
	elseif task:GetTaskSubmitNpc(32004) == npcId then
		if Task_Submit_00032004() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32004
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "发起挑战";
		elseif task:HasAcceptedTask(32004) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32004
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "发起挑战";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00032004_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你做的很好，现在是要让夜摩盟知道我们的厉害的时候了。把灰岩守卫的头颅挂到幻海城的晨钟镇上去，就会让从那里经过的夜摩盟全都看到，我们已经向他们发起挑战了。";
	action.m_ActionMsg = "我很乐意效劳，也让那帮家伙尝尝痛苦的滋味。";
	return action;
end

function Task_00032004_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在就是我们进入全面备战状态的时刻了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00032004_step_table = {
		[1] = Task_00032004_step_01,
		[10] = Task_00032004_step_10,
		};

function Task_00032004_step(step)
	if Task_00032004_step_table[step] ~= nil then
		return Task_00032004_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00032004_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00032004() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15083, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(32004) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00032004_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15083,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(32004) then
		return false;
	end

	package:DelItemAll(15083,1);

	player:AddExp(300000);
	player:getCoin(300000);
	player:getTael(40);
	return true;
end

--��������
function Task_00032004_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15083,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(32004);
end

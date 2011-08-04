--����Ľ�������
function Task_Accept_00000099()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(99) or task:HasCompletedTask(99) or task:HasSubmitedTask(99) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000099()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(99) or task:HasCompletedTask(99) or task:HasSubmitedTask(99) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000099()
	if GetPlayer():GetTaskMgr():HasCompletedTask(99) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000099(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(99) == npcId and Task_Accept_00000099 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 99
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "韩仙子";
	elseif task:GetTaskSubmitNpc(99) == npcId then
		if Task_Submit_00000099() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 99
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "韩仙子";
		elseif task:HasAcceptedTask(99) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 99
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "韩仙子";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000099_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然赤发寨主已灭，但是他修炼魔功所遗留的瘴气仍然残留在姑婆岭周围，后患无穷。离此不远有个白犀潭，相传有座地仙宫阙，如今是乙休真人的妻子韩仙子居住。"..GetPlayerName(GetPlayer()).."你可以去找韩仙子取些玉露符来给我，我会做法净化姑婆岭的瘴气。";
	action.m_ActionMsg = "这些瘴气果然污秽难当。";
	return action;
end

function Task_00000099_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你就是元敬常常跟我提起的那位后起之秀吧？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000099_step_table = {
		[1] = Task_00000099_step_01,
		[10] = Task_00000099_step_10,
		};

function Task_00000099_step(step)
	if Task_00000099_step_table[step] ~= nil then
		return Task_00000099_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000099_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000099() then
		return false;
	end
	if not task:AcceptTask(99) then
		return false;
	end
	task:AddTaskStep(99);
	return true;
end



--�ύ����
function Task_00000099_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(99) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000099_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(99);
end

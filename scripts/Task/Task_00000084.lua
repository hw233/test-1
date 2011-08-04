--����Ľ�������
function Task_Accept_00000084()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(84) or task:HasCompletedTask(84) or task:HasSubmitedTask(84) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000084()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(84) or task:HasCompletedTask(84) or task:HasSubmitedTask(84) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000084()
	if GetPlayer():GetTaskMgr():HasCompletedTask(84) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000084(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(84) == npcId and Task_Accept_00000084 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 84
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "魔阵对策";
	elseif task:GetTaskSubmitNpc(84) == npcId then
		if Task_Submit_00000084() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 84
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "魔阵对策";
		elseif task:HasAcceptedTask(84) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 84
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "魔阵对策";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000084_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "青螺魔宫里的八魔本领很是平常，只是他们这次邀来得帮手颇有几个厉害角色，尤其是滇西得毒龙尊者，他最近参悟了一个水火风雷魔阵毒辣无比。";
	action.m_ActionMsg = "那该怎么办是好？";
	return action;
end

function Task_00000084_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，在去魔宫的路上有一个清远寺，寺里的两个番僧虽然被妖人奴役，但是怀恨在心，他们修炼的天魔解体大法正是毒阵的克星，你这就去找他们商量下对策吧。";
	action.m_ActionMsg = "好的。我这就赶去。";
	return action;
end

function Task_00000084_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有你相助，想来我们解除被奴役的命运就不远了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000084_step_table = {
		[1] = Task_00000084_step_01,
		[2] = Task_00000084_step_02,
		[10] = Task_00000084_step_10,
		};

function Task_00000084_step(step)
	if Task_00000084_step_table[step] ~= nil then
		return Task_00000084_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000084_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000084() then
		return false;
	end
	if not task:AcceptTask(84) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(0,1);
	if itemNum ~= 0 then
		if itemNum > 0 then
			itemNum = 0;
			package:SetItem(0, itemNum, 1);
		end
		task:AddTaskStep2(84, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00000084_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(84) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000084_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(84);
end

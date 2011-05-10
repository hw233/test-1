--����Ľ�������
function Task_Accept_00020403()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20403) or task:HasCompletedTask(20403) or task:HasSubmitedTask(20403) then
		return false;
	end
	if not task:HasSubmitedTask(20402) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20403) or task:HasCompletedTask(20403) or task:HasSubmitedTask(20403) then
		return false;
	end
	if not task:HasSubmitedTask(20402) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20403) == npcId and Task_Accept_00020403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "了解甘露";
	elseif task:GetTaskSubmitNpc(20403) == npcId then
		if Task_Submit_00020403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "了解甘露";
		elseif task:HasAcceptedTask(20403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "了解甘露";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "原来你还不知道什么是甘露，但是你应该已经知道天魔之战的经过了吧，帝释天的狂妄无耻，让我们魔族忍无可忍，天魔之战的发生虽然对魔族和天族的毁灭性都很大，可是战争打了太久了，甘露正是帮助恢复和延长战斗力的，而这种东西只在天庭出产，天族因而占有一定的优势。";
	action.m_ActionMsg = "原来甘露有这么神奇的力量。";
	return action;
end

function Task_00020403_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "悦意公主为了我们的魔族，牺牲了自己的生活和身份，嫁给了帝释天，并且为我们换来了甘露。这个月的甘露，使者应该已经为我们送到了，你快去库房帮我取回来吧。";
	action.m_ActionMsg = "我们要珍惜悦意公主牺牲自己的幸福为我们换来的甘露。";
	return action;
end

function Task_00020403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在你看到了吧，你手上拿的这些就是甘露。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020403_step_table = {
		[1] = Task_00020403_step_01,
		[2] = Task_00020403_step_02,
		[10] = Task_00020403_step_10,
		};

function Task_00020403_step(step)
	if Task_00020403_step_table[step] ~= nil then
		return Task_00020403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20403) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15020,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15020, itemNum, 1);
		end
		task:AddTaskStep2(20403, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00020403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15020,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20403) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end
	package:DelItemAll(15020,1);

	player:AddExp(2200);
	player:getCoin(3100);
	player:getTael(10);
	return true;
end

--��������
function Task_00020403_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15020,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20403);
end

--����Ľ�������
function Task_Accept_00050403()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50403) or task:HasCompletedTask(50403) or task:HasSubmitedTask(50403) then
		return false;
	end
	if not task:HasSubmitedTask(50402) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(50403) or task:HasCompletedTask(50403) or task:HasSubmitedTask(50403) then
		return false;
	end
	if not task:HasSubmitedTask(50402) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50403) == npcId and Task_Accept_00050403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "炼丹材料";
	elseif task:GetTaskSubmitNpc(50403) == npcId then
		if Task_Submit_00050403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "炼丹材料";
		elseif task:HasAcceptedTask(50403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "炼丹材料";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "长生不老丹，重在其中包含了强大的内力，可以提供人在体力衰弱的时候所不能提供的力量，这种神秘丹药非一般的材料可以炼制，如果你真心的想要，就去十宝山的十宝寺，杀掉那里的邪恶剑士，从他们的尸体里收集一滴邪士之血。";
	action.m_ActionMsg = "这种事情对我来说太简单了。";
	return action;
end

function Task_00050403_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可是你要记得，这邪士之血，可是很难收集到的，这些剑士通常会在死之前将全身的气血耗尽，并不是那么容易收集的，你可能需要一点耐心。";
	action.m_ActionMsg = "那些人不人鬼不鬼的东西本不该存活在这个世上，我这就去。";
	return action;
end

function Task_00050403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "别说你还真有些让人没有预料到的运气啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050403_step_table = {
		[1] = Task_00050403_step_01,
		[2] = Task_00050403_step_02,
		[10] = Task_00050403_step_10,
		};

function Task_00050403_step(step)
	if Task_00050403_step_table[step] ~= nil then
		return Task_00050403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50403) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15049,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15049, itemNum, 1);
		end
		task:AddTaskStep2(50403, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00050403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15049,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8900,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50403) then
		return false;
	end

	if IsEquipTypeId(8900) then
		for k = 1, 1 do
			package:AddEquip(8900, 1);
		end
	else 
		package:AddItem(8900,1,1);
	end
	package:DelItemAll(15049,1);

	player:AddExp(1200);
	player:getCoin(1100);
	return true;
end

--��������
function Task_00050403_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15049,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50403);
end

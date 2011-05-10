--����Ľ�������
function Task_Accept_00040403()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40403) or task:HasCompletedTask(40403) or task:HasSubmitedTask(40403) then
		return false;
	end
	if not task:HasSubmitedTask(40402) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(40403) or task:HasCompletedTask(40403) or task:HasSubmitedTask(40403) then
		return false;
	end
	if not task:HasSubmitedTask(40402) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40403) == npcId and Task_Accept_00040403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "炼丹材料";
	elseif task:GetTaskSubmitNpc(40403) == npcId then
		if Task_Submit_00040403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "炼丹材料";
		elseif task:HasAcceptedTask(40403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40403
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
function Task_00040403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "长生不老丹，重在其中包含了强大的内力，可以提供人在体力衰老的时候所不能提供的力量，这种神秘丹药非一般的材料可以炼制，如果你真心的想要，就去瞻部峰的鬼雾山，杀掉那里的剧毒蜘蛛，收集一个稀有的毒囊。";
	action.m_ActionMsg = "这种事情对我来说太简单了。";
	return action;
end

function Task_00040403_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可是你要记得，这可是稀有的毒囊，并不是那么容易打到的，你可能需要一点耐心。";
	action.m_ActionMsg = "为了这不老丹，我一定会找到毒囊.";
	return action;
end

function Task_00040403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "别说你还真有些让人没有预料到的运气啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040403_step_table = {
		[1] = Task_00040403_step_01,
		[2] = Task_00040403_step_02,
		[10] = Task_00040403_step_10,
		};

function Task_00040403_step(step)
	if Task_00040403_step_table[step] ~= nil then
		return Task_00040403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40403) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15058,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15058, itemNum, 1);
		end
		task:AddTaskStep2(40403, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00040403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15058,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8900,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40403) then
		return false;
	end

	if IsEquipTypeId(8900) then
		for k = 1, 1 do
			package:AddEquip(8900, 1);
		end
	else 
		package:AddItem(8900,1,1);
	end
	package:DelItemAll(15058,1);

	player:AddExp(1200);
	player:getCoin(1050);
	return true;
end

--��������
function Task_00040403_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15058,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40403);
end

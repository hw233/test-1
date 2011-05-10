--����Ľ�������
function Task_Accept_00030701()
	local player = GetPlayer();
	if player:GetLev() < 47 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30701) or task:HasCompletedTask(30701) or task:HasSubmitedTask(30701) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30607) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30607) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 47 then
		return false;
	end
	if task:HasAcceptedTask(30701) or task:HasCompletedTask(30701) or task:HasSubmitedTask(30701) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30607) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30607) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30701) == npcId and Task_Accept_00030701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "寻找解药";
	elseif task:GetTaskSubmitNpc(30701) == npcId then
		if Task_Submit_00030701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "寻找解药";
		elseif task:HasAcceptedTask(30701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "寻找解药";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "谢谢你，为我们送情报的勇士，我们的守卫也告诉我了，你在这之前中了夜摩小鬼的毒，怕是毒性还会再次复发，要想解除这毒性，需要特别的解药。";
	action.m_ActionMsg = "要怎么获得呢？";
	return action;
end

function Task_00030701_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在无花山下的偏光洞里，有一种特别的巫毒蜘蛛，只有取回他们的毒液，以毒攻毒，才能将你身体里的毒排解掉，去吧，当你拿到了蜘蛛毒液，就去咸水岸找到我们的东山医官，他会帮助你的。";
	action.m_ActionMsg = "谢谢您的指示。";
	return action;
end

function Task_00030701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是吃了苦头，所以到这里来找我的吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030701_step_table = {
		[1] = Task_00030701_step_01,
		[2] = Task_00030701_step_02,
		[10] = Task_00030701_step_10,
		};

function Task_00030701_step(step)
	if Task_00030701_step_table[step] ~= nil then
		return Task_00030701_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30701) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15046,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15046, itemNum, 1);
		end
		task:AddTaskStep2(30701, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00030701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15046,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30701) then
		return false;
	end

	package:DelItemAll(15046,1);

	player:AddExp(44000);
	player:getCoin(31500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030701_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15046,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30701);
end

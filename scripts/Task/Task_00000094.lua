--����Ľ�������
function Task_Accept_00000094()
	local player = GetPlayer();
	if player:GetLev() < 47 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(94) or task:HasCompletedTask(94) or task:HasSubmitedTask(94) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000094()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 47 then
		return false;
	end
	if task:HasAcceptedTask(94) or task:HasCompletedTask(94) or task:HasSubmitedTask(94) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000094()
	if GetPlayer():GetTaskMgr():HasCompletedTask(94) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000094(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(94) == npcId and Task_Accept_00000094 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 94
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "枯禅";
	elseif task:GetTaskSubmitNpc(94) == npcId then
		if Task_Submit_00000094() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 94
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "枯禅";
		elseif task:HasAcceptedTask(94) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 94
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "枯禅";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000094_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "老身参这枯禅已经有数年之久，如今得了雪魂珠却还差几件物品才能得以圆满。一是九天元阳尺，二是需要两枚聚魄化形丹，这几样东西据说都被魏枫娘藏在魔宫之中，若你有机会找到，八姑将不胜感激。";
	action.m_ActionMsg = "救人于危难是我辈侠义本色。";
	return action;
end

function Task_00000094_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那"..GetPlayerName(GetPlayer()).."你是不是可以不要这个雪魂珠了，这件法宝晶莹剔透，我甚是喜爱，真是不舍得割爱啊。";
	action.m_ActionMsg = "咳……咳……咳……咳…………";
	return action;
end

function Task_00000094_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."这个雪魂珠我八姑就送给你了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000094_step_table = {
		[1] = Task_00000094_step_01,
		[2] = Task_00000094_step_02,
		[10] = Task_00000094_step_10,
		};

function Task_00000094_step(step)
	if Task_00000094_step_table[step] ~= nil then
		return Task_00000094_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000094_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000094() then
		return false;
	end
	if not task:AcceptTask(94) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(809,1);
	if itemNum ~= 0 then
		if itemNum > 2 then
			itemNum = 2;
			package:SetItem(809, itemNum, 1);
		end
		task:AddTaskStep2(94, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00000094_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(809,1) < 2 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(94) then
		return false;
	end

	package:DelItemAll(809,1);

	player:AddExp(40000);
	return true;
end

--��������
function Task_00000094_abandon()
	local package = GetPlayer():GetPackage();
	local itemNum = 0;
	package:DelItemAll(809,1);
	return GetPlayer():GetTaskMgr():AbandonTask(94);
end

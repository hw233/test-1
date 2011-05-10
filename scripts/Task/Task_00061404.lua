--����Ľ�������
function Task_Accept_00061404()
	local player = GetPlayer();
	if player:GetLev() < 56 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61404) or task:HasCompletedTask(61404) or task:HasSubmitedTask(61404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61403) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061404()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 56 then
		return false;
	end
	if task:HasAcceptedTask(61404) or task:HasCompletedTask(61404) or task:HasSubmitedTask(61404) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61403) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61403) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061404()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61404) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061404(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61404) == npcId and Task_Accept_00061404 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61404
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "猖狂僵尸";
	elseif task:GetTaskSubmitNpc(61404) == npcId then
		if Task_Submit_00061404() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61404
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "猖狂僵尸";
		elseif task:HasAcceptedTask(61404) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61404
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "猖狂僵尸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061404_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你在孤冷山大战原始毒龙的时候有没有注意到那里的乱骷崖出现了许多的恐怖的僵尸。";
	action.m_ActionMsg = "你怎么不一起说，是让我再去一趟吧。";
	return action;
end

function Task_00061404_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯是很不好意思，的确是需要勇士你再去一趟，前面实在是因为远征军的报告不完整，不然只需要你去一次就搞定了。";
	action.m_ActionMsg = "那我就再去一次好了，没问题的，我也很讨厌僵尸。";
	return action;
end

function Task_00061404_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你可真是横扫孤冷山啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061404_step_table = {
		[1] = Task_00061404_step_01,
		[2] = Task_00061404_step_02,
		[10] = Task_00061404_step_10,
		};

function Task_00061404_step(step)
	if Task_00061404_step_table[step] ~= nil then
		return Task_00061404_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061404_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061404() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61404) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061404_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8912,2,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61404) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 2 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,2,1);
	end

	player:AddExp(80000);
	player:getCoin(75000);
	return true;
end

--��������
function Task_00061404_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61404);
end

--����Ľ�������
function Task_Accept_00030802()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30802) or task:HasCompletedTask(30802) or task:HasSubmitedTask(30802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30801) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030802()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(30802) or task:HasCompletedTask(30802) or task:HasSubmitedTask(30802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30801) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030802()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30802) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030802(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30802) == npcId and Task_Accept_00030802 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30802
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "食人鱼鳞";
	elseif task:GetTaskSubmitNpc(30802) == npcId then
		if Task_Submit_00030802() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30802
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "食人鱼鳞";
		elseif task:HasAcceptedTask(30802) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30802
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "食人鱼鳞";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030802_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，我对于你的帮助很感动，我是个经历坎坷的人，镇邪派的历程也很坎坷，如果之前有什么对待不周的地方，还希望你能理解。";
	action.m_ActionMsg = "没关系的。";
	return action;
end

function Task_00030802_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "为那位投靠我们的名将打造装备的材料，还需要码头废墟那儿的陆上食人鱼的鱼鳞，还要麻烦勇士你再去一趟了。";
	action.m_ActionMsg = "这些都是小事，只是希望那位将士能早日同我们一起征战。";
	return action;
end

function Task_00030802_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯太好了，最重要的材料已经有了";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030802_step_table = {
		[1] = Task_00030802_step_01,
		[2] = Task_00030802_step_02,
		[10] = Task_00030802_step_10,
		};

function Task_00030802_step(step)
	if Task_00030802_step_table[step] ~= nil then
		return Task_00030802_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030802_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030802() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30802) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15061,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15061, itemNum, 1);
		end
		task:AddTaskStep2(30802, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00030802_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15061,1) < 12 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8905,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30802) then
		return false;
	end

	if IsEquipTypeId(8905) then
		for k = 1, 1 do
			package:AddEquip(8905, 1);
		end
	else 
		package:AddItem(8905,1,1);
	end
	package:DelItemAll(15061,1);

	player:AddExp(56000);
	player:getCoin(43500);
	player:getTael(20);
	return true;
end

--��������
function Task_00030802_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15061,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30802);
end

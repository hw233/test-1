--����Ľ�������
function Task_Accept_00031002()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 57 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31002) or task:HasCompletedTask(31002) or task:HasSubmitedTask(31002) then
		return false;
	end
	if not task:HasSubmitedTask(31001) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031002()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 57 then
		return false;
	end
	if task:HasAcceptedTask(31002) or task:HasCompletedTask(31002) or task:HasSubmitedTask(31002) then
		return false;
	end
	if not task:HasSubmitedTask(31001) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031002()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31002) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031002(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31002) == npcId and Task_Accept_00031002 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31002
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "护送如烟";
	elseif task:GetTaskAcceptNpc(31002) == npcId and task:HasAcceptedTask(31002) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31002
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "护送如烟";
	elseif task:GetTaskSubmitNpc(31002) == npcId then
		if Task_Submit_00031002() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31002
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "护送如烟";
		elseif task:HasAcceptedTask(31002) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31002
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "护送如烟";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031002_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "善良的勇士，能不能送我离开这里啊，好多天来，我都不敢出去，外面到处都有危险，如果找不到我丈夫，起码让我踏上回魔族的路也好。";
	action.m_ActionMsg = "天族的人真是毫无责任感啊，我当然会保护你回去。\n";
	return action;
end

function Task_00031002_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."你真是太好了，这么多天我吃了不少苦，终于等到了帮助我的人了。";
	action.m_ActionMsg = "别担心，我们走吧。";
	return action;
end

function Task_00031002_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你是什么人？对如烟做了什么？";
	action.m_ActionMsg = "";
	return action;
end

function Task_00031002_step_11()
	RunConveyAction(GetPlayer(),31002);
end

local Task_00031002_step_table = {
		[1] = Task_00031002_step_01,
		[2] = Task_00031002_step_02,
		[10] = Task_00031002_step_10,
		[11] = Task_00031002_step_11,
		};

function Task_00031002_step(step)
	if Task_00031002_step_table[step] ~= nil then
		return Task_00031002_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031002_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031002() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31002) then
		return false;
	end
	RunConveyAction(player,31002);
	return true;
end



--�ύ����
function Task_00031002_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(505,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31002) then
		return false;
	end

	if IsEquipTypeId(505) then
		for k = 1, 1 do
			package:AddEquip(505, 1);
		end
	else 
		package:AddItem(505,1,1);
	end

	player:AddExp(84000);
	player:getCoin(84000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031002_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31002);
end

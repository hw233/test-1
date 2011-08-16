--����Ľ�������
function Task_Accept_00000006()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(6) or task:HasCompletedTask(6) or task:HasSubmitedTask(6) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000006()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(6) or task:HasCompletedTask(6) or task:HasSubmitedTask(6) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000006()
	if GetPlayer():GetTaskMgr():HasCompletedTask(6) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000006(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(6) == npcId and Task_Accept_00000006 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 6
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "回报周淳";
	elseif task:GetTaskSubmitNpc(6) == npcId then
		if Task_Submit_00000006() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 6
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "回报周淳";
		elseif task:HasAcceptedTask(6) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 6
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "回报周淳";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000006_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看来这些古籍在我这里还真是明珠蒙尘了，周大哥的意思我已知晓，我一定好好教导这些幼童，你们就放心吧，只是剑侠之事我了解不多，关于这个剑侠心法我觉得你应该去请教一下周大哥才好。";
	action.m_ActionMsg = "好，我这就去找周前辈。";
	return action;
end

function Task_00000006_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哎呀，这可是传说中修仙之人练习的功法啊，少侠你真是有福缘，想不到马湘这个秀才竟然还收藏着这个。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000006_step_table = {
		[1] = Task_00000006_step_01,
		[10] = Task_00000006_step_10,
		};

function Task_00000006_step(step)
	if Task_00000006_step_table[step] ~= nil then
		return Task_00000006_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000006_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000006() then
		return false;
	end
	if not task:AcceptTask(6) then
		return false;
	end
	task:AddTaskStep(6);
	return true;
end



--�ύ����
function Task_00000006_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(2007,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(6) then
		return false;
	end

	if IsEquipTypeId(2007) then
		for k = 1, 1 do
			package:AddEquip(2007, 1);
		end
	else 
		package:AddItem(2007,1,1);
	end

	player:AddExp(860);
	player:getTael(200);
	return true;
end

--��������
function Task_00000006_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(6);
end

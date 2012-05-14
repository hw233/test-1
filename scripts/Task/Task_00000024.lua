--����Ľ�������
function Task_Accept_00000024()
	local player = GetPlayer();
	if player:GetLev() < 15 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(24) or task:HasCompletedTask(24) or task:HasSubmitedTask(24) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000024()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 15 then
		return false;
	end
	if task:HasAcceptedTask(24) or task:HasCompletedTask(24) or task:HasSubmitedTask(24) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000024()
	if GetPlayer():GetTaskMgr():HasCompletedTask(24) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000024(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(24) == npcId and Task_Accept_00000024 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 24
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "教训恶人";
	elseif task:GetTaskSubmitNpc(24) == npcId then
		if Task_Submit_00000024() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 24
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "教训恶人";
		elseif task:HasAcceptedTask(24) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 24
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "教训恶人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000024_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."不错，不错，我老人家就喜欢象你这样侠肝义胆的年青人，你看这边两个就和你完全不一样，没几分本事却在那里胡乱吹牛，说不得要好好教训他们一下。";
	action.m_ActionMsg = "老前辈，你过奖了。";
	return action;
end

function Task_00000024_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年青人太谦虚了，来，来，来，让我来给你掠阵，你尽管放手施为，教训一下这两个妖人。";
	action.m_ActionMsg = "晚辈遵命。";
	return action;
end

function Task_00000024_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "打的两个妖人屁滚尿流，哈哈！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000024_step_table = {
		[1] = Task_00000024_step_01,
		[2] = Task_00000024_step_02,
		[10] = Task_00000024_step_10,
		};

function Task_00000024_step(step)
	if Task_00000024_step_table[step] ~= nil then
		return Task_00000024_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000024_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000024() then
		return false;
	end
	if not task:AcceptTask(24) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000024_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(509,1,1) + package:GetItemUsedGrids(516,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(24) then
		return false;
	end

	if IsEquipTypeId(509) then
		for k = 1, 1 do
			package:AddEquip(509, 1);
		end
	else 
		package:AddItem(509,1,1);
	end
	if IsEquipTypeId(516) then
		for k = 1, 1 do
			package:AddEquip(516, 1);
		end
	else 
		package:AddItem(516,1,1);
	end

	player:AddExp(3800);
	return true;
end

--��������
function Task_00000024_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(24);
end

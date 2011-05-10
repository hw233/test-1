--����Ľ�������
function Task_Accept_00031902()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31902) or task:HasCompletedTask(31902) or task:HasSubmitedTask(31902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031902()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(31902) or task:HasCompletedTask(31902) or task:HasSubmitedTask(31902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31901) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031902()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31902) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031902(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31902) == npcId and Task_Accept_00031902 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31902
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "追思吕布";
	elseif task:GetTaskSubmitNpc(31902) == npcId then
		if Task_Submit_00031902() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31902
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "追思吕布";
		elseif task:HasAcceptedTask(31902) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31902
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "追思吕布";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031902_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不过尽管这样，我还是相信上天注定了我与吕布的缘分未尽，既然我能追到山海大陆来，我就一定要想办法再见到他，这个过程有点复杂，而且我需要帮忙，你愿意帮助我吗？";
	action.m_ActionMsg = "我一定会帮忙的。";
	return action;
end

function Task_00031902_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然你愿意帮忙，那我就不追究了，希望你用心的帮我。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031902_step_table = {
		[1] = Task_00031902_step_01,
		[10] = Task_00031902_step_10,
		};

function Task_00031902_step(step)
	if Task_00031902_step_table[step] ~= nil then
		return Task_00031902_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031902_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031902() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31902) then
		return false;
	end
	task:AddTaskStep(31902);
	return true;
end



--�ύ����
function Task_00031902_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8915,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31902) then
		return false;
	end

	if IsEquipTypeId(8915) then
		for k = 1, 1 do
			package:AddEquip(8915, 1);
		end
	else 
		package:AddItem(8915,1,1);
	end

	player:AddExp(230000);
	player:getCoin(217500);
	player:getTael(40);
	return true;
end

--��������
function Task_00031902_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31902);
end

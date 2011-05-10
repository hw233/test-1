--����Ľ�������
function Task_Accept_00010105()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10105) or task:HasCompletedTask(10105) or task:HasSubmitedTask(10105) then
		return false;
	end
	if not task:HasSubmitedTask(10104) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(10105) or task:HasCompletedTask(10105) or task:HasSubmitedTask(10105) then
		return false;
	end
	if not task:HasSubmitedTask(10104) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10105) == npcId and Task_Accept_00010105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "危机四伏";
	elseif task:GetTaskSubmitNpc(10105) == npcId then
		if Task_Submit_00010105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "危机四伏";
		elseif task:HasAcceptedTask(10105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "危机四伏";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我最近非常需要人手，你来的正好，一直听城郊的族民说最近受到很多怪物的骚扰，对他们的生活影响很大，我现在要你去那里查个究竟。";
	action.m_ActionMsg = "居然有怪物敢骚扰我们的族民，我会去查看清楚的。";
	return action;
end

function Task_00010105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是这些邪恶的家伙，幸好你替我发现了他们。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010105_step_table = {
		[1] = Task_00010105_step_01,
		[10] = Task_00010105_step_10,
		};

function Task_00010105_step(step)
	if Task_00010105_step_table[step] ~= nil then
		return Task_00010105_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10105) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00010105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(5,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10105) then
		return false;
	end

	if IsEquipTypeId(5) then
		for k = 1, 1 do
			package:AddEquip(5, 1);
		end
	else 
		package:AddItem(5,1,1);
	end

	player:AddExp(600);
	player:getCoin(700);
	player:getTael(5);
	return true;
end

--��������
function Task_00010105_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10105);
end

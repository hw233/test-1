--����Ľ�������
function Task_Accept_00050407()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50407) or task:HasCompletedTask(50407) or task:HasSubmitedTask(50407) then
		return false;
	end
	if not task:HasSubmitedTask(50406) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050407()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(50407) or task:HasCompletedTask(50407) or task:HasSubmitedTask(50407) then
		return false;
	end
	if not task:HasSubmitedTask(50406) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050407()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50407) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050407(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50407) == npcId and Task_Accept_00050407 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50407
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "真谛显现";
	elseif task:GetTaskSubmitNpc(50407) == npcId then
		if Task_Submit_00050407() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50407
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "真谛显现";
		elseif task:HasAcceptedTask(50407) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50407
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "真谛显现";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050407_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "通过显无意和一峰道长的指导，勇士你现在了解到了永生的真谛了吗？";
	action.m_ActionMsg = "略有所悟吧。";
	return action;
end

function Task_00050407_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "所谓的长生丹，所蕴含的力量，只不过是可以使你在实力受损时更好的回复，而不是无论你面临什么样的危险都能够活下来，真正的永生是不存在的，只有通过无尽的磨练提升自己的实力，才能战胜所有的危险和困难而接近永生。";
	action.m_ActionMsg = "现在我明白了。";
	return action;
end

function Task_00050407_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然你能明白，很好，也算这一切都经历都没有白费。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050407_step_table = {
		[1] = Task_00050407_step_01,
		[2] = Task_00050407_step_02,
		[10] = Task_00050407_step_10,
		};

function Task_00050407_step(step)
	if Task_00050407_step_table[step] ~= nil then
		return Task_00050407_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050407_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050407() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50407) then
		return false;
	end
	task:AddTaskStep(50407);
	return true;
end



--�ύ����
function Task_00050407_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8901,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50407) then
		return false;
	end

	if IsEquipTypeId(8901) then
		for k = 1, 1 do
			package:AddEquip(8901, 1);
		end
	else 
		package:AddItem(8901,1,1);
	end

	player:AddExp(1200);
	player:getCoin(1500);
	return true;
end

--��������
function Task_00050407_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50407);
end

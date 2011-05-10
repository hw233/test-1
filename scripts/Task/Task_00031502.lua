--����Ľ�������
function Task_Accept_00031502()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31502) or task:HasCompletedTask(31502) or task:HasSubmitedTask(31502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31501) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(31502) or task:HasCompletedTask(31502) or task:HasSubmitedTask(31502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31501) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31502) == npcId and Task_Accept_00031502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "发现吕布";
	elseif task:GetTaskSubmitNpc(31502) == npcId then
		if Task_Submit_00031502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "发现吕布";
		elseif task:HasAcceptedTask(31502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "发现吕布";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你的帮忙真是太及时了，你带回的这封密信，竟然是夜摩盟与名将吕布的密谋信。";
	action.m_ActionMsg = "难道名将吕布也在山海大陆，但是投靠的是夜摩盟？";
	return action;
end

function Task_00031502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "信里是这样写的，夜摩盟似乎许诺了他非常优越的条件，并且在他宣布效忠夜摩盟期间，整个夜摩盟的精锐部队，任他挑选，难怪吕布会投靠夜摩盟啊。你不妨去一趟常泉寺，吕布就在那里，去了解一下他到底是怎么想的吧。";
	action.m_ActionMsg = "原来是这样啊，我也很想会一会名将吕布。";
	return action;
end

function Task_00031502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻人，你的表情我读不懂啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031502_step_table = {
		[1] = Task_00031502_step_01,
		[2] = Task_00031502_step_02,
		[10] = Task_00031502_step_10,
		};

function Task_00031502_step(step)
	if Task_00031502_step_table[step] ~= nil then
		return Task_00031502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031502() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31502) then
		return false;
	end
	task:AddTaskStep(31502);
	return true;
end



--�ύ����
function Task_00031502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8916,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31502) then
		return false;
	end

	if IsEquipTypeId(8916) then
		for k = 1, 1 do
			package:AddEquip(8916, 1);
		end
	else 
		package:AddItem(8916,1,1);
	end

	player:AddExp(180000);
	player:getCoin(156000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031502_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31502);
end

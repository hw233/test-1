--����Ľ�������
function Task_Accept_00031202()
	local player = GetPlayer();
	if player:GetLev() < 59 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31202) or task:HasCompletedTask(31202) or task:HasSubmitedTask(31202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 59 then
		return false;
	end
	if task:HasAcceptedTask(31202) or task:HasCompletedTask(31202) or task:HasSubmitedTask(31202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31201) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31202) == npcId and Task_Accept_00031202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "摩睺的秘密";
	elseif task:GetTaskSubmitNpc(31202) == npcId then
		if Task_Submit_00031202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "摩睺的秘密";
		elseif task:HasAcceptedTask(31202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "摩睺的秘密";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你这么快就回来了？将那些强壮的疯狂雪怪收服并且来归顺我了吗？";
	action.m_ActionMsg = "……我将他们全部杀死了。";
	return action;
end

function Task_00031202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你是傻子吗？我是让你将他们收服来归顺我，以便增强我的势力，你却将他们杀死，简直傻到不行。";
	action.m_ActionMsg = "你如此居心叵测，居然还怪我？你是夜摩盟的吧。";
	return action;
end

function Task_00031202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦我的天啊，怎么能这样。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031202_step_table = {
		[1] = Task_00031202_step_01,
		[2] = Task_00031202_step_02,
		[10] = Task_00031202_step_10,
		};

function Task_00031202_step(step)
	if Task_00031202_step_table[step] ~= nil then
		return Task_00031202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31202) then
		return false;
	end
	task:AddTaskStep(31202);
	return true;
end



--�ύ����
function Task_00031202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8914,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31202) then
		return false;
	end

	if IsEquipTypeId(8914) then
		for k = 1, 1 do
			package:AddEquip(8914, 1);
		end
	else 
		package:AddItem(8914,1,1);
	end

	player:AddExp(100000);
	player:getCoin(100000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31202);
end

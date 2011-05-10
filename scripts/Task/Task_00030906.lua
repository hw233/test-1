--����Ľ�������
function Task_Accept_00030906()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30906) or task:HasCompletedTask(30906) or task:HasSubmitedTask(30906) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30905) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30905) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030906()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(30906) or task:HasCompletedTask(30906) or task:HasSubmitedTask(30906) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30905) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30905) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030906()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30906) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030906(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30906) == npcId and Task_Accept_00030906 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30906
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "传令布防";
	elseif task:GetTaskSubmitNpc(30906) == npcId then
		if Task_Submit_00030906() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30906
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "传令布防";
		elseif task:HasAcceptedTask(30906) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30906
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "传令布防";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030906_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你真是帮了我们的大忙，虽然没有直接找到摩可拿，但是追查到他的下落也很不错。";
	action.m_ActionMsg = "这些对我来说都不在话下。";
	return action;
end

function Task_00030906_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在麻烦你将这封军事密信送到摩加湖的摩封园，交给墨严将军，让他了解我们最近的情报，以便做好防御工事，警惕摩可拿的偷袭。";
	action.m_ActionMsg = "好的，我这就去。";
	return action;
end

function Task_00030906_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多闻把这么重要的任务交给我，我一定会做到最好的防守。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030906_step_table = {
		[1] = Task_00030906_step_01,
		[2] = Task_00030906_step_02,
		[10] = Task_00030906_step_10,
		};

function Task_00030906_step(step)
	if Task_00030906_step_table[step] ~= nil then
		return Task_00030906_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030906_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030906() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15066, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30906) then
		return false;
	end
	task:AddTaskStep(30906);
	return true;
end



--�ύ����
function Task_00030906_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15066,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(445,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30906) then
		return false;
	end

	if IsEquipTypeId(445) then
		for k = 1, 1 do
			package:AddEquip(445, 1);
		end
	else 
		package:AddItem(445,1,1);
	end
	package:DelItemAll(15066,1);

	player:AddExp(76000);
	player:getCoin(70000);
	player:getTael(30);
	return true;
end

--��������
function Task_00030906_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15066,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30906);
end

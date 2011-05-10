--����Ľ�������
function Task_Accept_00060203()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 31 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60203) or task:HasCompletedTask(60203) or task:HasSubmitedTask(60203) then
		return false;
	end
	if not task:HasSubmitedTask(60202) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 31 then
		return false;
	end
	if task:HasAcceptedTask(60203) or task:HasCompletedTask(60203) or task:HasSubmitedTask(60203) then
		return false;
	end
	if not task:HasSubmitedTask(60202) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60203) == npcId and Task_Accept_00060203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "武装升级";
	elseif task:GetTaskSubmitNpc(60203) == npcId then
		if Task_Submit_00060203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "武装升级";
		elseif task:HasAcceptedTask(60203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "武装升级";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "不过勇士我想你也能猜到，夜摩盟的狼鬼族们会有这样的计划，可见他们的一切邪恶计划都是有计划有准备的，可见他们的野心之大，我们绝对不能坐以待毙。";
	action.m_ActionMsg = "当然不能。";
	return action;
end

function Task_00060203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我已经准备好了一大批上等的刀剑装备，希望你能帮我将这些送给镇前所的前线士兵，让他们全部换上这些高级的武器装备，加强训练和防守。";
	action.m_ActionMsg = "好的，我现在就去。";
	return action;
end

function Task_00060203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们的责任更重了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060203_step_table = {
		[1] = Task_00060203_step_01,
		[2] = Task_00060203_step_02,
		[10] = Task_00060203_step_10,
		};

function Task_00060203_step(step)
	if Task_00060203_step_table[step] ~= nil then
		return Task_00060203_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060203() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15002, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(60203) then
		return false;
	end
	task:AddTaskStep(60203);
	return true;
end



--�ύ����
function Task_00060203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15002,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8905,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60203) then
		return false;
	end

	if IsEquipTypeId(8905) then
		for k = 1, 1 do
			package:AddEquip(8905, 1);
		end
	else 
		package:AddItem(8905,1,1);
	end
	package:DelItemAll(15002,1);

	player:AddExp(4000);
	player:getCoin(5600);
	return true;
end

--��������
function Task_00060203_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15002,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60203);
end

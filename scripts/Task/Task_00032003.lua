--����Ľ�������
function Task_Accept_00032003()
	local player = GetPlayer();
	if player:GetLev() < 74 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(32003) or task:HasCompletedTask(32003) or task:HasSubmitedTask(32003) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32002) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32002) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00032003()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 74 then
		return false;
	end
	if task:HasAcceptedTask(32003) or task:HasCompletedTask(32003) or task:HasSubmitedTask(32003) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32002) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32002) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00032003()
	if GetPlayer():GetTaskMgr():HasCompletedTask(32003) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00032003(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(32003) == npcId and Task_Accept_00032003 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 32003
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "直取头颅";
	elseif task:GetTaskSubmitNpc(32003) == npcId then
		if Task_Submit_00032003() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32003
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "直取头颅";
		elseif task:HasAcceptedTask(32003) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32003
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "直取头颅";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00032003_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我就知道终归会有这一天的来临，我们苦苦备战多日，终于到了要解决历久的仇恨的时机了。放心吧，即使东山派的不来找我们，我们也会积极的协助他们共同抵抗夜摩盟的。";
	action.m_ActionMsg = "如果是这样就太好了。";
	return action;
end

function Task_00032003_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如果可以，就请你先去灰岩塔，杀掉那里的灰岩守卫，砍下他们的头颅并带回来。";
	action.m_ActionMsg = "已经等不及了，我现在就去。";
	return action;
end

function Task_00032003_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我的激情已经开始燃烧了，等待多年的事情终于快要看到结果了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00032003_step_table = {
		[1] = Task_00032003_step_01,
		[2] = Task_00032003_step_02,
		[10] = Task_00032003_step_10,
		};

function Task_00032003_step(step)
	if Task_00032003_step_table[step] ~= nil then
		return Task_00032003_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00032003_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00032003() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(32003) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15083,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15083, itemNum, 1);
		end
		task:AddTaskStep2(32003, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00032003_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15083,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8927,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(32003) then
		return false;
	end

	if IsEquipTypeId(8927) then
		for k = 1, 1 do
			package:AddEquip(8927, 1);
		end
	else 
		package:AddItem(8927,1,1);
	end
	package:DelItemAll(15083,1);

	player:AddExp(290000);
	player:getCoin(275000);
	player:getTael(40);
	return true;
end

--��������
function Task_00032003_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15083,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(32003);
end

--����Ľ�������
function Task_Accept_00010305()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10305) or task:HasCompletedTask(10305) or task:HasSubmitedTask(10305) then
		return false;
	end
	if not task:HasSubmitedTask(10304) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010305()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10305) or task:HasCompletedTask(10305) or task:HasSubmitedTask(10305) then
		return false;
	end
	if not task:HasSubmitedTask(10304) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010305()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10305) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010305(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10305) == npcId and Task_Accept_00010305 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10305
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "传递密令";
	elseif task:GetTaskSubmitNpc(10305) == npcId then
		if Task_Submit_00010305() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10305
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "传递密令";
		elseif task:HasAcceptedTask(10305) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10305
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "传递密令";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010305_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "现在我要你帮我秘密传令下去，告诉在善法堂的护法军师，即日起开始减少给魔族甘露的供应，到下个月就完全停止供应。";
	action.m_ActionMsg = "我这就帮您传令下去。";
	return action;
end

function Task_00010305_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不能再这样迁就魔族的贪婪了，但是悦意王妃那里，最好不要让她知道这件事情。";
	action.m_ActionMsg = "您的考虑很有道理，我会遵照您的吩咐做的。";
	return action;
end

function Task_00010305_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "接令。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010305_step_table = {
		[1] = Task_00010305_step_01,
		[2] = Task_00010305_step_02,
		[10] = Task_00010305_step_10,
		};

function Task_00010305_step(step)
	if Task_00010305_step_table[step] ~= nil then
		return Task_00010305_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010305_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010305() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15023, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10305) then
		return false;
	end
	task:AddTaskStep(10305);
	return true;
end



--�ύ����
function Task_00010305_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15023,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8925,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10305) then
		return false;
	end

	if IsEquipTypeId(8925) then
		for k = 1, 1 do
			package:AddEquip(8925, 1);
		end
	else 
		package:AddItem(8925,1,1);
	end
	package:DelItemAll(15023,1);

	player:AddExp(1800);
	player:getCoin(2300);
	return true;
end

--��������
function Task_00010305_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15023,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10305);
end

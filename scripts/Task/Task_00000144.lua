--����Ľ�������
function Task_Accept_00000144()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(144) or task:HasCompletedTask(144) or task:HasSubmitedTask(144) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000144()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(144) or task:HasCompletedTask(144) or task:HasSubmitedTask(144) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(143) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000144()
	if GetPlayer():GetTaskMgr():HasCompletedTask(144) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000144(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(144) == npcId and Task_Accept_00000144 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 144
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "白骨神君";
	elseif task:GetTaskSubmitNpc(144) == npcId then
		if Task_Submit_00000144() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 144
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "白骨神君";
		elseif task:HasAcceptedTask(144) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 144
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "白骨神君";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000144_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "庐山有一处神魔洞，里面藏这个白骨神君，一身邪道异术非常了得。如今三尸墓妖孽作乱，必须趁早除掉庐山周围的隐患，以免将来我们对付妖尸的时候出现意外，我见少侠剑术高强，想来那白骨神君定然不是你的对手。";
	action.m_ActionMsg = "师太莫愁，我这就去消灭他们。";
	return action;
end

function Task_00000144_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，少侠真是剑术高强，一身正气啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000144_step_table = {
		[1] = Task_00000144_step_01,
		[10] = Task_00000144_step_10,
		};

function Task_00000144_step(step)
	if Task_00000144_step_table[step] ~= nil then
		return Task_00000144_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000144_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000144() then
		return false;
	end
	if not task:AcceptTask(144) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000144_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1267,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(144) then
		return false;
	end

	if IsEquipTypeId(1267) then
		for k = 1, 1 do
			package:AddEquip(1267, 1);
		end
	else 
		package:AddItem(1267,1,1);
	end

	player:AddExp(100000);
	return true;
end

--��������
function Task_00000144_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(144);
end

--����Ľ�������
function Task_Accept_00031004()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 57 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31004) or task:HasCompletedTask(31004) or task:HasSubmitedTask(31004) then
		return false;
	end
	if not task:HasSubmitedTask(31003) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031004()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 57 then
		return false;
	end
	if task:HasAcceptedTask(31004) or task:HasCompletedTask(31004) or task:HasSubmitedTask(31004) then
		return false;
	end
	if not task:HasSubmitedTask(31003) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031004()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31004) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031004(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31004) == npcId and Task_Accept_00031004 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31004
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "加强防卫";
	elseif task:GetTaskSubmitNpc(31004) == npcId then
		if Task_Submit_00031004() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31004
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "加强防卫";
		elseif task:HasAcceptedTask(31004) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31004
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "加强防卫";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031004_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "欢迎你回来勇士，我听说了你的事迹，我认为你做的非常好，那些狂妄的天族，无论走到哪里都改不了他们的本性。";
	action.m_ActionMsg = "就是。";
	return action;
end

function Task_00031004_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那么还需要你去一趟镇前所，告诉那里的前线士兵，让他们在防卫夜摩盟的同时，也不要放松对天族的警惕。";
	action.m_ActionMsg = "好的，我这就去告诉他们。";
	return action;
end

function Task_00031004_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，感谢你送来的消息，我们一定会加紧防范，任何时候都不会放松警惕的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031004_step_table = {
		[1] = Task_00031004_step_01,
		[2] = Task_00031004_step_02,
		[10] = Task_00031004_step_10,
		};

function Task_00031004_step(step)
	if Task_00031004_step_table[step] ~= nil then
		return Task_00031004_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031004_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031004() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31004) then
		return false;
	end
	task:AddTaskStep(31004);
	return true;
end



--�ύ����
function Task_00031004_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8925,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31004) then
		return false;
	end

	if IsEquipTypeId(8925) then
		for k = 1, 1 do
			package:AddEquip(8925, 1);
		end
	else 
		package:AddItem(8925,1,1);
	end

	player:AddExp(84000);
	player:getCoin(84000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031004_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31004);
end

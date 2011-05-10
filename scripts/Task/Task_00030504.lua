--����Ľ�������
function Task_Accept_00030504()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30504) or task:HasCompletedTask(30504) or task:HasSubmitedTask(30504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(30504) or task:HasCompletedTask(30504) or task:HasSubmitedTask(30504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30503) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30504) == npcId and Task_Accept_00030504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "保护随形";
	elseif task:GetTaskSubmitNpc(30504) == npcId then
		if Task_Submit_00030504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "保护随形";
		elseif task:HasAcceptedTask(30504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "保护随形";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "感谢你为我们做的这一切勇士，东山派为有你这样的朋友感到荣幸，请你回到山河郡的河川旅店见随形一下吧，确保她没有遭到那些僵尸刺客的偷袭，咱们后会有期。";
	action.m_ActionMsg = "好的，帮主再会。";
	return action;
end

function Task_00030504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士这么急匆匆的就回来了，有什么急事吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030504_step_table = {
		[1] = Task_00030504_step_01,
		[10] = Task_00030504_step_10,
		};

function Task_00030504_step(step)
	if Task_00030504_step_table[step] ~= nil then
		return Task_00030504_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30504) then
		return false;
	end
	task:AddTaskStep(30504);
	return true;
end



--�ύ����
function Task_00030504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8925,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30504) then
		return false;
	end

	if IsEquipTypeId(8925) then
		for k = 1, 1 do
			package:AddEquip(8925, 1);
		end
	else 
		package:AddItem(8925,1,1);
	end

	player:AddExp(10000);
	player:getCoin(13500);
	return true;
end

--��������
function Task_00030504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30504);
end

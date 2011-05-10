--����Ľ�������
function Task_Accept_00061803()
	local player = GetPlayer();
	if player:GetLev() < 66 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61803) or task:HasCompletedTask(61803) or task:HasSubmitedTask(61803) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61802) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61802) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061803()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 66 then
		return false;
	end
	if task:HasAcceptedTask(61803) or task:HasCompletedTask(61803) or task:HasSubmitedTask(61803) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61802) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61802) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061803()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61803) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061803(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61803) == npcId and Task_Accept_00061803 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61803
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "等待恢复";
	elseif task:GetTaskSubmitNpc(61803) == npcId then
		if Task_Submit_00061803() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61803
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "等待恢复";
		elseif task:HasAcceptedTask(61803) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61803
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "等待恢复";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061803_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "年轻的勇士，你这身上的印记是哪个派别的啊？";
	action.m_ActionMsg = "我现在正与东山派为伍，他们也希望你能加入。\n";
	return action;
end

function Task_00061803_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是这样，那我一定要加入你们，但是现在我武力尽失，似乎短时间内帮不上什么忙了，我正在想办法恢复，等我状态恢复了我就会去找游莫大将的，麻烦你回去告知一下吧。";
	action.m_ActionMsg = "好的，那么回见了。";
	return action;
end

function Task_00061803_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然僧格林沁有心加入我们就好，就先给他时间恢复吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061803_step_table = {
		[1] = Task_00061803_step_01,
		[2] = Task_00061803_step_02,
		[10] = Task_00061803_step_10,
		};

function Task_00061803_step(step)
	if Task_00061803_step_table[step] ~= nil then
		return Task_00061803_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061803_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061803() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61803) then
		return false;
	end
	task:AddTaskStep(61803);
	return true;
end



--�ύ����
function Task_00061803_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8902,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61803) then
		return false;
	end

	if IsEquipTypeId(8902) then
		for k = 1, 1 do
			package:AddEquip(8902, 1);
		end
	else 
		package:AddItem(8902,1,1);
	end

	player:AddExp(190000);
	player:getCoin(165000);
	return true;
end

--��������
function Task_00061803_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61803);
end

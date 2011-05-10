--����Ľ�������
function Task_Accept_00090104()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90104) or task:HasCompletedTask(90104) or task:HasSubmitedTask(90104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90104) or task:HasCompletedTask(90104) or task:HasSubmitedTask(90104) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90103) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90104) == npcId and Task_Accept_00090104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "只欠东风";
	elseif task:GetTaskSubmitNpc(90104) == npcId then
		if Task_Submit_00090104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "只欠东风";
		elseif task:HasAcceptedTask(90104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "只欠东风";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "万事具备，只欠东风。";
	action.m_ActionMsg = "东风是什么？";
	return action;
end

function Task_00090104_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我在研制一种神药，可以增强将士们的战斗力。但是这药还缺最后一味材料，那便是玄幻坡的玄幻草，你帮我去取来吧。";
	action.m_ActionMsg = "行，你在这等我。";
	return action;
end

function Task_00090104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我终于研制出神药了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090104_step_table = {
		[1] = Task_00090104_step_01,
		[2] = Task_00090104_step_02,
		[10] = Task_00090104_step_10,
		};

function Task_00090104_step(step)
	if Task_00090104_step_table[step] ~= nil then
		return Task_00090104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15103, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(90104) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15103,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15103, itemNum, 1);
		end
		task:AddTaskStep2(90104, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00090104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15103,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90104) then
		return false;
	end

	package:DelItemAll(15103,1);

	player:AddExp(550000);
	player:getCoin(304000);
	return true;
end

--��������
function Task_00090104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15103,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90104);
end

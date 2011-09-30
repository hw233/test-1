--����Ľ�������
function Task_Accept_00000064()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(64) or task:HasCompletedTask(64) or task:HasSubmitedTask(64) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000064()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(64) or task:HasCompletedTask(64) or task:HasSubmitedTask(64) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000064()
	if GetPlayer():GetTaskMgr():HasCompletedTask(64) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000064(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(64) == npcId and Task_Accept_00000064 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 64
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "千里送药";
	elseif task:GetTaskSubmitNpc(64) == npcId then
		if Task_Submit_00000064() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 64
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "千里送药";
		elseif task:HasAcceptedTask(64) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 64
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "千里送药";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000064_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "方才髯仙李元华来这里找你，原来顽石大师被追云叟前辈他们护送到了长沙谷王峰，可是因为伤势恶化疼痛难忍，顽石大师几次欲兵解被大家劝住，"..GetPlayerName(GetPlayer()).."髯仙前辈叫你迅速赶去谷王峰用乌风草救治顽石大师。";
	action.m_ActionMsg = "事不宜迟，那我这就告辞了，申姑娘多保重。";
	return action;
end

function Task_00000064_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢小友千里送药，救命之恩没齿难忘。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000064_step_table = {
		[1] = Task_00000064_step_01,
		[10] = Task_00000064_step_10,
		};

function Task_00000064_step(step)
	if Task_00000064_step_table[step] ~= nil then
		return Task_00000064_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000064_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000064() then
		return false;
	end
	if not task:AcceptTask(64) then
		return false;
	end
	task:AddTaskStep(64);
	return true;
end



--�ύ����
function Task_00000064_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(64) then
		return false;
	end


	player:AddExp(10000);
	return true;
end

--��������
function Task_00000064_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(64);
end

--����Ľ�������
function Task_Accept_00000088()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(88) or task:HasCompletedTask(88) or task:HasSubmitedTask(88) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(87) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(87) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(87) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000088()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(88) or task:HasCompletedTask(88) or task:HasSubmitedTask(88) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(87) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(87) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(87) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000088()
	if GetPlayer():GetTaskMgr():HasCompletedTask(88) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000088(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(88) == npcId and Task_Accept_00000088 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 88
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "西方野魔";
	elseif task:GetTaskSubmitNpc(88) == npcId then
		if Task_Submit_00000088() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 88
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "西方野魔";
		elseif task:HasAcceptedTask(88) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 88
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "西方野魔";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000088_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "什么人鬼鬼祟祟躲在一边，以为我半身不遂就真变瞎变聋了吗！";
	action.m_ActionMsg = "发生了什么事？";
	return action;
end

function Task_00000088_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老身一人在此参枯禅已久，些许风吹草动都逃不过我的耳朵，我发现有个妖僧在一边偷窥，想来是觊觎这个雪魂珠，这种天材地宝可不能落进妖人手里。老身行动不便就麻烦小友你出手将他除去了。";
	action.m_ActionMsg = "除魔卫道是我等义不容辞的责任。";
	return action;
end

function Task_00000088_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这个妖僧竟然是西方野魔雅各达，亏得小友将他赶跑。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000088_step_table = {
		[1] = Task_00000088_step_01,
		[2] = Task_00000088_step_02,
		[10] = Task_00000088_step_10,
		};

function Task_00000088_step(step)
	if Task_00000088_step_table[step] ~= nil then
		return Task_00000088_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000088_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000088() then
		return false;
	end
	if not task:AcceptTask(88) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000088_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(88) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--��������
function Task_00000088_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(88);
end

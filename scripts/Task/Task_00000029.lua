--����Ľ�������
function Task_Accept_00000029()
	local player = GetPlayer();
	if player:GetLev() < 24 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(29) or task:HasCompletedTask(29) or task:HasSubmitedTask(29) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000029()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 24 then
		return false;
	end
	if task:HasAcceptedTask(29) or task:HasCompletedTask(29) or task:HasSubmitedTask(29) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000029()
	if GetPlayer():GetTaskMgr():HasCompletedTask(29) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000029(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(29) == npcId and Task_Accept_00000029 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 29
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "炼制材料";
	elseif task:GetTaskSubmitNpc(29) == npcId then
		if Task_Submit_00000029() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 29
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "炼制材料";
		elseif task:HasAcceptedTask(29) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 29
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "炼制材料";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000029_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "慈云寺妖人众多，其中颇有些厉害的角色，无论剑术还是妖法都非常犀利，我现在正在炼制一件克制敌人妖法的法宝，正在紧要关头却发现有些材料数量不足，不知道<name>你可否帮我寻一些来？";
	action.m_ActionMsg = "需要哪些材料呢？";
	return action;
end

function Task_00000029_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这些材料却也不难取，此处往北30里有一个乱葬岗，因为战乱连连，怨气聚集所以生出了不少的僵尸作怪，他们身上的玄阴之魄就是缺少的材料，<name>请速去速回，很快我就用着这些材料了。";
	action.m_ActionMsg = "我这就去乱葬岗。";
	return action;
end

function Task_00000029_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠你赶回来的真是及时，玉清多谢少侠援手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000029_step_table = {
		[1] = Task_00000029_step_01,
		[2] = Task_00000029_step_02,
		[10] = Task_00000029_step_10,
		};

function Task_00000029_step(step)
	if Task_00000029_step_table[step] ~= nil then
		return Task_00000029_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000029_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000029() then
		return false;
	end
	if not task:AcceptTask(29) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000029_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(29) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--��������
function Task_00000029_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(29);
end

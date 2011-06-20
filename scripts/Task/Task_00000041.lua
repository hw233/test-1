--����Ľ�������
function Task_Accept_00000041()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(41) or task:HasCompletedTask(41) or task:HasSubmitedTask(41) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000041()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(41) or task:HasCompletedTask(41) or task:HasSubmitedTask(41) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(40) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000041()
	if GetPlayer():GetTaskMgr():HasCompletedTask(41) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000041(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(41) == npcId and Task_Accept_00000041 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 41
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "强化法宝";
	elseif task:GetTaskSubmitNpc(41) == npcId then
		if Task_Submit_00000041() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 41
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "强化法宝";
		elseif task:HasAcceptedTask(41) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 41
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "强化法宝";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000041_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "想不到醉道人这个整天泡在酒葫芦里的家伙，竟然老眼不花，找到象你资质这么高的年青人……你的来意我已知晓，今天吗就免费教你强化法宝，要知道我收费可是很贵的哦。";
	action.m_ActionMsg = "多谢廖前辈了。";
	return action;
end

function Task_00000041_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我就知道这家伙最喜爱那些仙根福厚，资质不凡的年青人了，省下这点银子又够我喝几天酒了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000041_step_table = {
		[1] = Task_00000041_step_01,
		[10] = Task_00000041_step_10,
		};

function Task_00000041_step(step)
	if Task_00000041_step_table[step] ~= nil then
		return Task_00000041_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000041_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000041() then
		return false;
	end
	if not task:AcceptTask(41) then
		return false;
	end
	task:AddTaskStep(41);
	return true;
end



--�ύ����
function Task_00000041_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(41) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--��������
function Task_00000041_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(41);
end

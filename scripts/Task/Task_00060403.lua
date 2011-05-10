--����Ľ�������
function Task_Accept_00060403()
	local player = GetPlayer();
	if player:GetLev() < 36 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60403) or task:HasCompletedTask(60403) or task:HasSubmitedTask(60403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60402) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 36 then
		return false;
	end
	if task:HasAcceptedTask(60403) or task:HasCompletedTask(60403) or task:HasSubmitedTask(60403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60402) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60403) == npcId and Task_Accept_00060403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "赶尸慰魂";
	elseif task:GetTaskSubmitNpc(60403) == npcId then
		if Task_Submit_00060403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "赶尸慰魂";
		elseif task:HasAcceptedTask(60403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "赶尸慰魂";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你看这些黑风残留，正是那些为天魔之战的牺牲者的灵魂残留，这些魔兽正是靠这些灵魂残留的支撑才活的下来。可是他们这样，却让那些亡魂无法安心的归去，而这一切都是战争非常那些怨灵鬼魂的计谋。";
	action.m_ActionMsg = "原来是这样。";
	return action;
end

function Task_00060403_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "所有我希望你能去清除那些怨灵鬼魂，这样才有办法抚慰那些战争的亡魂";
	action.m_ActionMsg = "好，我这就去。";
	return action;
end

function Task_00060403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这下好了，战争的亡魂们虽然不能跟你对话，但是他们会在地下感谢你的帮助的，勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060403_step_table = {
		[1] = Task_00060403_step_01,
		[2] = Task_00060403_step_02,
		[10] = Task_00060403_step_10,
		};

function Task_00060403_step(step)
	if Task_00060403_step_table[step] ~= nil then
		return Task_00060403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60403) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00060403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60403) then
		return false;
	end


	player:AddExp(5500);
	player:getCoin(8000);
	return true;
end

--��������
function Task_00060403_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60403);
end

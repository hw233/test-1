--����Ľ�������
function Task_Accept_00061102()
	local player = GetPlayer();
	if player:GetLev() < 48 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61102) or task:HasCompletedTask(61102) or task:HasSubmitedTask(61102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61101) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 48 then
		return false;
	end
	if task:HasAcceptedTask(61102) or task:HasCompletedTask(61102) or task:HasSubmitedTask(61102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61101) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61102) == npcId and Task_Accept_00061102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "教训小偷";
	elseif task:GetTaskSubmitNpc(61102) == npcId then
		if Task_Submit_00061102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "教训小偷";
		elseif task:HasAcceptedTask(61102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "教训小偷";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这些渔人小偷实在太过分了，我路之遥就这么一个贴身的重要工具，我做饭练武全靠这把菜刀，居然就给我偷走了，我一定要给他们的颜色。";
	action.m_ActionMsg = "你自己也该小心点的，不过小偷的确是很让人厌恶的。";
	return action;
end

function Task_00061102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，那就麻烦你和我一起再去教训一次那些小偷吧，看他们还敢偷我路之遥的菜刀。";
	action.m_ActionMsg = "那我就陪你去好了。";
	return action;
end

function Task_00061102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "教训了这些小偷，我心情终于好一点了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061102_step_table = {
		[1] = Task_00061102_step_01,
		[2] = Task_00061102_step_02,
		[10] = Task_00061102_step_10,
		};

function Task_00061102_step(step)
	if Task_00061102_step_table[step] ~= nil then
		return Task_00061102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61102) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61102) then
		return false;
	end


	player:AddExp(48000);
	player:getCoin(34000);
	return true;
end

--��������
function Task_00061102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61102);
end

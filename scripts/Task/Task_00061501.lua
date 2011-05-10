--����Ľ�������
function Task_Accept_00061501()
	local player = GetPlayer();
	if player:GetLev() < 58 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61501) or task:HasCompletedTask(61501) or task:HasSubmitedTask(61501) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 58 then
		return false;
	end
	if task:HasAcceptedTask(61501) or task:HasCompletedTask(61501) or task:HasSubmitedTask(61501) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61501) == npcId and Task_Accept_00061501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "蒙不凡的请求";
	elseif task:GetTaskSubmitNpc(61501) == npcId then
		if Task_Submit_00061501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "蒙不凡的请求";
		elseif task:HasAcceptedTask(61501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61501
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "蒙不凡的请求";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我经常听说你强大的战斗实力，在山海大陆横扫许多地方，到处帮人清除各种怪物，恢复当地的平静，我所在的冰凌城正在被一些鬼族和恶龙侵蚀这片土地。";
	action.m_ActionMsg = "看来又是一个需要我出手的地方啊。";
	return action;
end

function Task_00061501_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "正是这样，尤其是冰裂谷的那些骷髅恶龙，经常到处飞，已经威胁到我的安全了，我又不是一个能够上场打仗的人，我需要你帮忙清除这些威胁，才能更好的行使我作为大使的使命。";
	action.m_ActionMsg = "没问题。";
	return action;
end

function Task_00061501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你可回来了，你差点就见不到我了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061501_step_table = {
		[1] = Task_00061501_step_01,
		[2] = Task_00061501_step_02,
		[10] = Task_00061501_step_10,
		};

function Task_00061501_step(step)
	if Task_00061501_step_table[step] ~= nil then
		return Task_00061501_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61501) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61501) then
		return false;
	end


	player:AddExp(88000);
	player:getCoin(90000);
	return true;
end

--��������
function Task_00061501_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61501);
end

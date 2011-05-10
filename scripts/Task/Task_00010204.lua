--����Ľ�������
function Task_Accept_00010204()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10204) or task:HasCompletedTask(10204) or task:HasSubmitedTask(10204) then
		return false;
	end
	if not task:HasSubmitedTask(10203) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(10204) or task:HasCompletedTask(10204) or task:HasSubmitedTask(10204) then
		return false;
	end
	if not task:HasSubmitedTask(10203) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10204) == npcId and Task_Accept_00010204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "拜见婆修";
	elseif task:GetTaskSubmitNpc(10204) == npcId then
		if Task_Submit_00010204() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10204
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "拜见婆修";
		elseif task:HasAcceptedTask(10204) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10204
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "拜见婆修";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "现在我看你也成长了不少了，看起来勇敢而强大，我为你感到高兴，为天族有你这样的勇士感到骄傲。";
	action.m_ActionMsg = "我以后还会成长的更加厉害的。";
	return action;
end

function Task_00010204_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你先去西边牛霍洲的云岩寺找婆修吧，他似乎正需要人手，他可是梵天最得意的弟子，会给予你很多的指导和帮助。";
	action.m_ActionMsg = "哦好的，看来又有需要我的地方了。";
	return action;
end

function Task_00010204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是我们的国师派你来的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010204_step_table = {
		[1] = Task_00010204_step_01,
		[2] = Task_00010204_step_02,
		[10] = Task_00010204_step_10,
		};

function Task_00010204_step(step)
	if Task_00010204_step_table[step] ~= nil then
		return Task_00010204_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010204() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10204) then
		return false;
	end
	task:AddTaskStep(10204);
	return true;
end



--�ύ����
function Task_00010204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10204) then
		return false;
	end


	player:AddExp(1400);
	player:getCoin(1300);
	player:getTael(10);
	return true;
end

--��������
function Task_00010204_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10204);
end

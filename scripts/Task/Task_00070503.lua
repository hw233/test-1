--����Ľ�������
function Task_Accept_00070503()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70503) or task:HasCompletedTask(70503) or task:HasSubmitedTask(70503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70315) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70315) then
			return false;
		end
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70503) or task:HasCompletedTask(70503) or task:HasSubmitedTask(70503) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1004) >= 4 then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70315) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70315) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00070503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70503) == npcId and Task_Accept_00070503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "老板娘叫你回家吃饭";
	elseif task:GetTaskSubmitNpc(70503) == npcId then
		if Task_Submit_00070503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "老板娘叫你回家吃饭";
		elseif task:HasAcceptedTask(70503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "老板娘叫你回家吃饭";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，最近我的旅店似乎遭到了僵尸刺客的偷袭，为了保护我店内的客人的安全，也为了我自己的生意不受影响，我想雇你作为我们旅店的临时保镖，只要你能去杀掉黑森林虚无林的那些僵尸刺客，我会付给你报酬，完成以后回来找我，我是本着诚信的原则作生意的。";
	action.m_ActionMsg = "还能赚外快，真是太好了，生活总是美好的，哈哈。";
	return action;
end

function Task_00070503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，如果你愿意我希望你每天都能来帮我维持我们旅店的安全。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070503_step_table = {
		[1] = Task_00070503_step_01,
		[10] = Task_00070503_step_10,
		};

function Task_00070503_step(step)
	if Task_00070503_step_table[step] ~= nil then
		return Task_00070503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70503) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(70503) then
		return false;
	end


	player:getTael(50);
	return true;
end

--��������
function Task_00070503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70503);
end

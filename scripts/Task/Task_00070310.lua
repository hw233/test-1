--����Ľ�������
function Task_Accept_00070310()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70310) or task:HasCompletedTask(70310) or task:HasSubmitedTask(70310) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070310()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(70310) or task:HasCompletedTask(70310) or task:HasSubmitedTask(70310) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70310 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070310()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70310) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070310(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70310) == npcId and Task_Accept_00070310 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70310
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "前世无知后世可修";
	elseif task:GetTaskSubmitNpc(70310) == npcId then
		if Task_Submit_00070310() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70310
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "前世无知后世可修";
		elseif task:HasAcceptedTask(70310) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70310
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "前世无知后世可修";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070310_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，每当提起那些岩岛毒蛇，我自己都胆战心惊的，不知道是不是因为我前世无知啊，人们都跟我说蛇是密语之王，他们掌握了最古老最神秘的预言，只有通灵的人才能听懂他们的预言，可是我却从心底对那种怪物感到毛骨悚然，就算我无知，也让我下辈子再修吧，求你去杀掉他们吧。";
	action.m_ActionMsg = "那些怪物，我看他们是想修成人形，我可不会成全他们。";
	return action;
end

function Task_00070310_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "任他有知无知兮，我只是再也不想看到那些毒蛇了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070310_step_table = {
		[1] = Task_00070310_step_01,
		[10] = Task_00070310_step_10,
		};

function Task_00070310_step(step)
	if Task_00070310_step_table[step] ~= nil then
		return Task_00070310_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070310_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070310() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70310) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070310_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70310) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70310);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070310_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70310);
end

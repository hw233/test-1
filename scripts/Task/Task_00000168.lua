--����Ľ�������
function Task_Accept_00000168()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(168) or task:HasCompletedTask(168) or task:HasSubmitedTask(168) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(167) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(167) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(167) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000168()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(168) or task:HasCompletedTask(168) or task:HasSubmitedTask(168) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(167) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(167) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(167) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000168()
	if GetPlayer():GetTaskMgr():HasCompletedTask(168) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000168(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(168) == npcId and Task_Accept_00000168 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 168
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "冰雪结晶";
	elseif task:GetTaskSubmitNpc(168) == npcId then
		if Task_Submit_00000168() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 168
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "冰雪结晶";
		elseif task:HasAcceptedTask(168) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 168
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "冰雪结晶";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000168_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这里天寒地冻本是一片安静的所在，只是百年前易周为那只老鳖搞出一块元磁禁制，引得周围很多生灵变异，尤其是周边冰原上凝结出一些冰雪结晶，经常伤我手下弟子，而我清心潜修已不问这些琐事，如果小友有暇去帮帮我这些不成器的弟子也好。";
	action.m_ActionMsg = "不敢不敢，弟子我这就去清除这些冰雪异类。";
	return action;
end

function Task_00000168_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哈哈，早就听说小友你在正邪之战中大放异彩。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000168_step_table = {
		[1] = Task_00000168_step_01,
		[10] = Task_00000168_step_10,
		};

function Task_00000168_step(step)
	if Task_00000168_step_table[step] ~= nil then
		return Task_00000168_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000168_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000168() then
		return false;
	end
	if not task:AcceptTask(168) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000168_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(168) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000168_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(168);
end

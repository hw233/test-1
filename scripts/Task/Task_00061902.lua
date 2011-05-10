--����Ľ�������
function Task_Accept_00061902()
	local player = GetPlayer();
	if player:GetLev() < 68 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61902) or task:HasCompletedTask(61902) or task:HasSubmitedTask(61902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61901) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061902()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 68 then
		return false;
	end
	if task:HasAcceptedTask(61902) or task:HasCompletedTask(61902) or task:HasSubmitedTask(61902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61901) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061902()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61902) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061902(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61902) == npcId and Task_Accept_00061902 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61902
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "假象的原因";
	elseif task:GetTaskSubmitNpc(61902) == npcId then
		if Task_Submit_00061902() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61902
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "假象的原因";
		elseif task:HasAcceptedTask(61902) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61902
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "假象的原因";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061902_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这个无影的预言真是越来越玄乎了，怎么就世界末日了，最多也就发生点战争什么的，他所谓的星象不稳，其实我早就发现了，他却不追求现象的原因，只追求现象的结果。";
	action.m_ActionMsg = "大师这么说，是没有世界末日这样的事情了？";
	return action;
end

function Task_00061902_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是没有的，无影说的那个现象，是因为他那边的浦夷林里出现了许多原始雷龙，这怪物具有强大的光辐射，以至于对普通人观看到的星象都是受到扰乱了的，你可以尝试去杀掉那些雷龙再看看情况的变化。";
	action.m_ActionMsg = "那我不妨去试试。";
	return action;
end

function Task_00061902_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士在杀那些雷龙的时候你可以感觉到他们强大的辐射吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061902_step_table = {
		[1] = Task_00061902_step_01,
		[2] = Task_00061902_step_02,
		[10] = Task_00061902_step_10,
		};

function Task_00061902_step(step)
	if Task_00061902_step_table[step] ~= nil then
		return Task_00061902_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061902_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061902() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61902) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061902_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61902) then
		return false;
	end


	player:AddExp(210000);
	player:getCoin(190000);
	return true;
end

--��������
function Task_00061902_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61902);
end

--����Ľ�������
function Task_Accept_00000512()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(512) or task:HasCompletedTask(512) or task:HasSubmitedTask(512) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000512()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(512) or task:HasCompletedTask(512) or task:HasSubmitedTask(512) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000512()
	if GetPlayer():GetTaskMgr():HasCompletedTask(512) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000512(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(512) == npcId and Task_Accept_00000512 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 512
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "隐居剑侠";
	elseif task:GetTaskSubmitNpc(512) == npcId then
		if Task_Submit_00000512() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 512
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "隐居剑侠";
		elseif task:HasAcceptedTask(512) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 512
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "隐居剑侠";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000512_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在成都有位隐居多年的剑侠，叫沈浪，昔年为师曾和他一起出门历练，降魔除妖无数，只是汉室颠覆，又恪于修道之人的戒律，不能对凡人出手，所以他心灰意冷隐居成都，你去拜访一下他吧。";
	action.m_ActionMsg = "弟子这就去。";
	return action;
end

function Task_00000512_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "数十年没见了，想不到齐掌教还记得我。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000512_step_table = {
		[1] = Task_00000512_step_01,
		[10] = Task_00000512_step_10,
		};

function Task_00000512_step(step)
	if Task_00000512_step_table[step] ~= nil then
		return Task_00000512_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000512_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000512() then
		return false;
	end
	if not task:AcceptTask(512) then
		return false;
	end
	task:AddTaskStep(512);
	return true;
end



--�ύ����
function Task_00000512_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(512) then
		return false;
	end


	player:AddExp(8000);
	return true;
end

--��������
function Task_00000512_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(512);
end

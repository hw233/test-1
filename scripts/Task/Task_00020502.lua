--����Ľ�������
function Task_Accept_00020502()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 26 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20502) or task:HasCompletedTask(20502) or task:HasSubmitedTask(20502) then
		return false;
	end
	if not task:HasSubmitedTask(20501) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 26 then
		return false;
	end
	if task:HasAcceptedTask(20502) or task:HasCompletedTask(20502) or task:HasSubmitedTask(20502) then
		return false;
	end
	if not task:HasSubmitedTask(20501) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20502) == npcId and Task_Accept_00020502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "情况汇报";
	elseif task:GetTaskSubmitNpc(20502) == npcId then
		if Task_Submit_00020502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "情况汇报";
		elseif task:HasAcceptedTask(20502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "情况汇报";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这些邪恶的夜摩盟，已经穿过我们跟山海大陆之间的连接处，来到我们这里了，他们的阴谋越来越明显了。";
	action.m_ActionMsg = "他们想得到什么呢？";
	return action;
end

function Task_00020502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成立夜摩盟的夜叉王和摩可拿，都曾经被天族和魔族驱逐了出去，可是他们内心邪恶，不肯罢休，贪念促使他们在山海大陆上结成夜摩盟，继续他们的邪恶计划，并一直觊觎我们魔族在地界的影响。我们不能任由他们的阴谋得逞，现在你去找旁边的摧伏大人吧，他会帮助你。\n";
	action.m_ActionMsg = "还是不相信我的能力啊。";
	return action;
end

function Task_00020502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "又是可恶的夜摩盟。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020502_step_table = {
		[1] = Task_00020502_step_01,
		[2] = Task_00020502_step_02,
		[10] = Task_00020502_step_10,
		};

function Task_00020502_step(step)
	if Task_00020502_step_table[step] ~= nil then
		return Task_00020502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020502() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20502) then
		return false;
	end
	task:AddTaskStep(20502);
	return true;
end



--�ύ����
function Task_00020502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20502) then
		return false;
	end


	player:AddExp(3200);
	player:getCoin(4700);
	return true;
end

--��������
function Task_00020502_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20502);
end

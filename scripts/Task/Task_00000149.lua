--����Ľ�������
function Task_Accept_00000149()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(149) or task:HasCompletedTask(149) or task:HasSubmitedTask(149) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000149()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(149) or task:HasCompletedTask(149) or task:HasSubmitedTask(149) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000149()
	if GetPlayer():GetTaskMgr():HasCompletedTask(149) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000149(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(149) == npcId and Task_Accept_00000149 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 149
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "雁荡之劫";
	elseif task:GetTaskSubmitNpc(149) == npcId then
		if Task_Submit_00000149() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 149
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "雁荡之劫";
		elseif task:HasAcceptedTask(149) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 149
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "雁荡之劫";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000149_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "优昙神尼的高徒齐霞儿如今在雁荡山一带剿除妖鲧，这妖鲧也是蛟类的一种，修炼多年眼看可以脱去妖身化为神龙，可是这孽畜竟然罔顾众生，携洪水而出必将导致生灵涂炭。我恐齐霞儿势单力孤，此去雁荡不远，不如你去助齐霞儿一臂之力。";
	action.m_ActionMsg = "师太，我这就去雁荡山与齐霞儿回合。";
	return action;
end

function Task_00000149_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，这只妖龙修炼千年，修为毁于一旦，真是可惜。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000149_step_table = {
		[1] = Task_00000149_step_01,
		[10] = Task_00000149_step_10,
		};

function Task_00000149_step(step)
	if Task_00000149_step_table[step] ~= nil then
		return Task_00000149_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000149_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000149() then
		return false;
	end
	if not task:AcceptTask(149) then
		return false;
	end
	task:AddTaskStep(149);
	return true;
end



--�ύ����
function Task_00000149_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(149) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--��������
function Task_00000149_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(149);
end

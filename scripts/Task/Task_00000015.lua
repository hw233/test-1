--����Ľ�������
function Task_Accept_00000015()
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(15) or task:HasCompletedTask(15) or task:HasSubmitedTask(15) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(14) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(14) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000015()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(15) or task:HasCompletedTask(15) or task:HasSubmitedTask(15) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(14) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(14) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000015()
	if GetPlayer():GetTaskMgr():HasCompletedTask(15) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000015(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(15) == npcId and Task_Accept_00000015 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 15
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "白云寺";
	elseif task:GetTaskSubmitNpc(15) == npcId then
		if Task_Submit_00000015() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 15
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "白云寺";
		elseif task:HasAcceptedTask(15) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 15
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "白云寺";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000015_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "说起毛太，其实他一人实不足为虑，只是慈云寺众多妖人中的一员，那里妖人聚集，真是乌烟瘴气，不过里面确实有一些厉害的角色，连我也很是头疼。 ";
	action.m_ActionMsg = "那该如何是好？";
	return action;
end

function Task_00000015_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "慈云寺里虽然邪派人物众多，不过我们正道中人也在聚集准备出手剿灭慈云寺妖孽。成都郊外有一座白云寺，主持白云大师道法高深，你先去大师那里等候我的佳音。";
	action.m_ActionMsg = "好的，前辈你也要小心。";
	return action;
end

function Task_00000015_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠根骨资质俱佳，未来成就不可限量。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000015_step_table = {
		[1] = Task_00000015_step_01,
		[2] = Task_00000015_step_02,
		[10] = Task_00000015_step_10,
		};

function Task_00000015_step(step)
	if Task_00000015_step_table[step] ~= nil then
		return Task_00000015_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000015_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000015() then
		return false;
	end
	if not task:AcceptTask(15) then
		return false;
	end
	task:AddTaskStep(15);
	return true;
end



--�ύ����
function Task_00000015_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(15) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000015_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(15);
end

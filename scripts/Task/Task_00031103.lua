--����Ľ�������
function Task_Accept_00031103()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 57 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31103) or task:HasCompletedTask(31103) or task:HasSubmitedTask(31103) then
		return false;
	end
	if not task:HasSubmitedTask(31102) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 57 then
		return false;
	end
	if task:HasAcceptedTask(31103) or task:HasCompletedTask(31103) or task:HasSubmitedTask(31103) then
		return false;
	end
	if not task:HasSubmitedTask(31102) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31103) == npcId and Task_Accept_00031103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "魔族部队的挑衅";
	elseif task:GetTaskSubmitNpc(31103) == npcId then
		if Task_Submit_00031103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "魔族部队的挑衅";
		elseif task:HasAcceptedTask(31103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "魔族部队的挑衅";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你这无耻的天族小子，凭什么找到我们头上来了？也不问问，是你们自己不照顾好我们魔族的如烟，既然你们照顾不好，当然要由我们来照顾了，如烟就是被我们魔族的人护送走的。";
	action.m_ActionMsg = "如烟已经嫁到天族来了，凭什么你们要来插足？";
	return action;
end

function Task_00031103_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如烟始终都是我们魔族的人，只要有我们在你们就别想再欺负她了，现在的状况都是你们自己的错误造成的，你们自己要为此付出代价，如果你们一定坚持不服气的话，就是自找苦吃";
	action.m_ActionMsg = "不知道天高地厚，别以为我会怕你。";
	return action;
end

function Task_00031103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你又跟那些狂暴无礼的魔族的人打起来了？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031103_step_table = {
		[1] = Task_00031103_step_01,
		[2] = Task_00031103_step_02,
		[10] = Task_00031103_step_10,
		};

function Task_00031103_step(step)
	if Task_00031103_step_table[step] ~= nil then
		return Task_00031103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31103) then
		return false;
	end
	RunAutoBattleAction(player,4258,100);
	task:AddTaskStep(31103);

	return true;
end



--�ύ����
function Task_00031103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31103) then
		return false;
	end


	player:AddExp(84000);
	player:getCoin(84000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031103_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31103);
end

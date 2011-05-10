--����Ľ�������
function Task_Accept_00031003()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 57 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31003) or task:HasCompletedTask(31003) or task:HasSubmitedTask(31003) then
		return false;
	end
	if not task:HasSubmitedTask(31002) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031003()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 57 then
		return false;
	end
	if task:HasAcceptedTask(31003) or task:HasCompletedTask(31003) or task:HasSubmitedTask(31003) then
		return false;
	end
	if not task:HasSubmitedTask(31002) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031003()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31003) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031003(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31003) == npcId and Task_Accept_00031003 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31003
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "天族护卫的挑衅";
	elseif task:GetTaskSubmitNpc(31003) == npcId then
		if Task_Submit_00031003() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31003
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "天族护卫的挑衅";
		elseif task:HasAcceptedTask(31003) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31003
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "天族护卫的挑衅";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031003_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你这无聊的魔族小子，居然将我们的如烟夫人私自送走，也太不把我们放在眼里了。";
	action.m_ActionMsg = "如烟流落孤寺多日无人救助，你们不去救她的，还无耻的说我。";
	return action;
end

function Task_00031003_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你这小子真是不知天高地厚，等你吃到苦头，就知道这一切都是你自找的。";
	action.m_ActionMsg = "别以为我会怕你。";
	return action;
end

function Task_00031003_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你又跟那些狂妄自大的天族的人打起来了？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031003_step_table = {
		[1] = Task_00031003_step_01,
		[2] = Task_00031003_step_02,
		[10] = Task_00031003_step_10,
		};

function Task_00031003_step(step)
	if Task_00031003_step_table[step] ~= nil then
		return Task_00031003_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031003_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031003() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31003) then
		return false;
	end
	RunAutoBattleAction(player,4252,100);
	task:AddTaskStep(31003);

	return true;
end



--�ύ����
function Task_00031003_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31003) then
		return false;
	end


	player:AddExp(84000);
	player:getCoin(84000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031003_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31003);
end

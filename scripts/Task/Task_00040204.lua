--����Ľ�������
function Task_Accept_00040204()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 20 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40204) or task:HasCompletedTask(40204) or task:HasSubmitedTask(40204) then
		return false;
	end
	if not task:HasSubmitedTask(40203) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 20 then
		return false;
	end
	if task:HasAcceptedTask(40204) or task:HasCompletedTask(40204) or task:HasSubmitedTask(40204) then
		return false;
	end
	if not task:HasSubmitedTask(40203) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40204) == npcId and Task_Accept_00040204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "招募苏轼";
	elseif task:HasAcceptedTask(40204) and npcId == 136 then
		RunAutoBattleAction(player, 136, -1);
	elseif task:GetTaskSubmitNpc(40204) == npcId and Task_Submit_00040204() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40204
		action.m_ActionToken = 2;
		action.m_ActionStep = 10;
		action.m_ActionMsg = "招募苏轼";	
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "替我谢谢那天，他希望我还能继续为天族效力。勇士，如果你能击败我，我将成为你的将领，随你一起征战丑陋的魔族。";
	action.m_ActionMsg = "我会用实力向你证明，我是一名真正的勇士。";
	return action;
end

function Task_00040204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "今后请阁下多多指点了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040204_step_table = {
		[1] = Task_00040204_step_01,
		[10] = Task_00040204_step_10,
		};

function Task_00040204_step(step)
	if Task_00040204_step_table[step] ~= nil then
		return Task_00040204_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040204() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40204) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if player:isFighterFull() then
		player:sendMsgCode(0, 2006);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40204) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	player:takeFighter(136, true);
	return true;
end

--��������
function Task_00040204_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(40204);
end

--����Ľ�������
function Task_Accept_00040106()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40106) or task:HasCompletedTask(40106) or task:HasSubmitedTask(40106) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(40106) or task:HasCompletedTask(40106) or task:HasSubmitedTask(40106) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40106) == npcId and Task_Accept_00040106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "招募貂蝉";
	elseif task:HasAcceptedTask(40106) and npcId == 134 then
		RunAutoBattleAction(player, 134, -1);
	elseif task:GetTaskSubmitNpc(40106) == npcId and Task_Submit_00040106() then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40106
		action.m_ActionToken = 2;
		action.m_ActionStep = 10;
		action.m_ActionMsg = "招募貂蝉";	
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "修炼之路将会遇到各种艰难险阻，只凭一己之力是不够的，天族有很多隐居的名将，貂蝉是其中一个。你需要去打败并收服她，让她成为你的士兵，同你并肩作战。";
	action.m_ActionMsg = "这有什么难的，我会让她诚服于我的。";
	return action;
end

function Task_00040106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "妾身愿跟随你征战到天涯海角。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040106_step_table = {
		[1] = Task_00040106_step_01,
		[10] = Task_00040106_step_10,
		};

function Task_00040106_step(step)
	if Task_00040106_step_table[step] ~= nil then
		return Task_00040106_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040106() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40106) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040106_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if player:isFighterFull() then
		player:sendMsgCode(0, 2006);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40106) then
		return false;
	end


	player:AddExp(1000);
	player:getCoin(10000);
	player:getTael(60);
	player:takeFighter(134, true);
	return true;
end

--��������
function Task_00040106_abandon()
	return GetPlayer():GetTaskMgr():AbandonTask(40106);
end

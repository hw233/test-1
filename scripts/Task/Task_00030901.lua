--����Ľ�������
function Task_Accept_00030901()
	local player = GetPlayer();
	if player:GetLev() < 54 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30901) or task:HasCompletedTask(30901) or task:HasSubmitedTask(30901) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30806) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30806) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030901()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 54 then
		return false;
	end
	if task:HasAcceptedTask(30901) or task:HasCompletedTask(30901) or task:HasSubmitedTask(30901) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30806) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30806) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030901()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30901) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030901(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30901) == npcId and Task_Accept_00030901 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30901
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "探索冰封窟";
	elseif task:GetTaskSubmitNpc(30901) == npcId then
		if Task_Submit_00030901() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30901
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "探索冰封窟";
		elseif task:HasAcceptedTask(30901) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30901
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "探索冰封窟";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030901_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好年轻的勇士，很高兴在这冰冷的雪山之城见到了东山派的同盟勇士，最近形势越来越严峻了，你愿意顶着严寒为我们探查情况吗。";
	action.m_ActionMsg = "寒冷算什么，没问题的。";
	return action;
end

function Task_00030901_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "城郊的冰封窟，据说最近出现了夜叉王和摩可拿的踪迹，希望你能帮我去探查确认一下，不过要小心，在完全查清楚之前，贸然动手是危险的。";
	action.m_ActionMsg = "我现在恨不得一刀宰了他们，但我深知我的力量是不够的.";
	return action;
end

function Task_00030901_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士我正等着你回来告诉我你的探索结果。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030901_step_table = {
		[1] = Task_00030901_step_01,
		[2] = Task_00030901_step_02,
		[10] = Task_00030901_step_10,
		};

function Task_00030901_step(step)
	if Task_00030901_step_table[step] ~= nil then
		return Task_00030901_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030901_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030901() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30901) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030901_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8902,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30901) then
		return false;
	end

	if IsEquipTypeId(8902) then
		for k = 1, 1 do
			package:AddEquip(8902, 1);
		end
	else 
		package:AddItem(8902,1,1);
	end

	player:AddExp(72000);
	player:getCoin(64000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030901_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30901);
end

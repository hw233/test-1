--����Ľ�������
function Task_Accept_00060802()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 46 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60802) or task:HasCompletedTask(60802) or task:HasSubmitedTask(60802) then
		return false;
	end
	if not task:HasSubmitedTask(60801) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060802()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 46 then
		return false;
	end
	if task:HasAcceptedTask(60802) or task:HasCompletedTask(60802) or task:HasSubmitedTask(60802) then
		return false;
	end
	if not task:HasSubmitedTask(60801) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060802()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60802) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060802(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60802) == npcId and Task_Accept_00060802 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60802
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "采集神药";
	elseif task:GetTaskSubmitNpc(60802) == npcId then
		if Task_Submit_00060802() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60802
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "采集神药";
		elseif task:HasAcceptedTask(60802) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60802
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "采集神药";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060802_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你带来这么多稀世草药，明显是有求于我啊，快说吧，到底是什么事情。";
	action.m_ActionMsg = "其实是天武城主，要我找您求一个治疗失语症的方子。";
	return action;
end

function Task_00060802_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是这样，礼数倒是挺足，看来我还不得不给你这个方子了，但是还是需要你自己去动手采集这种神药啊。在整个天庭，只有观音山的血染崖上出产一种叫做神麻草的神药，只要服下就可以治好这种疾病。";
	action.m_ActionMsg = "明白了，感谢您神指大师。";
	return action;
end

function Task_00060802_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "……";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060802_step_table = {
		[1] = Task_00060802_step_01,
		[2] = Task_00060802_step_02,
		[10] = Task_00060802_step_10,
		};

function Task_00060802_step(step)
	if Task_00060802_step_table[step] ~= nil then
		return Task_00060802_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060802_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060802() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60802) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15009,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15009, itemNum, 1);
		end
		task:AddTaskStep2(60802, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00060802_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15009,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60802) then
		return false;
	end

	package:DelItemAll(15009,1);

	player:AddExp(40000);
	player:getCoin(27000);
	return true;
end

--��������
function Task_00060802_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15009,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60802);
end

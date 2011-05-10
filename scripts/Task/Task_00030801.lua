--����Ľ�������
function Task_Accept_00030801()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30801) or task:HasCompletedTask(30801) or task:HasSubmitedTask(30801) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30707) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30707) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(30801) or task:HasCompletedTask(30801) or task:HasSubmitedTask(30801) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30707) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30707) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30801) == npcId and Task_Accept_00030801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "伏地龙骨";
	elseif task:GetTaskSubmitNpc(30801) == npcId then
		if Task_Submit_00030801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "伏地龙骨";
		elseif task:HasAcceptedTask(30801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "伏地龙骨";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "欢迎回来我的勇士，我已经听说了你最近的行为和事迹，的确让我刮目相看，消除了我一直以来对你的疑虑和不信任。不瞒你说，我们镇邪派能有今天，也是我们每个人的功劳，但是我很清楚，我们的势力还是很薄弱的，人数、资金，对我们来说压力都很大，现在急需你的帮忙。\n";
	action.m_ActionMsg = "帮主放心，我会尽我所能。";
	return action;
end

function Task_00030801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们最近遇到一位名将投靠进来，可惜窘迫的是我们连一件上好的装备都拿不出来送给他，希望你能帮我们收集点打造装备的材料，我们首先需要漂流谷的那些伏地龙的龙骨碎片。";
	action.m_ActionMsg = "我希望以后我们能以诚相见，我这就去取你要的东西。";
	return action;
end

function Task_00030801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是辛苦你了勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030801_step_table = {
		[1] = Task_00030801_step_01,
		[2] = Task_00030801_step_02,
		[10] = Task_00030801_step_10,
		};

function Task_00030801_step(step)
	if Task_00030801_step_table[step] ~= nil then
		return Task_00030801_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30801) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15059,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15059, itemNum, 1);
		end
		task:AddTaskStep2(30801, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00030801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15059,1) < 12 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30801) then
		return false;
	end

	package:DelItemAll(15059,1);

	player:AddExp(56000);
	player:getCoin(43500);
	player:getTael(20);
	return true;
end

--��������
function Task_00030801_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15059,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30801);
end

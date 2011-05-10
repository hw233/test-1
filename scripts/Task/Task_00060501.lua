--����Ľ�������
function Task_Accept_00060501()
	local player = GetPlayer();
	if player:GetLev() < 38 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60501) or task:HasCompletedTask(60501) or task:HasSubmitedTask(60501) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 38 then
		return false;
	end
	if task:HasAcceptedTask(60501) or task:HasCompletedTask(60501) or task:HasSubmitedTask(60501) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60501) == npcId and Task_Accept_00060501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "收集兽皮";
	elseif task:GetTaskSubmitNpc(60501) == npcId then
		if Task_Submit_00060501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "收集兽皮";
		elseif task:HasAcceptedTask(60501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60501
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "收集兽皮";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "有你这样的勇士做东山派的朋友，我从心底里感到荣幸，这也不由得让我想起一个人，如果不是他介绍你来找我，也许我们没那么快结为同盟。";
	action.m_ActionMsg = "帮主是在想什么呢？";
	return action;
end

function Task_00060501_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "先不说那么多了，我现在需要一些兽皮，能麻烦你去黑风岗那里，杀掉黑风兽，收集他们的兽皮来给我吗？";
	action.m_ActionMsg = "不是什么难事，我现在就去帮你。";
	return action;
end

function Task_00060501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是一些上等的兽皮，不错。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060501_step_table = {
		[1] = Task_00060501_step_01,
		[2] = Task_00060501_step_02,
		[10] = Task_00060501_step_10,
		};

function Task_00060501_step(step)
	if Task_00060501_step_table[step] ~= nil then
		return Task_00060501_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60501) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15006,1);
	if itemNum ~= 0 then
		if itemNum > 20 then
			itemNum = 20;
			package:SetItem(15006, itemNum, 1);
		end
		task:AddTaskStep2(60501, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00060501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15006,1) < 20 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60501) then
		return false;
	end

	package:DelItemAll(15006,1);

	player:AddExp(7000);
	player:getCoin(10000);
	return true;
end

--��������
function Task_00060501_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15006,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60501);
end

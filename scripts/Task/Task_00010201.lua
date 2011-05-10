--����Ľ�������
function Task_Accept_00010201()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10201) or task:HasCompletedTask(10201) or task:HasSubmitedTask(10201) then
		return false;
	end
	if not task:HasSubmitedTask(10107) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(10201) or task:HasCompletedTask(10201) or task:HasSubmitedTask(10201) then
		return false;
	end
	if not task:HasSubmitedTask(10107) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10201) == npcId and Task_Accept_00010201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "送甘露";
	elseif task:GetTaskSubmitNpc(10201) == npcId then
		if Task_Submit_00010201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "送甘露";
		elseif task:HasAcceptedTask(10201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "送甘露";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我本来正要找人帮我把这些甘露送给等在城外的魔族使节，就你去吧，我也不用找别人了。";
	action.m_ActionMsg = "额……甘露……可是什么是甘露？";
	return action;
end

function Task_00010201_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你不是天族的勇士么？怎么会不知道甘露是什么呢？也对，是新来没多久的吧，先别问了，帮我把这手头的事情做了，回头我会告诉你的。";
	action.m_ActionMsg = "那我先去，不过回来以后还请您给我解释一下。";
	return action;
end

function Task_00010201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦？这次送甘露来的换人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010201_step_table = {
		[1] = Task_00010201_step_01,
		[2] = Task_00010201_step_02,
		[10] = Task_00010201_step_10,
		};

function Task_00010201_step(step)
	if Task_00010201_step_table[step] ~= nil then
		return Task_00010201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010201() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15020, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10201) then
		return false;
	end
	task:AddTaskStep(10201);
	return true;
end



--�ύ����
function Task_00010201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15020,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(10201) then
		return false;
	end

	package:DelItemAll(15020,1);

	player:AddExp(1400);
	player:getCoin(1000);
	return true;
end

--��������
function Task_00010201_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15020,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10201);
end

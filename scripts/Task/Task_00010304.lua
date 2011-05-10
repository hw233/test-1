--����Ľ�������
function Task_Accept_00010304()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10304) or task:HasCompletedTask(10304) or task:HasSubmitedTask(10304) then
		return false;
	end
	if not task:HasSubmitedTask(10303) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10304) or task:HasCompletedTask(10304) or task:HasSubmitedTask(10304) then
		return false;
	end
	if not task:HasSubmitedTask(10303) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10304) == npcId and Task_Accept_00010304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "王妃的秘密";
	elseif task:GetTaskSubmitNpc(10304) == npcId then
		if Task_Submit_00010304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "王妃的秘密";
		elseif task:HasAcceptedTask(10304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "王妃的秘密";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "王妃让你替她送信，你却送给我来看？";
	action.m_ActionMsg = "是的，我自作主张看到了信的内容，所以觉得应该先报告您。";
	return action;
end

function Task_00010304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯干的漂亮，魔族的人都是这么不知好歹，当初的天魔之战，他们想开战就开战，想讲和就讲和，结果他们还是送来了悦意，我甚至还给他们甘露作为回报，没想到他们一而再再而三的来要甘露。";
	action.m_ActionMsg = "魔族还真是贪婪无度啊。";
	return action;
end

function Task_00010304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你这样做足以表明你对我的忠心了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010304_step_table = {
		[1] = Task_00010304_step_01,
		[2] = Task_00010304_step_02,
		[10] = Task_00010304_step_10,
		};

function Task_00010304_step(step)
	if Task_00010304_step_table[step] ~= nil then
		return Task_00010304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10304) then
		return false;
	end
	task:AddTaskStep(10304);
	return true;
end



--�ύ����
function Task_00010304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10304) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(2100);
	return true;
end

--��������
function Task_00010304_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10304);
end

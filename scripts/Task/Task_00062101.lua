--����Ľ�������
function Task_Accept_00062101()
	local player = GetPlayer();
	if player:GetLev() < 73 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62101) or task:HasCompletedTask(62101) or task:HasSubmitedTask(62101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 73 then
		return false;
	end
	if task:HasAcceptedTask(62101) or task:HasCompletedTask(62101) or task:HasSubmitedTask(62101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00062101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62101) == npcId and Task_Accept_00062101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "奇异佛珠";
	elseif task:GetTaskSubmitNpc(62101) == npcId then
		if Task_Submit_00062101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "奇异佛珠";
		elseif task:HasAcceptedTask(62101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "奇异佛珠";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你知道最近有什么神或者佛的人物吗？我最近在自己的杂物中发现了一串特别的佛珠，很明显这不是我的东西，而且这佛珠看起很古旧，应该是很久以前的东西了。";
	action.m_ActionMsg = "我没有听说过谁会有这样的佛珠。";
	return action;
end

function Task_00062101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那只好去找最有名的挖墓人问问了，在幻海城的晨钟镇，有一个著名的挖墓人，他发掘过各种各样的古老的物品，你去把这串佛珠带去给他看看吧，最好能物归原主。";
	action.m_ActionMsg = "行，我带去问问看。";
	return action;
end

function Task_00062101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这可不是一般的佛珠啊，这可是蕴含了法力的一串佛珠。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062101_step_table = {
		[1] = Task_00062101_step_01,
		[2] = Task_00062101_step_02,
		[10] = Task_00062101_step_10,
		};

function Task_00062101_step(step)
	if Task_00062101_step_table[step] ~= nil then
		return Task_00062101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062101() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15018, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(62101) then
		return false;
	end
	task:AddTaskStep(62101);
	return true;
end



--�ύ����
function Task_00062101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15018,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(62101) then
		return false;
	end

	package:DelItemAll(15018,1);

	player:AddExp(280000);
	player:getCoin(259000);
	return true;
end

--��������
function Task_00062101_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15018,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62101);
end

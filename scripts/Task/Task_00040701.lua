--����Ľ�������
function Task_Accept_00040701()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40701) or task:HasCompletedTask(40701) or task:HasSubmitedTask(40701) then
		return false;
	end
	if not task:HasSubmitedTask(40603) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(40701) or task:HasCompletedTask(40701) or task:HasSubmitedTask(40701) then
		return false;
	end
	if not task:HasSubmitedTask(40603) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40701) == npcId and Task_Accept_00040701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "明察秋毫";
	elseif task:GetTaskSubmitNpc(40701) == npcId then
		if Task_Submit_00040701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "明察秋毫";
		elseif task:HasAcceptedTask(40701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "明察秋毫";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "太好了勇士，你带来的这束山茶，可真是稀世珍品啊，伍子胥还真够意思，找到这样的好东西马上就能想到我。";
	action.m_ActionMsg = "你果然很喜欢这山茶。";
	return action;
end

function Task_00040701_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是啊 ，我本来正打算去陈田沼泽查看下那里的情况的，现在我要忙着煸炒我的山茶，很快我就能享受上这清香的茶叶了，就拜托你去帮我一下吧，我一定不会让你白去的。";
	action.m_ActionMsg = "好吧，我很好奇一会你会让我得到什么。";
	return action;
end

function Task_00040701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦勇士你这么快就回来了，我的茶叶还没有处理完呢。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040701_step_table = {
		[1] = Task_00040701_step_01,
		[2] = Task_00040701_step_02,
		[10] = Task_00040701_step_10,
		};

function Task_00040701_step(step)
	if Task_00040701_step_table[step] ~= nil then
		return Task_00040701_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40701) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40701) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(3200);
	return true;
end

--��������
function Task_00040701_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40701);
end

--����Ľ�������
function Task_Accept_00030301()
	local player = GetPlayer();
	if player:GetLev() < 34 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30301) or task:HasCompletedTask(30301) or task:HasSubmitedTask(30301) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30207) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 34 then
		return false;
	end
	if task:HasAcceptedTask(30301) or task:HasCompletedTask(30301) or task:HasSubmitedTask(30301) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30207) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30301) == npcId and Task_Accept_00030301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "鬼魂游荡";
	elseif task:GetTaskSubmitNpc(30301) == npcId then
		if Task_Submit_00030301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "鬼魂游荡";
		elseif task:HasAcceptedTask(30301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "鬼魂游荡";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位年轻的勇士，真是很有勇气啊，居然敢独自一人来到这荒凉的万人墓，你可知道这里埋葬着成千上万的战争亡魂。";
	action.m_ActionMsg = "我已经知道很多关于天魔之战的事情了。";
	return action;
end

function Task_00030301_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那看来你也不是无端闯到这里来的，既然来了就帮老夫做点事情吧，阿兰若山谷中的战场废墟，最近似乎是闹鬼了，你去查看下是怎么回事吧。";
	action.m_ActionMsg = "闹鬼？难道是天魔将士的冤魂吗？";
	return action;
end

function Task_00030301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么样年轻人，没有被吓到吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030301_step_table = {
		[1] = Task_00030301_step_01,
		[2] = Task_00030301_step_02,
		[10] = Task_00030301_step_10,
		};

function Task_00030301_step(step)
	if Task_00030301_step_table[step] ~= nil then
		return Task_00030301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30301) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30301) then
		return false;
	end


	player:AddExp(5000);
	player:getCoin(6800);
	player:getTael(10);
	return true;
end

--��������
function Task_00030301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30301);
end

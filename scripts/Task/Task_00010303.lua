--����Ľ�������
function Task_Accept_00010303()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10303) or task:HasCompletedTask(10303) or task:HasSubmitedTask(10303) then
		return false;
	end
	if not task:HasSubmitedTask(10302) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10303) or task:HasCompletedTask(10303) or task:HasSubmitedTask(10303) then
		return false;
	end
	if not task:HasSubmitedTask(10302) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10303) == npcId and Task_Accept_00010303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "为王妃送信";
	elseif task:GetTaskSubmitNpc(10303) == npcId then
		if Task_Submit_00010303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "为王妃送信";
		elseif task:HasAcceptedTask(10303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "为王妃送信";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "没想到你居然帮我带来这么秘密的东西，相信你也见过并且知道送这个东西来的人在哪里了。";
	action.m_ActionMsg = "是的，王妃，我已经见过那个人了。";
	return action;
end

function Task_00010303_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那么就请你再跑一趟，把这封信送给那个请你来见我的人。";
	action.m_ActionMsg = "又让我回去啊……这……好吧。";
	return action;
end

function Task_00010303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我的勇士，你居然带来了悦意王妃的密信？让我看看";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010303_step_table = {
		[1] = Task_00010303_step_01,
		[2] = Task_00010303_step_02,
		[10] = Task_00010303_step_10,
		};

function Task_00010303_step(step)
	if Task_00010303_step_table[step] ~= nil then
		return Task_00010303_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010303() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15022, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10303) then
		return false;
	end
	task:AddTaskStep(10303);
	return true;
end



--�ύ����
function Task_00010303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15022,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(10303) then
		return false;
	end

	package:DelItemAll(15022,1);

	player:AddExp(1800);
	player:getCoin(1900);
	player:getTael(10);
	return true;
end

--��������
function Task_00010303_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15022,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10303);
end

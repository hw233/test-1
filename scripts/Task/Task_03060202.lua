--����Ľ�������
function Task_Accept_03060202()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 44 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(3060202) or task:HasCompletedTask(3060202) or task:HasSubmitedTask(3060202) then
		return false;
	end
	if not task:HasSubmitedTask(3060102) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_03060202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 44 then
		return false;
	end
	if task:HasAcceptedTask(3060202) or task:HasCompletedTask(3060202) or task:HasSubmitedTask(3060202) then
		return false;
	end
	if not task:HasSubmitedTask(3060102) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_03060202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(3060202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_03060202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(3060202) == npcId and Task_Accept_03060202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 3060202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "申请美食";
	elseif task:GetTaskSubmitNpc(3060202) == npcId then
		if Task_Submit_03060202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 3060202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "申请美食";
		elseif task:HasAcceptedTask(3060202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 3060202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "申请美食";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_03060202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我们的勇士"..GetPlayerName(GetPlayer()).."，多日不见，已经成长的让人刮目相看了，俨然一位无所畏惧四处征战的勇士了啊。这次回到地界，有什么事情吗？";
	action.m_ActionMsg = "我遇到一个因对抗魔族受伤的兄弟，想申请一点点美食来救活他。";
	return action;
end

function Task_03060202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "难得你有此善心，那么我就批准送你一点吧。";
	action.m_ActionMsg = "非常感谢您。";
	return action;
end

function Task_03060202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哇，勇士回来了，带美食回来了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_03060202_step_table = {
		[1] = Task_03060202_step_01,
		[2] = Task_03060202_step_02,
		[10] = Task_03060202_step_10,
		};

function Task_03060202_step(step)
	if Task_03060202_step_table[step] ~= nil then
		return Task_03060202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_03060202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_03060202() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15044, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(3060202) then
		return false;
	end
	task:AddTaskStep(3060202);
	return true;
end



--�ύ����
function Task_03060202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15044,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(3060202) then
		return false;
	end

	package:DelItemAll(15044,1);

	player:AddExp(32000);
	player:getCoin(22000);
	player:getTael(10);
	return true;
end

--��������
function Task_03060202_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15044,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(3060202);
end

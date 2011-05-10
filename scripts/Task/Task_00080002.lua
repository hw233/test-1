--����Ľ�������
function Task_Accept_00080002()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80002) or task:HasCompletedTask(80002) or task:HasSubmitedTask(80002) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080002()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80002) or task:HasCompletedTask(80002) or task:HasSubmitedTask(80002) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080002()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80002) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080002(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80002) == npcId and Task_Accept_00080002 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80002
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "找寻名将";
	elseif task:GetTaskSubmitNpc(80002) == npcId then
		if Task_Submit_00080002() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80002
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "找寻名将";
		elseif task:HasAcceptedTask(80002) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80002
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "找寻名将";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080002_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我亲爱的魔族勇士，你终于成长到可以开始收服名将了，我们的英雄志显示了当前所有可以收服的名将，他们如今散落的山海大陆的各个角落，只要给他们定期送喜好品，就能够逐渐赢得对他们的友好值，直至将他们收服作为自己的武将，他们可是具有超越普通武将的强大实力的名将，能够与他们为伍，付出怎样的代价都是值得的。";
	action.m_ActionMsg = "可以我要怎么才能得到喜好品呢？";
	return action;
end

function Task_00080002_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们城里的银币商人和山河城的银币商人都出售各种喜好品，每次买上4个，就可以送给收集这种喜好品的名将，从而赢得对他的友好值增加。当然如果你运气足够好，山海大陆的一些怪物，也有很小几率掉落这些珍贵的喜好品。只要完成一次就可以回到我这里，获得丰厚的奖励了。";
	action.m_ActionMsg = "知道了，我会去看看的。";
	return action;
end

function Task_00080002_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "希望你可以顺利收服自己喜欢的独一无二的名将。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080002_step_table = {
		[1] = Task_00080002_step_01,
		[2] = Task_00080002_step_02,
		[10] = Task_00080002_step_10,
		};

function Task_00080002_step(step)
	if Task_00080002_step_table[step] ~= nil then
		return Task_00080002_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080002_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080002() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80002) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080002_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80002) then
		return false;
	end


	player:getTael(200);
	return true;
end

--��������
function Task_00080002_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80002);
end

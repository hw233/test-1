--����Ľ�������
function Task_Accept_00031701()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 67 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31701) or task:HasCompletedTask(31701) or task:HasSubmitedTask(31701) then
		return false;
	end
	if not task:HasSubmitedTask(31505) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 67 then
		return false;
	end
	if task:HasAcceptedTask(31701) or task:HasCompletedTask(31701) or task:HasSubmitedTask(31701) then
		return false;
	end
	if not task:HasSubmitedTask(31505) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31701) == npcId and Task_Accept_00031701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "震惊吕布之死";
	elseif task:GetTaskSubmitNpc(31701) == npcId then
		if Task_Submit_00031701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "震惊吕布之死";
		elseif task:HasAcceptedTask(31701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "震惊吕布之死";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我才听到手下禀报，说著名的名将吕布，刚来到山海大陆没有多久就被你杀死了，你是不是逞英雄之能啊，这件事情已经惊动了天族在昆仑殿的光盖天了。";
	action.m_ActionMsg = "我不是故意这样做的，他其实是借我的力量自杀的。";
	return action;
end

function Task_00031701_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你这样说谁会相信，昆仑殿的光盖天已经非常愤怒了，他连着派人让我找出是谁杀死的吕布，并且他要亲自问你，我想你还是去跟他解释一下吧，本来我们两族之间的矛盾已经很激烈了，不要触发什么严重的事情最好。";
	action.m_ActionMsg = "好吧，我亲自去跟他解释。";
	return action;
end

function Task_00031701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你就是那个杀死吕布的人？你还真的来见我了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031701_step_table = {
		[1] = Task_00031701_step_01,
		[2] = Task_00031701_step_02,
		[10] = Task_00031701_step_10,
		};

function Task_00031701_step(step)
	if Task_00031701_step_table[step] ~= nil then
		return Task_00031701_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31701) then
		return false;
	end
	task:AddTaskStep(31701);
	return true;
end



--�ύ����
function Task_00031701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31701) then
		return false;
	end


	player:AddExp(200000);
	player:getCoin(180000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031701_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31701);
end

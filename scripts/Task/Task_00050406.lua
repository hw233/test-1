--����Ľ�������
function Task_Accept_00050406()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50406) or task:HasCompletedTask(50406) or task:HasSubmitedTask(50406) then
		return false;
	end
	if not task:HasSubmitedTask(50405) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050406()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(50406) or task:HasCompletedTask(50406) or task:HasSubmitedTask(50406) then
		return false;
	end
	if not task:HasSubmitedTask(50405) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050406()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50406) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050406(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50406) == npcId and Task_Accept_00050406 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50406
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "永生的效果";
	elseif task:GetTaskSubmitNpc(50406) == npcId then
		if Task_Submit_00050406() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50406
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "永生的效果";
		elseif task:HasAcceptedTask(50406) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50406
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "永生的效果";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050406_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，若要让你了解到永生的真谛，在服下长生丹药之后，你将面临与我的战斗，如果人真的可以永生，你即便实力不如我，也不会之我面前失败，准备好了就服下这丹药吧。";
	action.m_ActionMsg = "（服下丹药）";
	return action;
end

function Task_00050406_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们的战斗就要开始了，很快便可知分晓。然后你就可以回到七叶堂去见罗睺罗了。";
	action.m_ActionMsg = "但愿这颗丹药有效果，为了它我可是吃了不少苦头。";
	return action;
end

function Task_00050406_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，在这一系列的经历之后，你得到成长了吗";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050406_step_table = {
		[1] = Task_00050406_step_01,
		[2] = Task_00050406_step_02,
		[10] = Task_00050406_step_10,
		};

function Task_00050406_step(step)
	if Task_00050406_step_table[step] ~= nil then
		return Task_00050406_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050406_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050406() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50406) then
		return false;
	end
	RunAutoBattleAction(player,4170,100);
	task:AddTaskStep(50406);

	return true;
end



--�ύ����
function Task_00050406_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50406) then
		return false;
	end


	player:AddExp(1200);
	player:getCoin(1250);
	return true;
end

--��������
function Task_00050406_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50406);
end

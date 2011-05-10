--����Ľ�������
function Task_Accept_00030604()
	local player = GetPlayer();
	if player:GetLev() < 44 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30604) or task:HasCompletedTask(30604) or task:HasSubmitedTask(30604) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30603) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30603) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030604()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 44 then
		return false;
	end
	if task:HasAcceptedTask(30604) or task:HasCompletedTask(30604) or task:HasSubmitedTask(30604) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30603) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30603) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030604()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30604) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030604(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30604) == npcId and Task_Accept_00030604 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30604
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "头晕脑胀";
	elseif task:GetTaskSubmitNpc(30604) == npcId then
		if Task_Submit_00030604() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30604
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "头晕脑胀";
		elseif task:HasAcceptedTask(30604) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30604
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "头晕脑胀";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030604_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我手下的小鬼刚刚向我报告了，是你这位善心的勇士用自己族的特产救活了我们的兄弟，我们非常感谢你。";
	action.m_ActionMsg = "我们有共同的敌人，不必客气，不过我怎么头晕脑胀的？";
	return action;
end

function Task_00030604_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不会的，是你的错觉吧，不过如果你能帮我去焚林谷打败那里的焚尸怪，并且将他们收服，我倒是可以找我们这里的医官为你诊一下脉。";
	action.m_ActionMsg = "那些焚尸怪对我来说都是些虾兵蟹将,等我的好消息吧。";
	return action;
end

function Task_00030604_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "还是有点本事的嘛，看来把你弄来一点错都没有。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030604_step_table = {
		[1] = Task_00030604_step_01,
		[2] = Task_00030604_step_02,
		[10] = Task_00030604_step_10,
		};

function Task_00030604_step(step)
	if Task_00030604_step_table[step] ~= nil then
		return Task_00030604_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030604_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030604() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30604) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030604_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30604) then
		return false;
	end


	player:AddExp(32000);
	player:getCoin(22000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030604_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30604);
end

--����Ľ�������
function Task_Accept_00050502()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50502) or task:HasCompletedTask(50502) or task:HasSubmitedTask(50502) then
		return false;
	end
	if not task:HasSubmitedTask(50501) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(50502) or task:HasCompletedTask(50502) or task:HasSubmitedTask(50502) then
		return false;
	end
	if not task:HasSubmitedTask(50501) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50502) == npcId and Task_Accept_00050502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "电闪骷髅";
	elseif task:GetTaskSubmitNpc(50502) == npcId then
		if Task_Submit_00050502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "电闪骷髅";
		elseif task:HasAcceptedTask(50502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "电闪骷髅";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你会回到这天鹰寺来找我，定是因为又有什么事情了吧，说吧，我一向很愿意帮助像你这样的勇士。";
	action.m_ActionMsg = "名将伍天锡求助于你，不知为何他的威力受限了。";
	return action;
end

function Task_00050502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是这样，他并不是第一个因为来到我们地界而遇上这样的麻烦的人，其实是因为灰烬谷的乱石堆，藏匿了许多电闪骷髅，他们离这里并不近，普通人感受不到他辐射出来的影响，可是像伍天锡这样的名将，对于力量非常敏感，电闪骷髅的辐射尽管离的远，还是会制约到他们，你去解决一下吧。搞定以后去告诉伍天锡吧。";
	action.m_ActionMsg = "电闪骷髅这么厉害吗？那我去会会他们。";
	return action;
end

function Task_00050502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，很高兴看到你回来了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050502_step_table = {
		[1] = Task_00050502_step_01,
		[2] = Task_00050502_step_02,
		[10] = Task_00050502_step_10,
		};

function Task_00050502_step(step)
	if Task_00050502_step_table[step] ~= nil then
		return Task_00050502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050502() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50502) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00050502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50502) then
		return false;
	end


	player:AddExp(1400);
	player:getCoin(1700);
	return true;
end

--��������
function Task_00050502_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50502);
end

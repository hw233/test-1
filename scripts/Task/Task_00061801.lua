--����Ľ�������
function Task_Accept_00061801()
	local player = GetPlayer();
	if player:GetLev() < 66 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61801) or task:HasCompletedTask(61801) or task:HasSubmitedTask(61801) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 66 then
		return false;
	end
	if task:HasAcceptedTask(61801) or task:HasCompletedTask(61801) or task:HasSubmitedTask(61801) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61801) == npcId and Task_Accept_00061801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "登上猎户山";
	elseif task:GetTaskSubmitNpc(61801) == npcId then
		if Task_Submit_00061801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "登上猎户山";
		elseif task:HasAcceptedTask(61801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "登上猎户山";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我们东山派的朋友你好，猎户山所在的这片山脉地区，绵延起伏，山势雄伟，我也是因为喜爱猎户山的风景，才选址驻扎在这里，你可以四周转一下，不要错过这引人入胜的美景。";
	action.m_ActionMsg = "好的，知道了。";
	return action;
end

function Task_00061801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "然了，很多人其实都是赞同我的观点的，甚至有一位名将也来到我们猎户山，我觉得他也很会选地方啊，你要不要去拜见一下那位叫僧格林沁的名将啊。";
	action.m_ActionMsg = "可以，我去看看。";
	return action;
end

function Task_00061801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "猎户山上好风光，青山白云流水长。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061801_step_table = {
		[1] = Task_00061801_step_01,
		[2] = Task_00061801_step_02,
		[10] = Task_00061801_step_10,
		};

function Task_00061801_step(step)
	if Task_00061801_step_table[step] ~= nil then
		return Task_00061801_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61801) then
		return false;
	end
	task:AddTaskStep(61801);
	return true;
end



--�ύ����
function Task_00061801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61801) then
		return false;
	end


	player:AddExp(190000);
	player:getCoin(165000);
	return true;
end

--��������
function Task_00061801_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61801);
end

--����Ľ�������
function Task_Accept_00000012()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(12) or task:HasCompletedTask(12) or task:HasSubmitedTask(12) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(11) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(11) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000012()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(12) or task:HasCompletedTask(12) or task:HasSubmitedTask(12) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(11) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(11) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000012()
	if GetPlayer():GetTaskMgr():HasCompletedTask(12) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000012(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(12) == npcId and Task_Accept_00000012 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 12
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "任务名称	女儿红";
	elseif task:GetTaskSubmitNpc(12) == npcId then
		if Task_Submit_00000012() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 12
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "任务名称	女儿红";
		elseif task:HasAcceptedTask(12) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 12
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "任务名称	女儿红";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000012_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位客官来得真不巧啊，本店原本有上好的女儿红，开坛十里香，是远近闻名啊，只是现在本店的女儿红是一滴也不剩了。";
	action.m_ActionMsg = "这位客官来得真不巧啊，本店原本有上好的女儿红，开坛十里香，是远近奇怪，酒楼怎会没有酒卖呢？掌柜的可否在说笑。";
	return action;
end

function Task_00000012_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "客官不知，上个月来了一伙贼兵，他们是为祸四川多年那张献忠的残部，四处流窜，无恶不作，小号的酒全被他们免费征去了。客官想找酒……只有去那伙贼兵的营地了。";
	action.m_ActionMsg = "奇怪，酒楼怎会没有酒卖呢？掌柜的可否在说笑。";
	return action;
end

function Task_00000012_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "客官不知，上个月来了一伙贼兵，他们是为祸四川多年那张献忠的残部，四处流窜，无恶不作，小号的酒全被他们免费征去了。客官想找酒……只有去那伙贼兵的营地了。";
	action.m_ActionMsg = "岂有此理，待我去铲除这些恶人。";
	return action;
end

function Task_00000012_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这酒不错，<name>看你眼光清明，也是正道中人，这本修道功法我就传授于你，你们的忙我也一定帮……哈哈，好酒！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000012_step_table = {
		[1] = Task_00000012_step_01,
		[2] = Task_00000012_step_02,
		[3] = Task_00000012_step_03,
		[10] = Task_00000012_step_10,
		};

function Task_00000012_step(step)
	if Task_00000012_step_table[step] ~= nil then
		return Task_00000012_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000012_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000012() then
		return false;
	end
	if not task:AcceptTask(12) then
		return false;
	end
	task:AddTaskStep(12);
	return true;
end



--�ύ����
function Task_00000012_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(12) then
		return false;
	end


	player:AddExp(1000);
	return true;
end

--��������
function Task_00000012_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(12);
end

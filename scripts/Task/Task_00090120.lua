--����Ľ�������
function Task_Accept_00090120()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90120) or task:HasCompletedTask(90120) or task:HasSubmitedTask(90120) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90119) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090120()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90120) or task:HasCompletedTask(90120) or task:HasSubmitedTask(90120) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90119) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090120()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90120) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090120(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90120) == npcId and Task_Accept_00090120 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90120
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "鲜美的食材";
	elseif task:GetTaskSubmitNpc(90120) == npcId then
		if Task_Submit_00090120() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90120
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "鲜美的食材";
		elseif task:HasAcceptedTask(90120) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90120
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "鲜美的食材";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090120_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我的手艺超级棒。";
	action.m_ActionMsg = "那也要等尝过以后才知道，你得赶紧做了，大伙都等着呢。";
	return action;
end

function Task_00090120_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "夜摩城的将士们想要补充体力，那可少不了落魄渔人的鱼肉。你去帮我弄20块来。";
	action.m_ActionMsg = "啊？那些半人半鱼的家伙？吃怪物的肉？我怎么感觉有点反胃。";
	return action;
end

function Task_00090120_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这可是上等的鱼肉啊，我做好后会马上送过去的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090120_step_table = {
		[1] = Task_00090120_step_01,
		[2] = Task_00090120_step_02,
		[10] = Task_00090120_step_10,
		};

function Task_00090120_step(step)
	if Task_00090120_step_table[step] ~= nil then
		return Task_00090120_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090120_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090120() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90120) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090120_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15106,1) < 20 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90120) then
		return false;
	end

	package:DelItemAll(15106,1);

	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--��������
function Task_00090120_abandon()
	local package = GetPlayer():GetPackage();
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90120);
end

--����Ľ�������
function Task_Accept_00090119()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90119) or task:HasCompletedTask(90119) or task:HasSubmitedTask(90119) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090119()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90119) or task:HasCompletedTask(90119) or task:HasSubmitedTask(90119) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090119()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90119) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090119(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90119) == npcId and Task_Accept_00090119 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90119
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "美食";
	elseif task:GetTaskSubmitNpc(90119) == npcId then
		if Task_Submit_00090119() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90119
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "美食";
		elseif task:HasAcceptedTask(90119) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90119
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "美食";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090119_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你帮我跑一趟吧。";
	action.m_ActionMsg = "这回不会是又要去探望谁吧？";
	return action;
end

function Task_00090119_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不是，我想让你去找天池的路之遥，让他做些美食送来夜摩城犒劳作战的将士们，大家都需要补充体力。";
	action.m_ActionMsg = "看来路之遥的手艺挺不错的呀，大家都喜欢吃他做的食物。";
	return action;
end

function Task_00090119_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我的手艺超级棒。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090119_step_table = {
		[1] = Task_00090119_step_01,
		[2] = Task_00090119_step_02,
		[10] = Task_00090119_step_10,
		};

function Task_00090119_step(step)
	if Task_00090119_step_table[step] ~= nil then
		return Task_00090119_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090119_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090119() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90119) then
		return false;
	end
	task:AddTaskStep(90119);
	return true;
end



--�ύ����
function Task_00090119_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90119) then
		return false;
	end


	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--��������
function Task_00090119_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90119);
end

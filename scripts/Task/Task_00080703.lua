--����Ľ�������
function Task_Accept_00080703()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80703) or task:HasCompletedTask(80703) or task:HasSubmitedTask(80703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80702) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080703()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80703) or task:HasCompletedTask(80703) or task:HasSubmitedTask(80703) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80702) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80702) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080703()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80703) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080703(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80703) == npcId and Task_Accept_00080703 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80703
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "毁坏玄幻坡";
	elseif task:GetTaskSubmitNpc(80703) == npcId then
		if Task_Submit_00080703() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80703
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "毁坏玄幻坡";
		elseif task:HasAcceptedTask(80703) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80703
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "毁坏玄幻坡";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080703_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这可是极难得的玄幻草呀。";
	action.m_ActionMsg = "这株小的草有什么作用？";
	return action;
end

function Task_00080703_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "你可别小看它，它迷惑人的功效可是极强的。原来夜摩盟也发现它了，不能让他们利用这些草。";
	action.m_ActionMsg = "那我们该怎么办？";
	return action;
end

function Task_00080703_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "刚才我给这株草加了点东西（坏坏的笑），现在你带去玄幻坡使用它，然后去向勒勤汇报那的情况。";
	action.m_ActionMsg = "我倒要看看这株草的神奇之处。";
	return action;
end

function Task_00080703_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可惜了那一大片的玄幻草。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080703_step_table = {
		[1] = Task_00080703_step_01,
		[2] = Task_00080703_step_02,
		[3] = Task_00080703_step_03,
		[10] = Task_00080703_step_10,
		};

function Task_00080703_step(step)
	if Task_00080703_step_table[step] ~= nil then
		return Task_00080703_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080703_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080703() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15103, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(80703) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080703_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80703) then
		return false;
	end


	player:AddExp(675000);
	player:getCoin(314000);
	return true;
end

--��������
function Task_00080703_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80703);
end

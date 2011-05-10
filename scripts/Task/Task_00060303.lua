--����Ľ�������
function Task_Accept_00060303()
	local player = GetPlayer();
	if player:GetLev() < 33 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60303) or task:HasCompletedTask(60303) or task:HasSubmitedTask(60303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60302) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 33 then
		return false;
	end
	if task:HasAcceptedTask(60303) or task:HasCompletedTask(60303) or task:HasSubmitedTask(60303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60302) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60303) == npcId and Task_Accept_00060303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "爱莫能助";
	elseif task:GetTaskSubmitNpc(60303) == npcId then
		if Task_Submit_00060303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "爱莫能助";
		elseif task:HasAcceptedTask(60303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "爱莫能助";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位名将柴进还真是惨啊，居然到这片荒凉的阿兰若古城来了，这可不是什么好地方，既没有什么活人，连动物和植物也几乎没有，他能坚持生活下去也不容易。";
	action.m_ActionMsg = "所以他觉得孤独的生活太难了，问你愿不愿意和他搭伙";
	return action;
end

function Task_00060303_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "年轻人啊，你好好想想，我一个人守在这个地方怎么吃的上饭呢，我早已经修炼到完成不需要吃饭了，哈哈。你去告诉柴进吧，爱莫能助啊。";
	action.m_ActionMsg = "原来是这样。";
	return action;
end

function Task_00060303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我的天哪。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060303_step_table = {
		[1] = Task_00060303_step_01,
		[2] = Task_00060303_step_02,
		[10] = Task_00060303_step_10,
		};

function Task_00060303_step(step)
	if Task_00060303_step_table[step] ~= nil then
		return Task_00060303_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060303() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60303) then
		return false;
	end
	task:AddTaskStep(60303);
	return true;
end



--�ύ����
function Task_00060303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60303) then
		return false;
	end


	player:AddExp(4500);
	player:getCoin(6150);
	return true;
end

--��������
function Task_00060303_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60303);
end

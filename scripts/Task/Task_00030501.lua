--����Ľ�������
function Task_Accept_00030501()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30501) or task:HasCompletedTask(30501) or task:HasSubmitedTask(30501) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30410) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30410) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(30501) or task:HasCompletedTask(30501) or task:HasSubmitedTask(30501) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30410) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30410) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30501) == npcId and Task_Accept_00030501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "相思无解";
	elseif task:GetTaskSubmitNpc(30501) == npcId then
		if Task_Submit_00030501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "相思无解";
		elseif task:HasAcceptedTask(30501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30501
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "相思无解";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "看到了你帮我带来那个隐士的礼物了，真是辛苦你了，也难为他的一片痴心了，我从来没有接受过他，虽然他隐姓埋名躲在没有人烟的地方，可是每年都会送我喜欢的东西给我做礼物，可惜我已经有心爱的人了，也很无奈。";
	action.m_ActionMsg = "原来是这样，可是你已经跟心上人在一起了吗？";
	return action;
end

function Task_00030501_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "并没有，我爱的人他不能接受我，（苦笑）从你的印记可以看出你已经去过东山城，见过他们的帮主如影了吧，我一直爱着他，可是他是个执着的人，只坚持自己的信念。";
	action.m_ActionMsg = "倒是能感觉出他的执着。";
	return action;
end

function Task_00030501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你不会是在笑话我们这些性情中人吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030501_step_table = {
		[1] = Task_00030501_step_01,
		[2] = Task_00030501_step_02,
		[10] = Task_00030501_step_10,
		};

function Task_00030501_step(step)
	if Task_00030501_step_table[step] ~= nil then
		return Task_00030501_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30501) then
		return false;
	end
	task:AddTaskStep(30501);
	return true;
end



--�ύ����
function Task_00030501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30501) then
		return false;
	end


	player:AddExp(10000);
	player:getCoin(13500);
	return true;
end

--��������
function Task_00030501_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30501);
end

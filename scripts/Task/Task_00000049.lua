--����Ľ�������
function Task_Accept_00000049()
	local player = GetPlayer();
	if player:GetLev() < 29 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(49) or task:HasCompletedTask(49) or task:HasSubmitedTask(49) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000049()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 29 then
		return false;
	end
	if task:HasAcceptedTask(49) or task:HasCompletedTask(49) or task:HasSubmitedTask(49) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000049()
	if GetPlayer():GetTaskMgr():HasCompletedTask(49) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000049(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(49) == npcId and Task_Accept_00000049 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 49
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "再探慈云寺";
	elseif task:GetTaskSubmitNpc(49) == npcId then
		if Task_Submit_00000049() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 49
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "再探慈云寺";
		elseif task:HasAcceptedTask(49) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 49
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "再探慈云寺";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000049_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，其实醉道人前辈怕你出意外，一直跟着你去了慈云寺，方才寺内的恶人躁动也是前辈故意做的，方便少侠你去救人。";
	action.m_ActionMsg = "原来如此，我说怎么救人这么顺利。";
	return action;
end

function Task_00000049_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如今慈云寺大批的妖人已经被前辈引走了，临走时前辈让我转告你，慈云寺藏污纳垢，密室内有不少邪恶之徒，其中有个叫杨花的女贼作恶多端让少侠你去将她铲除。";
	action.m_ActionMsg = "我这就去再探慈云寺。";
	return action;
end

function Task_00000049_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "慈云寺里的这些妖人真是可恶！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000049_step_table = {
		[1] = Task_00000049_step_01,
		[2] = Task_00000049_step_02,
		[10] = Task_00000049_step_10,
		};

function Task_00000049_step(step)
	if Task_00000049_step_table[step] ~= nil then
		return Task_00000049_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000049_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000049() then
		return false;
	end
	if not task:AcceptTask(49) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000049_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(49) then
		return false;
	end


	player:AddExp(6000);
	return true;
end

--��������
function Task_00000049_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(49);
end

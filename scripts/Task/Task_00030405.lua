--����Ľ�������
function Task_Accept_00030405()
	local player = GetPlayer();
	if player:GetLev() < 37 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30405) or task:HasCompletedTask(30405) or task:HasSubmitedTask(30405) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030405()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 37 then
		return false;
	end
	if task:HasAcceptedTask(30405) or task:HasCompletedTask(30405) or task:HasSubmitedTask(30405) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30404) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030405()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30405) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030405(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30405) == npcId and Task_Accept_00030405 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30405
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "康国遗迹";
	elseif task:GetTaskSubmitNpc(30405) == npcId then
		if Task_Submit_00030405() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30405
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "康国遗迹";
		elseif task:HasAcceptedTask(30405) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30405
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "康国遗迹";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030405_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，帮主愿意为您打造这样的装备，真是对您很有诚意，可见您已经获得了我们东山派最大的信任，既然这样，我想您在对付夜摩盟的过程中也是肩负重任的。";
	action.m_ActionMsg = "这个我明白。";
	return action;
end

function Task_00030405_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可是我想你并没有非常了解我们的东山派，如果你想知道我们的历史，到那片破败的康国遗迹去看一看吧，有一位禅静大师终日在那里修行，他见证过我们东山派整个历史。";
	action.m_ActionMsg = "我现在去拜见这位大师。";
	return action;
end

function Task_00030405_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "施主没有什么重要的事情，就快离开这里吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030405_step_table = {
		[1] = Task_00030405_step_01,
		[2] = Task_00030405_step_02,
		[10] = Task_00030405_step_10,
		};

function Task_00030405_step(step)
	if Task_00030405_step_table[step] ~= nil then
		return Task_00030405_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030405_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030405() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30405) then
		return false;
	end
	task:AddTaskStep(30405);
	return true;
end



--�ύ����
function Task_00030405_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30405) then
		return false;
	end


	player:AddExp(6000);
	player:getCoin(9500);
	return true;
end

--��������
function Task_00030405_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30405);
end

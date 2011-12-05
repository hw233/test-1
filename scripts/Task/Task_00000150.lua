--����Ľ�������
function Task_Accept_00000150()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(150) or task:HasCompletedTask(150) or task:HasSubmitedTask(150) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000150()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(150) or task:HasCompletedTask(150) or task:HasSubmitedTask(150) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(149) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000150()
	if GetPlayer():GetTaskMgr():HasCompletedTask(150) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000150(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(150) == npcId and Task_Accept_00000150 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 150
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "雁荡斩蛟";
	elseif task:GetTaskSubmitNpc(150) == npcId then
		if Task_Submit_00000150() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 150
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "雁荡斩蛟";
		elseif task:HasAcceptedTask(150) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 150
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "雁荡斩蛟";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000150_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这只妖鲧狡诈异常，我在此守候了数日，它只是躲在江底深处不敢路面。眼看它劫期将至，这孽畜估计按捺不住，就要出来兴风作浪……它手下不少水族妖孽，需要你助我将它们尽数铲除。";
	action.m_ActionMsg = "没问题……其实我最喜欢帮助妹子了。";
	return action;
end

function Task_00000150_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，此间事了，对你对我都是莫大的功德啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000150_step_table = {
		[1] = Task_00000150_step_01,
		[10] = Task_00000150_step_10,
		};

function Task_00000150_step(step)
	if Task_00000150_step_table[step] ~= nil then
		return Task_00000150_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000150_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000150() then
		return false;
	end
	if not task:AcceptTask(150) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000150_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(150) then
		return false;
	end


	player:AddExp(90000);
	return true;
end

--��������
function Task_00000150_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(150);
end

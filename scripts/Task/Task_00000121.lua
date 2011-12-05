--����Ľ�������
function Task_Accept_00000121()
	local player = GetPlayer();
	if player:GetLev() < 32 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(121) or task:HasCompletedTask(121) or task:HasSubmitedTask(121) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000121()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 32 then
		return false;
	end
	if task:HasAcceptedTask(121) or task:HasCompletedTask(121) or task:HasSubmitedTask(121) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(50) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000121()
	if GetPlayer():GetTaskMgr():HasCompletedTask(121) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000121(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(121) == npcId and Task_Accept_00000121 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 121
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "方丈智通";
	elseif task:GetTaskSubmitNpc(121) == npcId then
		if Task_Submit_00000121() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 121
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "方丈智通";
		elseif task:HasAcceptedTask(121) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 121
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "方丈智通";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000121_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "慈云寺的方丈叫做智通，乃是当年五台派太乙混元祖师的弟子，一手飞剑使得出神入化。自从混元祖师在第二次峨眉斗剑中陨落，他手下的弟子各奔东西，而智通跑到成都建了这慈云寺，聚集了不少妖人，暗中图谋反攻正道。";
	action.m_ActionMsg = "原来还有这么一段故事啊。";
	return action;
end

function Task_00000121_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，魏家场一役，智通的师兄金身罗汉法元战败，其他妖人死的死伤的伤，现在智通一人独木难支，已经不成气候，"..GetPlayerName(GetPlayer()).."这智通就交给你对付了。";
	action.m_ActionMsg = "好，我这就去慈云寺铲除首恶。";
	return action;
end

function Task_00000121_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "慈云寺的首恶已除，其他助拳妖人在魏家场也溃不成军，"..GetPlayerName(GetPlayer()).."你居功居首位啊！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000121_step_table = {
		[1] = Task_00000121_step_01,
		[2] = Task_00000121_step_02,
		[10] = Task_00000121_step_10,
		};

function Task_00000121_step(step)
	if Task_00000121_step_table[step] ~= nil then
		return Task_00000121_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000121_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000121() then
		return false;
	end
	if not task:AcceptTask(121) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000121_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(121) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--��������
function Task_00000121_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(121);
end

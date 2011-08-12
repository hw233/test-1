--����Ľ�������
function Task_Accept_00000018()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(18) or task:HasCompletedTask(18) or task:HasSubmitedTask(18) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000018()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(18) or task:HasCompletedTask(18) or task:HasSubmitedTask(18) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000018()
	if GetPlayer():GetTaskMgr():HasCompletedTask(18) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000018(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(18) == npcId and Task_Accept_00000018 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 18
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "蛇妖之害";
	elseif task:GetTaskSubmitNpc(18) == npcId then
		if Task_Submit_00000018() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 18
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "蛇妖之害";
		elseif task:HasAcceptedTask(18) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 18
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "蛇妖之害";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000018_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "多谢少侠送药之恩，那蛇妖修炼多年已经快成气候，它喷吐的毒雾厉害无比，所以啊剑术再高也怕菜刀，出门在外不随身带些救命的灵药说不定哪天就阴沟里翻船了，"..GetPlayerName(GetPlayer()).."你可要切记，买点自动回血符随身带着以备不时之需！ ";
	action.m_ActionMsg = "施大哥我知道了。";
	return action;
end

function Task_00000018_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，服了"..GetPlayerName(GetPlayer()).."你带来的灵药，我现在感觉好多了。这蛇妖不除始终是个麻烦，你这就快去白云寺吧，找白云大师和我师父商量怎么除掉这个妖孽。";
	action.m_ActionMsg = "好，我这就去！";
	return action;
end

function Task_00000018_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠古道热肠，真是多谢了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000018_step_table = {
		[1] = Task_00000018_step_01,
		[2] = Task_00000018_step_02,
		[10] = Task_00000018_step_10,
		};

function Task_00000018_step(step)
	if Task_00000018_step_table[step] ~= nil then
		return Task_00000018_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000018_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000018() then
		return false;
	end
	if not task:AcceptTask(18) then
		return false;
	end
	task:AddTaskStep(18);
	return true;
end



--�ύ����
function Task_00000018_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(18) then
		return false;
	end


	player:AddExp(2000);
	player:getTael(500);
	return true;
end

--��������
function Task_00000018_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(18);
end

--����Ľ�������
function Task_Accept_00000511()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(511) or task:HasCompletedTask(511) or task:HasSubmitedTask(511) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000511()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(511) or task:HasCompletedTask(511) or task:HasSubmitedTask(511) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000511()
	if GetPlayer():GetTaskMgr():HasCompletedTask(511) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000511(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(511) == npcId and Task_Accept_00000511 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 511
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "成都知府";
	elseif task:GetTaskSubmitNpc(511) == npcId then
		if Task_Submit_00000511() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 511
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "成都知府";
		elseif task:HasAcceptedTask(511) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 511
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "成都知府";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000511_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "明末党争剧烈，又赶上陕北大旱，闯王起义覆灭了明朝大好江山，真个时天灾人祸啊，如今满清入关，颠覆了我汉室江山，在这兵荒马乱的时节，象成都知府这样爱民如子的好官真是不多见，我见他任期将满，有意度他入我门派，"..GetPlayerName(GetPlayer()).."你去问问他意下如何？";
	action.m_ActionMsg = "好的，弟子这就去。";
	return action;
end

function Task_00000511_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "蒙仙长如此夸奖，真是愧不敢当，所谓当官应为民做主，这只是我为官的本分而已。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000511_step_table = {
		[1] = Task_00000511_step_01,
		[10] = Task_00000511_step_10,
		};

function Task_00000511_step(step)
	if Task_00000511_step_table[step] ~= nil then
		return Task_00000511_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000511_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000511() then
		return false;
	end
	if not task:AcceptTask(511) then
		return false;
	end
	task:AddTaskStep(511);
	return true;
end



--�ύ����
function Task_00000511_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(511) then
		return false;
	end


	player:AddExp(8000);
	return true;
end

--��������
function Task_00000511_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(511);
end

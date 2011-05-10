--����Ľ�������
function Task_Accept_00020303()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 19 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20303) or task:HasCompletedTask(20303) or task:HasSubmitedTask(20303) then
		return false;
	end
	if not task:HasSubmitedTask(20302) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 19 then
		return false;
	end
	if task:HasAcceptedTask(20303) or task:HasCompletedTask(20303) or task:HasSubmitedTask(20303) then
		return false;
	end
	if not task:HasSubmitedTask(20302) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20303) == npcId and Task_Accept_00020303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "阿修罗之子";
	elseif task:GetTaskSubmitNpc(20303) == npcId then
		if Task_Submit_00020303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "阿修罗之子";
		elseif task:HasAcceptedTask(20303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "阿修罗之子";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你是见到了文隣之墓所以来的吗？那我不妨告诉你，你在蒲昌海见过的那块石碑，是我为文隣立的，文隣是我跟阿修罗的儿子。";
	action.m_ActionMsg = "原来是王的儿子。";
	return action;
end

function Task_00020303_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "文隣是我们魔族的骄傲，他从小天赋异禀，跟随难陀罗大龙王修行，在天魔之战中，他奋勇战斗，却奉献出了自己的生命，我依然很想念我的儿子，蒲昌海是他出生的地方，所以我在那里给他立了石碑。\n";
	action.m_ActionMsg = "我们会永远纪念文隣。";
	return action;
end

function Task_00020303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，你来见我我很高兴。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020303_step_table = {
		[1] = Task_00020303_step_01,
		[2] = Task_00020303_step_02,
		[10] = Task_00020303_step_10,
		};

function Task_00020303_step(step)
	if Task_00020303_step_table[step] ~= nil then
		return Task_00020303_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020303() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20303) then
		return false;
	end
	task:AddTaskStep(20303);
	return true;
end



--�ύ����
function Task_00020303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20303) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(2100);
	player:getTael(10);
	return true;
end

--��������
function Task_00020303_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20303);
end

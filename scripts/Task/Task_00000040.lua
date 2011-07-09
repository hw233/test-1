--����Ľ�������
function Task_Accept_00000040()
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40) or task:HasCompletedTask(40) or task:HasSubmitedTask(40) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(39) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(39) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(39) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000040()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(40) or task:HasCompletedTask(40) or task:HasSubmitedTask(40) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(39) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(39) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(39) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000040()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000040(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40) == npcId and Task_Accept_00000040 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "炼制法宝";
	elseif task:GetTaskSubmitNpc(40) == npcId then
		if Task_Submit_00000040() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "炼制法宝";
		elseif task:HasAcceptedTask(40) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "炼制法宝";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000040_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我们蜀山剑侠之人，虽说是以剑入道，不过有些犀利的法宝在手还是很给力的……哈哈！我早已得知，你在白云大师那里得到一件丙灵梭，不过恐怕你还无法发挥它的全部威力。";
	action.m_ActionMsg = "那我要怎么做呢？";
	return action;
end

function Task_00000040_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你不是从王老丈那里得到一个五行菁英吗，那可是一件异物啊，可以用来重新整合法宝蕴含的灵气，大大提升法宝的威力，不过用之不当，说不定会损坏法宝。我有位老友廖臣明隐居在成都的珍宝斋，你现在可以去找他，让他教你如何使用。";
	action.m_ActionMsg = "嗯，我这就去。";
	return action;
end

function Task_00000040_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "醉道人这个酒鬼，老是给我找些麻烦。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000040_step_table = {
		[1] = Task_00000040_step_01,
		[2] = Task_00000040_step_02,
		[10] = Task_00000040_step_10,
		};

function Task_00000040_step(step)
	if Task_00000040_step_table[step] ~= nil then
		return Task_00000040_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000040_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000040() then
		return false;
	end
	if not task:AcceptTask(40) then
		return false;
	end
	task:AddTaskStep(40);
	return true;
end



--�ύ����
function Task_00000040_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000040_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(40);
end

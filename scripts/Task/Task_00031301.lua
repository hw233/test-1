--����Ľ�������
function Task_Accept_00031301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31301) or task:HasCompletedTask(31301) or task:HasSubmitedTask(31301) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31205) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31205) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31301) or task:HasCompletedTask(31301) or task:HasSubmitedTask(31301) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31205) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31205) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31301) == npcId and Task_Accept_00031301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "帮助逃亡";
	elseif task:GetTaskAcceptNpc(31301) == npcId and task:HasAcceptedTask(31301) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31301
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "帮助逃亡";
	elseif task:GetTaskSubmitNpc(31301) == npcId then
		if Task_Submit_00031301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "帮助逃亡";
		elseif task:HasAcceptedTask(31301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "帮助逃亡";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "年轻的勇士你好，啊……，有没有时间来帮我一下。";
	action.m_ActionMsg = "你是谁？你怎么受了这么重的伤在这里？";
	return action;
end

function Task_00031301_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我是墨无疾，本是驻扎在摩加湖的镇邪派的一位将士，我发现了摩睺已经暗自加入了夜摩盟，并且有一些邪恶的阴谋，我试图阻止他，可是我对他的黑暗魔法毫无办法，被他打成了重伤，连返回的力气都没有了，你愿意送我回去吗？";
	action.m_ActionMsg = "又是摩睺那家伙，好在我已经将他打败了，我这就送你回去。";
	return action;
end

function Task_00031301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "能有你这样的勇士的帮助和保护，虽然我最近很惨，但我还是感到很高兴。";
	action.m_ActionMsg = "";
	return action;
end

function Task_00031301_step_11()
	RunConveyAction(GetPlayer(),31301);
end

local Task_00031301_step_table = {
		[1] = Task_00031301_step_01,
		[2] = Task_00031301_step_02,
		[10] = Task_00031301_step_10,
		[11] = Task_00031301_step_11,
		};

function Task_00031301_step(step)
	if Task_00031301_step_table[step] ~= nil then
		return Task_00031301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31301) then
		return false;
	end
	RunConveyAction(player,31301);
	return true;
end



--�ύ����
function Task_00031301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31301) then
		return false;
	end


	player:AddExp(80000);
	player:getCoin(107500);
	player:getTael(40);
	return true;
end

--��������
function Task_00031301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31301);
end

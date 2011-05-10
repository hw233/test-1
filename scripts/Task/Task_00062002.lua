--����Ľ�������
function Task_Accept_00062002()
	local player = GetPlayer();
	if player:GetLev() < 71 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62002) or task:HasCompletedTask(62002) or task:HasSubmitedTask(62002) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62001) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62001) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062002()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 71 then
		return false;
	end
	if task:HasAcceptedTask(62002) or task:HasCompletedTask(62002) or task:HasSubmitedTask(62002) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62001) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62001) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062002()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62002) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062002(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62002) == npcId and Task_Accept_00062002 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62002
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "迷失心智";
	elseif task:GetTaskSubmitNpc(62002) == npcId then
		if Task_Submit_00062002() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62002
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "迷失心智";
		elseif task:HasAcceptedTask(62002) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62002
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "迷失心智";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062002_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位施主，到伏虎穴来所为何事？";
	action.m_ActionMsg = "大师，我受吕布之托，想请您给他算个命。";
	return action;
end

function Task_00062002_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "吕布这样的绝世名将，居然也会迷失心智，看不清自己的路啊，我来算算……，嗯这不是他的原因，是天河遗址的复仇亡魂，盯上了吕布，似乎他们之间曾经有什么冤孽，所以迷住了吕布的心智，施主只要去杀掉那些施法的亡魂，应该就没问题了。\n";
	action.m_ActionMsg = "原来是这样，那很简单，我这就去。";
	return action;
end

function Task_00062002_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "冤有头，债有主，迷人心智自己苦。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062002_step_table = {
		[1] = Task_00062002_step_01,
		[2] = Task_00062002_step_02,
		[10] = Task_00062002_step_10,
		};

function Task_00062002_step(step)
	if Task_00062002_step_table[step] ~= nil then
		return Task_00062002_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062002_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062002() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62002) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00062002_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62002) then
		return false;
	end


	player:AddExp(250000);
	player:getCoin(230000);
	return true;
end

--��������
function Task_00062002_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62002);
end

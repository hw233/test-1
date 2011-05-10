--����Ľ�������
function Task_Accept_00020203()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20203) or task:HasCompletedTask(20203) or task:HasSubmitedTask(20203) then
		return false;
	end
	if not task:HasSubmitedTask(20202) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020203()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(20203) or task:HasCompletedTask(20203) or task:HasSubmitedTask(20203) then
		return false;
	end
	if not task:HasSubmitedTask(20202) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020203()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20203) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020203(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20203) == npcId and Task_Accept_00020203 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20203
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "战神的愤怒";
	elseif task:GetTaskSubmitNpc(20203) == npcId then
		if Task_Submit_00020203() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20203
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "战神的愤怒";
		elseif task:HasAcceptedTask(20203) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20203
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "战神的愤怒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020203_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "（看完信大怒）帝释天实在是太不像话了，娶走了我们的悦意公主，现在却又完全冷落她，实在是太无耻狂妄了，以后我们一定不能轻饶了这个家伙。";
	action.m_ActionMsg = "帝释天怎么可以这样对待我们的公主呢？";
	return action;
end

function Task_00020203_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "他就是这样的喜新厌旧，总是不厌其烦的追求不同的美貌女子，即使已经有万千妻妾，还是硬要娶我们的悦意公主，甚至对她也是这么快就厌烦而冷落她了，我们相爱很久，而今却天地相隔不能见面。公主的近况，我们有必要让她的父亲毗摩质多罗知道。";
	action.m_ActionMsg = "那我现在就要去见传说中的火神了啊。";
	return action;
end

function Task_00020203_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是悦意的密信啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020203_step_table = {
		[1] = Task_00020203_step_01,
		[2] = Task_00020203_step_02,
		[10] = Task_00020203_step_10,
		};

function Task_00020203_step(step)
	if Task_00020203_step_table[step] ~= nil then
		return Task_00020203_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020203_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020203() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15027, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(20203) then
		return false;
	end
	task:AddTaskStep(20203);
	return true;
end



--�ύ����
function Task_00020203_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15027,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(20203) then
		return false;
	end

	package:DelItemAll(15027,1);

	player:AddExp(1400);
	player:getCoin(1100);
	return true;
end

--��������
function Task_00020203_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15027,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20203);
end

--����Ľ�������
function Task_Accept_00060302()
	local player = GetPlayer();
	if player:GetLev() < 33 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60302) or task:HasCompletedTask(60302) or task:HasSubmitedTask(60302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60301) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 33 then
		return false;
	end
	if task:HasAcceptedTask(60302) or task:HasCompletedTask(60302) or task:HasSubmitedTask(60302) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60301) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60301) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60302) == npcId and Task_Accept_00060302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "怨灵碎片";
	elseif task:GetTaskSubmitNpc(60302) == npcId then
		if Task_Submit_00060302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "怨灵碎片";
		elseif task:HasAcceptedTask(60302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "怨灵碎片";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "不过前些天我似乎看到不远处那个万人墓，有一个孤独的守墓人，从不见他说话，也没什么活动，不知道他是不是害怕战争废墟的那些怨灵鬼魂，你去把那些怪物打败吧。";
	action.m_ActionMsg = "嗯打掉那些怪物对我来不是什么问题，但是要做什么呢？";
	return action;
end

function Task_00060302_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "打掉那些鬼魂以后，收集他们的碎片，然后帮我找一下那个守墓人，给他这些碎片，告诉他这些怪物已经被杀掉了，不要再害怕了，看他愿不愿以后很我一起搭伙，再这样下去实在太无聊了。";
	action.m_ActionMsg = "原来你是想这样啊，好的，我去了。";
	return action;
end

function Task_00060302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "谁说我害怕那些鬼魂了，我要是害怕也不可能长年守在这万人墓里啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060302_step_table = {
		[1] = Task_00060302_step_01,
		[2] = Task_00060302_step_02,
		[10] = Task_00060302_step_10,
		};

function Task_00060302_step(step)
	if Task_00060302_step_table[step] ~= nil then
		return Task_00060302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60302) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15003,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15003, itemNum, 1);
		end
		task:AddTaskStep2(60302, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00060302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15003,1) < 12 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60302) then
		return false;
	end

	package:DelItemAll(15003,1);

	player:AddExp(4500);
	player:getCoin(6150);
	return true;
end

--��������
function Task_00060302_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15003,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60302);
end

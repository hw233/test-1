--����Ľ�������
function Task_Accept_00000037()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(37) or task:HasCompletedTask(37) or task:HasSubmitedTask(37) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000037()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(37) or task:HasCompletedTask(37) or task:HasSubmitedTask(37) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(88) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000037()
	if GetPlayer():GetTaskMgr():HasCompletedTask(37) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000037(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(37) == npcId and Task_Accept_00000037 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 37
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "青螺八魔";
	elseif task:GetTaskSubmitNpc(37) == npcId then
		if Task_Submit_00000037() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 37
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "青螺八魔";
		elseif task:HasAcceptedTask(37) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 37
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "青螺八魔";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000037_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "方才我以元神之法去青螺魔宫进行了探查，发现正道群英已经齐聚青螺山，准备进攻青螺魔宫，剿灭这处邪道的据点，"..GetPlayerName(GetPlayer()).."你也应该去助一臂之力。";
	action.m_ActionMsg = "嗯，我知道了。";
	return action;
end

function Task_00000037_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "青螺魔宫是八魔的师傅神手比丘魏枫娘所建，这个妖妇非常狡猾，他的弟子七魔许人龙昔日与我有点恩怨，正好趁此机会报昔年之仇。";
	action.m_ActionMsg = "你放心，我一定手刃此獠。";
	return action;
end

function Task_00000037_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你果然是正道的后起之秀，未来前途不可限量啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000037_step_table = {
		[1] = Task_00000037_step_01,
		[2] = Task_00000037_step_02,
		[10] = Task_00000037_step_10,
		};

function Task_00000037_step(step)
	if Task_00000037_step_table[step] ~= nil then
		return Task_00000037_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000037_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000037() then
		return false;
	end
	if not task:AcceptTask(37) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000037_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(37) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--��������
function Task_00000037_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(37);
end

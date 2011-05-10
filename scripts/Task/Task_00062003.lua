--����Ľ�������
function Task_Accept_00062003()
	local player = GetPlayer();
	if player:GetLev() < 71 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62003) or task:HasCompletedTask(62003) or task:HasSubmitedTask(62003) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62002) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62002) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062003()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 71 then
		return false;
	end
	if task:HasAcceptedTask(62003) or task:HasCompletedTask(62003) or task:HasSubmitedTask(62003) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62002) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62002) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062003()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62003) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062003(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62003) == npcId and Task_Accept_00062003 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62003
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "冥冥之签";
	elseif task:GetTaskSubmitNpc(62003) == npcId then
		if Task_Submit_00062003() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62003
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "冥冥之签";
		elseif task:HasAcceptedTask(62003) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62003
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "冥冥之签";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062003_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在可以了，没有外在的因素迷惑当事人，才有可能追求到真正准确的命运之符。如果施主真的有心替名将吕布算卦，就拿这冥冥签，去吕布所在的常泉寺外使用一下，便可得到属于吕布的冥冥签，然后回来给我看看。";
	action.m_ActionMsg = "真神奇啊，我去试试，你等我的消息。";
	return action;
end

function Task_00062003_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "天灵灵，地灵灵，山海大陆我最灵。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062003_step_table = {
		[1] = Task_00062003_step_01,
		[10] = Task_00062003_step_10,
		};

function Task_00062003_step(step)
	if Task_00062003_step_table[step] ~= nil then
		return Task_00062003_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062003_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062003() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15017, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(62003) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00062003_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15017,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(62003) then
		return false;
	end

	package:DelItemAll(15017,1);

	player:AddExp(250000);
	player:getCoin(230000);
	return true;
end

--��������
function Task_00062003_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15017,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62003);
end

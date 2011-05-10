--����Ľ�������
function Task_Accept_00050103()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50103) or task:HasCompletedTask(50103) or task:HasSubmitedTask(50103) then
		return false;
	end
	if not task:HasSubmitedTask(50102) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50103) or task:HasCompletedTask(50103) or task:HasSubmitedTask(50103) then
		return false;
	end
	if not task:HasSubmitedTask(50102) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50103) == npcId and Task_Accept_00050103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "回音贝";
	elseif task:GetTaskSubmitNpc(50103) == npcId then
		if Task_Submit_00050103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "回音贝";
		elseif task:HasAcceptedTask(50103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "回音贝";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然这样，我想你可以帮我去十宝山的蛮胡国，杀掉那里的暴戾巨人，收集一个回音贝，然后交给名人堂的张良，他在这方面很擅长，总是能做出非常好听的曲子，请他录一首在回音贝里，以后可以放在龙王庙里。";
	action.m_ActionMsg = "我想这会是一首很不错的曲子.";
	return action;
end

function Task_00050103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，很高兴再次见到你。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050103_step_table = {
		[1] = Task_00050103_step_01,
		[10] = Task_00050103_step_10,
		};

function Task_00050103_step(step)
	if Task_00050103_step_table[step] ~= nil then
		return Task_00050103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50103) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15048,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15048, itemNum, 1);
		end
		task:AddTaskStep2(50103, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00050103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15048,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(50103) then
		return false;
	end

	package:DelItemAll(15048,1);

	player:AddExp(850);
	player:getCoin(600);
	return true;
end

--��������
function Task_00050103_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15048,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50103);
end
